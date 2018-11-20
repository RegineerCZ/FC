#ifndef TILEDATA_H
#define TILEDATA_H

#include <QDebug>
#include <QObject>
#include "global.h"

class tileData : public QObject
{
    Q_OBJECT
public:
    tileData();
    tileData(int type);
    tileData(int type, int resources);
    void setTileType(int type);
    int tileType();
    void setTileResources(int resources);
    int tileResources();

private:
    int tile_type = tile_EMPTY;
    int tile_resources = 0;
};

#endif // TILEDATA_H
