#include "tiledata.h"

tileData::tileData()
{

}

tileData::tileData(int type)
{
    setTileType (type);
}

tileData::tileData(int type, int resources)
{
    setTileType (type);
    setTileResources (resources);
}

tileData::tileData(int type, int resources, int dir)
{
    setTileType (type);
    setTileResources (resources);
    setTileDirection (dir);
}

void tileData::setTileType(int type)
{
    tile_type = type;
}

int tileData::tileType()
{
    return tile_type;
}

void tileData::setTileResources(int resources)
{
    tile_resources = resources;
}

int tileData::tileResources()
{
    return tile_resources;
}

void tileData::setTileDirection(int dir)
{
    tile_dir = dir;
}

int tileData::tileDirection()
{
    return tile_dir;
}
