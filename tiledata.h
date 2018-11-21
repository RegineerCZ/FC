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
    tileData(int type, int resources, int dir);
    void setTileType(int type);
    int tileType();
    void setTileResources(int resources);
    int tileResources();
    void setTileDirection(int dir);
    int tileDirection();

private:
    int tile_type = tile_EMPTY;
    int tile_resources = 0;
    int tile_dir = dir_UP;
};

#endif // TILEDATA_H
