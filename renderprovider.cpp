#include "renderprovider.h"

renderProvider::renderProvider()
    : QQuickImageProvider(QQuickImageProvider::Pixmap/*, QQuickImageProvider::ForceAsynchronousImageLoading*/)
{
    //noiseGen = new simplexNoiseGenerator();
    _map = new mapData();
    _map->basicGeneration (0x2B7AC6E5, 30, 30);
    _map_image = QPixmap(30,30);
    _map_image.fill(QColor(0,0,0));
}

void renderProvider::generateImage()
{
    _map_image = QPixmap(_map->width ()*tile_size,_map->height ()*tile_size);
    _map_image.fill(QColor(0,0,0));
    QPainter painter(&_map_image);

    painter.setPen(QColor(0x80,0xFF,0x80));
    //painter.fillRect (0,0,3,3,Qt::SolidPattern);

    QList<tileData *> _f = _map->firstLayer ();

    for (int y = 0; y < _map->height (); y++){
        for (int x = 0; x < _map->width (); x++){
            if (_f.at ((y*_map->width ()) + x)->tileType () == tile_GRASS){
                painter.fillRect (x*tile_size, y*tile_size, tile_size, tile_size,QColor(0x80,0xFF,0x80));
            } else if (_f.at ((y*_map->width ()) + x)->tileType () == tile_WATER){
                painter.fillRect (x*tile_size, y*tile_size, tile_size, tile_size,QColor(0x80,0x80,0xFF));
            }
        }
    }

    QList<tileData *> _s = _map->secondLayer ();

    for (int y = 0; y < _map->height (); y++){
        //QString row = "";
        for (int x = 0; x < _map->width (); x++){
            //row = row + QString::number(_s.at ((y*_map->height ()) + x)->tileType ());
            if (_f.at ((y*_map->width ()) + x)->tileType () == tile_GRASS){
                if (_s.at ((y*_map->width ()) + x)->tileType () == tile_COAL){
                    painter.setPen(QColor(0,0,0));
                    int r = _s.at ((y*_map->width ()) + x)->tileResources ();
                    double a = (double)(r/100.0); if (a > 1.0) a = 1.0;
                    painter.fillRect (x*tile_size, y*tile_size, tile_size, tile_size,QColor(0,0,0,a*255));
                } else if (_s.at ((y*_map->width ()) + x)->tileType () == tile_COPPER){
                    painter.setPen(QColor(0xFF,0x80,0));
                    int r = _s.at ((y*_map->width ()) + x)->tileResources ();
                    double a = (double)(r/100.0); if (a > 1.0) a = 1.0;
                    painter.fillRect (x*tile_size, y*tile_size, tile_size, tile_size,QColor(0xFF,0x80,0,a*255));
                }
            }
        }
        //qDebug() << row;
    }

    QList<tileData *> _t = _map->thirdLayer ();

    for (int y = 0; y < _map->height (); y++){
        //QString row = "";
        for (int x = 0; x < _map->width (); x++){
            //row = row + QString::number(_s.at ((y*_map->height ()) + x)->tileType ());
            tileData *tile = _t.at ((y*_map->width ()) + x);
                if (tile->tileType () == tile_BELT){
                    renderBelt (&painter, x, y, tile->tileDirection ());
                    if (tile->holderAmount () > 0){
                        painter.fillRect (x*tile_size + (tile_size*0.25), y*tile_size + (tile_size*0.25), (tile_size/2), (tile_size/2),QColor(0,0,0));

                        painter.setPen(QColor(255,0,0));
                        painter.drawText(x*tile_size + (tile_size * 2), y*tile_size+tile_size, QString::number(tile->holderAmount ()));
                    }
                } else if (tile->tileType () == tile_MINER){
                    renderMiner (&painter, x, y, tile->tileDirection ());
                    if (tile->holderAmount () > 0){
                        painter.fillRect (x*tile_size + (tile_size*0.75), y*tile_size + (tile_size*0.75), (tile_size/2), (tile_size/2),QColor(0,0,0));

                        painter.setPen(QColor(255,0,0));
                        painter.drawText(x*tile_size + (tile_size * 2), y*tile_size+(tile_size*2), QString::number(tile->holderAmount ()));
                    }
                } else if (tile->holderAmount () > 0){
                    painter.fillRect (x*tile_size + (tile_size*0.25), y*tile_size + (tile_size*0.25), (tile_size/2), (tile_size/2),QColor(0,0,0));

                    painter.setPen(QColor(255,0,0));
                    painter.drawText(x*tile_size + (tile_size * 2), y*tile_size+tile_size, QString::number(tile->holderAmount ()));
                }
        }
        //qDebug() << row;
    }

    //int selection_size = 2;
    painter.setPen(QColor(0xFF,0x00,0x00));
    bool mouse_done = false;
    int mouse_map_x = (_mouseX / tile_size);
    int mouse_map_y = (_mouseY / tile_size);
    for (int y = 0; y < _map->height (); y++){
        if (mouse_done)
            break;
        for (int x = 0; x < _map->width (); x++){
            if (mouse_map_x == x){
                if (mouse_map_y == y){
                    painter.drawRect (x*tile_size, y*tile_size, build_w*tile_size, build_h*tile_size);
                    //Calculate center of building rect
                    int x_cen = (x*tile_size) + ((build_w*tile_size) / 2);
                    int y_cen = (y*tile_size) + ((build_h*tile_size) / 2);
                    if (build_dir == dir_UP){
                        painter.drawLine (x_cen, y_cen - (tile_size /2), x_cen - (tile_size /2), y_cen + (tile_size /2));
                        painter.drawLine (x_cen, y_cen - (tile_size /2), x_cen + (tile_size /2), y_cen + (tile_size /2));
                        painter.drawLine (x_cen - (tile_size /2), y_cen + (tile_size /2), x_cen + (tile_size /2), y_cen + (tile_size /2));
                    } else if (build_dir == dir_RIGHT){
                        painter.drawLine (x_cen + (tile_size /2), y_cen, x_cen - (tile_size /2), y_cen - (tile_size /2));
                        painter.drawLine (x_cen + (tile_size /2), y_cen, x_cen - (tile_size /2), y_cen + (tile_size /2));
                        painter.drawLine (x_cen - (tile_size /2), y_cen - (tile_size /2), x_cen - (tile_size /2), y_cen + (tile_size /2));
                    } else if (build_dir == dir_DOWN){
                        painter.drawLine (x_cen, y_cen + (tile_size /2), x_cen - (tile_size /2), y_cen - (tile_size /2));
                        painter.drawLine (x_cen, y_cen + (tile_size /2), x_cen + (tile_size /2), y_cen - (tile_size /2));
                        painter.drawLine (x_cen - (tile_size /2), y_cen - (tile_size /2), x_cen + (tile_size /2), y_cen - (tile_size /2));
                    } else {
                        painter.drawLine (x_cen - (tile_size /2), y_cen, x_cen + (tile_size /2), y_cen - (tile_size /2));
                        painter.drawLine (x_cen - (tile_size /2), y_cen, x_cen + (tile_size /2), y_cen + (tile_size /2));
                        painter.drawLine (x_cen + (tile_size /2), y_cen - (tile_size /2), x_cen + (tile_size /2), y_cen + (tile_size /2));
                    }
                    mouse_done = true;
                    break;
                }
            }
        }
    }

    int copper_res = 0;
    int coal_res = 0;

    for (int y = 0; y < build_h; y++){
        for (int x = 0; x < build_w; x++){
            int tile_pos = ((mouse_map_y+y)*_map->width ()) + (mouse_map_x+x);
            if (tile_pos < _s.size()){
                tileData *_tile = _s.at (tile_pos);
                if (_tile->tileType () == tile_COPPER){
                    copper_res = copper_res + _tile->tileResources ();
                } else if (_tile->tileType () == tile_COAL){
                    coal_res = coal_res + _tile->tileResources ();
                }
            }
        }
    }

    painter.setPen(QColor(255,0,0));
    painter.drawText(10, 10, "Ticks: " + QString::number(calc));
    painter.drawText(10, 30, "Copper: "+QString::number(copper_res));
    painter.drawText(10, 50, "Coal: "+QString::number(coal_res));
    calc = calc +1;
    qDebug() << "generated image tick: "<<calc;
}

