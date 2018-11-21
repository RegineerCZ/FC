#include "mapdata.h"

mapData::mapData()
{
    _first_layer = QList<tileData *>();
    _first_layer.clear ();
    _second_layer = QList<tileData *>();
    _second_layer.clear ();
    _third_layer = QList<tileData *>();
    _third_layer.clear ();

    _seed = 0;
    _width = 1;
    _height = 1;

    _noiseGen = new simplexNoiseGenerator();
}

void mapData::basicGeneration(int seed, int width, int height)
{
    _first_layer.clear ();
    _second_layer.clear ();
    _third_layer.clear ();

    _seed = seed; _width = width; _height = height;
    bool apply_coastal_noise = true;
    double min_water_level = 0.8;

    QTime t = QTime::currentTime();
    qsrand(t.msec());

    // First layer will be all grass for now
    // TODO Later add lakes, rivers and sea
    for (int y = 0; y < height; y++){
        for (int x = 0; x < width; x++){
            _first_layer.append (new tileData(tile_GRASS));
        }
    }
    //Apply water
    int water_cycles = ((_seed ^ 0x25846795) ^ (_seed ^ 0xa2bde89c)) % 10;
    qDebug() << "water_cycles: "<<water_cycles;
    for (int a = 0; a < water_cycles; a++){
        //qDebug() << "CYCLE "<<a;
        //qDebug() << "";
        int water_x = ((_seed ^ 0xa15dc235) ^ ((a * 0x12879) ^ 0x57912de5));
        int water_y = ((_seed ^ 0x28d6f3e2) ^ ((a * 0x7e8ba) ^ 0x36c5d48e));

        for (int y = 0; y < height; y++){
            QString row = "";
            for (int x = 0; x < width; x++){
                double water_v = qAbs(_noiseGen->noise ((x+water_x) / 150.0, (y+water_y) / 150.0));
                if (water_v > min_water_level){
                    if (apply_coastal_noise){
                        double r = (water_v - min_water_level);
                        double m = (1 - min_water_level);
                        double perc = r / m;
                        int val = perc * 100;
                        int rand = qAbs(_noiseGen->noise (x+water_y, y+water_x)) * 60;
                        int rand2 = qAbs(_noiseGen->noise (x+water_x, y+water_y)) * 60;
                        if (val > rand){
                            row = row + "#";
                            _first_layer.replace ((y*width)+x, new tileData(tile_WATER));
                        } else if (val > rand2){
                            row = row + "#";
                            _first_layer.replace ((y*width)+x, new tileData(tile_WATER));
                        } else {
                            row = row + "-";
                        }
                    }
                } else
                    row = row + "-";
            }
            //qDebug() << row;
        }
        //qDebug() << "";
        //qDebug() << "";
    }


    // Second layer will be coal and copper for now
    // TODO Later add more metals and minerals as well as trees and rocks
    int _copper_x = _seed ^ 0xAAAAAAAA;                         _copper_x = _copper_x & 0xFFFF;
    int _copper_y = ((_seed ^ 0xCCCCCCCC) >> 1) ^ 0x80808080;   _copper_y = _copper_y & 0xFFFF;
    int _coal_x = ((_seed ^ 0x80808080) >> 8) ^ _seed;          _coal_x = _coal_x & 0xFFFF;
    int _coal_y = (_seed ^ 0x12312312) ^ (_seed ^ 0x80808080);  _coal_y = _coal_y & 0xFFFF;
    qDebug() << "copper x,y: "<<_copper_x<<", "<<_copper_y;
    qDebug() << "coal x,y: "<<_coal_x<<", "<<_coal_y;

    for (int y = 0; y < height; y++){
        for (int x = 0; x < width; x++){
            double coal_v = _noiseGen->noise ((x+_coal_x) / 100.0, (y+_coal_y) / 100.0);
            double copper_v = _noiseGen->noise ((x+_copper_x) / 100.0, (y+_copper_y) / 100.0);
            coal_v = qAbs(coal_v);
            copper_v = qAbs(copper_v);
            //qDebug() << "vals: "<<coal_v<<", "<<copper_v;

            if (copper_v > tile_MIN_INTENSITY){
                double r = (copper_v - tile_MIN_INTENSITY);
                double m = (1 - tile_MIN_INTENSITY);
                double perc = r / m;
                int res = qAbs((perc * 200));//+((0x25867914 ^ _copper_x) % 100));
                //qDebug() << "COPP: "<<res;
                _second_layer.append (new tileData(tile_COPPER, res));
            } else if (coal_v > tile_MIN_INTENSITY){
                double r = (coal_v - tile_MIN_INTENSITY);
                double m = (1 - tile_MIN_INTENSITY);
                double perc = r / m;
                int res = qAbs((perc * 200));//+((0x25867914 ^ _coal_x) % 100));
                //qDebug() << "COAL: "<<res;
                _second_layer.append (new tileData(tile_COAL, res));
            } else {
                _second_layer.append (new tileData(tile_EMPTY));
            }
        }
    }

    // Third layer will be empty for now
    // TODO Later add buildings and enemy bases, doors and such
    for (int y = 0; y < height; y++){
        for (int x = 0; x < width; x++){
            _third_layer.append (new tileData(tile_EMPTY));
        }
    }

}

int mapData::seed()
{
    return _seed;
}

int mapData::width()
{
    return _width;
}

int mapData::height()
{
    return _height;
}

QList<tileData *> mapData::firstLayer()
{
    return _first_layer;
}

QList<tileData *> mapData::secondLayer()
{
    return _second_layer;
}

QList<tileData *> mapData::thirdLayer()
{
    return _third_layer;
}

void mapData::replaceThirdLayer(QList<tileData *> data)
{
    _third_layer.clear ();
    _third_layer.append (data);
}
