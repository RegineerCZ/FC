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