QPixmap renderProvider::requestPixmap(const QString &id, QSize *size, const QSize &requestedSize)
{
    qDebug() << "renderRequest: "<<id<<", qSize("<<size->width ()<<", "<<size->height ()<<"), "<<requestedSize;
    /*if (calc == 0){
        //If this is first time calling renderer let's generate current map data

    }

    calc = calc+1;
    QPixmap pixmap(640,480);
    pixmap.fill(QColor(0,0,0));

    QPixmap *tile = new QPixmap(":/images/tiles/grass.png");
    QPixmap *coal = new QPixmap(":/images/tiles/coal.png");
    QPainter painter(&pixmap);

    QTime t = QTime::currentTime();
    qsrand(t.msec());
    int offset_x = qrand() % 100000;
    int offset_y = qrand() % 100000;

    QRectF source(0,0,tile->width(),tile->height());
    for (int y = 0; y < pixmap.height(); y = y + tile->height()){
        for (int x = 0; x < pixmap.width(); x = x + tile->width()){
            QRectF target(x,y,tile->width(),tile->height());
            painter.drawPixmap(target, *tile, source);
        }
    }


    for (int y = 0; y < pixmap.height(); y = y + tile->height()){
        for (int x = 0; x < pixmap.width(); x = x + tile->width()){
            QRectF target(x,y,tile->width(),tile->height());
            double v = noiseGen->noise(((x+offset_x)/1000.0), ((y+offset_y)/1000.0));
            int a = (v*255);
            int b = a %254;
            b = b -200;
            if (b > 0){
                painter.drawPixmap(target, *coal, source);
            }
        }
    }*/

    generateImage ();

    return _map_image;
}

