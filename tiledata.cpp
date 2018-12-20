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

void tileData::setHolderType(int i)
{
    holder_type = i;
}

int tileData::holderType()
{
    return holder_type;
}

void tileData::setHolderAmount(int i)
{
    holder_amount = i;
}

int tileData::holderAmount()
{
    return holder_amount;
}

void tileData::setHolderMax(int i)
{
    holder_max = i;
}

int tileData::holderMax()
{
    return holder_max;
}

void tileData::setUpdated()
{
    updated = true;
}

void tileData::resetUpdated()
{
    updated = false;
}

bool tileData::getUpdateStatus()
{
    return updated;
}

void tileData::setLastFeedDirection(int dir)
{
    last_fed_direction = dir;
}

int tileData::getLastFedDirection()
{
    return last_fed_direction;
}
