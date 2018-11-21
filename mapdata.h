#ifndef MAPDATA_H
#define MAPDATA_H

#include <QDebug>
#include <QObject>
#include <QTime>
#include <QList>
#include "global.h"
#include "tiledata.h"
#include "simplexnoisegenerator.h"

class mapData : public QObject
{
    Q_OBJECT
public:
    mapData();
    void basicGeneration(int seed, int width, int height);

    int seed();
    int width();
    int height();
    QList<tileData *> firstLayer();
    QList<tileData *> secondLayer();
    QList<tileData *> thirdLayer();
    void replaceThirdLayer(QList<tileData*> data);

private:
    int _seed;
    int _width;
    int _height;

    QList<tileData *> _first_layer;
    QList<tileData *> _second_layer;
    QList<tileData *> _third_layer;

    simplexNoiseGenerator *_noiseGen;
};

#endif // MAPDATA_H