void renderProvider::mousePosChanges(int x, int y)
{
    //qDebug() << "renderer mouse change";
    _mouseX = x;
    _mouseY = y;
}

void renderProvider::regenerateMap()
{
    int r = qrand() % 0xFFFFFFFF;
    qDebug() << "seed: "<<r;
    _map->basicGeneration (r, 100, 100);
}

void renderProvider::mouseClick(int x, int y)
{
    //For test we add a building at a place of click
    int block_layed = 0;
    int xt = x / tile_size;
    int yt = y / tile_size;
    QList<tileData *> _tl = _map->thirdLayer ();
    for (int yy = 0; yy < build_h; yy++){
        for (int xx = 0; xx < build_w; xx++){
            int xf = xt+xx;
            int yf = yt+yy;
            int indx = (yf * _map->width ()) + xf;
            if (indx < _tl.size ()){
                if (block_layed == 0){
                    tileData *o = _tl.at (indx);
                    int ht = o->holderType ();
                    int ha = o->holderAmount ();
                    if (build_type == 1){
                        tileData *d = new tileData(tile_BELT,0,build_dir);
                        if (ha > 10) ha = 10;
                        d->setHolderType (ht);
                        d->setHolderAmount (ha);
                        d->setHolderMax (10);
                        _tl.replace (indx, d);
                    } else {
                        tileData *d = new tileData(tile_MINER,0,build_dir);
                        if (ha > 4) ha = 4;
                        d->setHolderType (ht);
                        d->setHolderAmount (ha);
                        d->setHolderMax (4);
                        _tl.replace (indx, d);
                    }
                } else {
                    _tl.replace (indx, new tileData(tile_BLOCKED));
                }
                block_layed++;
            }
        }
    }
    _map->replaceThirdLayer (_tl);
}

void renderProvider::selectBuilding(int index)
{
    build_type = index;
    if (index == 1){
        build_w = 1;
        build_h = 1;
    } else {
        build_w = 2;
        build_h = 2;
    }
}

void renderProvider::changeDir()
{
    if (build_dir < 3){
        build_dir = build_dir + 1;
    } else {
        build_dir = 0;
    }
}

mapData *renderProvider::getCurrentMap()
{
    return _map;
}

void renderProvider::renderBelt(QPainter *painter, int x, int y, int dir)
{
    painter->setPen(QColor(0xFF,0xFF,0xFF));
    painter->drawRect (x*tile_size, y*tile_size, tile_size, tile_size);
    //Calculate center of building rect
    int x_cen = (x*tile_size) + (tile_size / 2);
    int y_cen = (y*tile_size) + (tile_size / 2);
    painter->setPen(QColor(0x80,0x80,0xFF));
    if (dir == dir_UP){
        painter->drawLine (x_cen, y_cen - (tile_size /2), x_cen - (tile_size /2), y_cen + (tile_size /2));
        painter->drawLine (x_cen, y_cen - (tile_size /2), x_cen + (tile_size /2), y_cen + (tile_size /2));
    } else if (dir == dir_RIGHT){
        painter->drawLine (x_cen + (tile_size /2), y_cen, x_cen - (tile_size /2), y_cen - (tile_size /2));
        painter->drawLine (x_cen + (tile_size /2), y_cen, x_cen - (tile_size /2), y_cen + (tile_size /2));
    } else if (dir == dir_DOWN){
        painter->drawLine (x_cen, y_cen + (tile_size /2), x_cen - (tile_size /2), y_cen - (tile_size /2));
        painter->drawLine (x_cen, y_cen + (tile_size /2), x_cen + (tile_size /2), y_cen - (tile_size /2));
    } else {
        painter->drawLine (x_cen - (tile_size /2), y_cen, x_cen + (tile_size /2), y_cen - (tile_size /2));
        painter->drawLine (x_cen - (tile_size /2), y_cen, x_cen + (tile_size /2), y_cen + (tile_size /2));
    }
}

void renderProvider::renderMiner(QPainter *painter, int x, int y, int dir)
{
    painter->setPen(QColor(0xFF,0xFF,0xFF));
    painter->drawRect (x*tile_size, y*tile_size, 2*tile_size, 2*tile_size);
    //Calculate center of building rect
    int x_cen = (x*tile_size) + tile_size;
    int y_cen = (y*tile_size) + tile_size;
    painter->setPen(QColor(0xFF,0x80,0x80));
    if (dir == dir_UP){
        painter->drawLine (x_cen + (tile_size /2), y_cen - (tile_size), x_cen, y_cen);
        painter->drawLine (x_cen + (tile_size /2), y_cen - (tile_size), x_cen + (tile_size), y_cen);
        painter->drawLine (x_cen, y_cen, x_cen + (tile_size), y_cen);
    } else if (dir == dir_RIGHT){
        painter->drawLine (x_cen + (tile_size), y_cen + (tile_size /2), x_cen, y_cen);
        painter->drawLine (x_cen + (tile_size), y_cen + (tile_size /2), x_cen, y_cen + (tile_size));
        painter->drawLine (x_cen, y_cen, x_cen, y_cen + (tile_size));
    } else if (dir == dir_DOWN){
        painter->drawLine (x_cen - (tile_size /2), y_cen + (tile_size), x_cen - (tile_size), y_cen);
        painter->drawLine (x_cen - (tile_size /2), y_cen + (tile_size), x_cen, y_cen);
        painter->drawLine (x_cen - (tile_size), y_cen, x_cen, y_cen);
    } else {
        painter->drawLine (x_cen - (tile_size), y_cen - (tile_size /2), x_cen, y_cen - (tile_size));
        painter->drawLine (x_cen - (tile_size), y_cen - (tile_size /2), x_cen, y_cen);
        painter->drawLine (x_cen, y_cen - (tile_size), x_cen, y_cen);
    }
}
