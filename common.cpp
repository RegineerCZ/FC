#include "common.h"

common::common()
{
    _tim = new QTimer();
    connect(_tim, &QTimer::timeout, this, &common::timerTick);
    _tim->start (1000);
}

void common::bindRenderer(renderProvider *provider)
{
    _renderer = provider;
}

void common::mousePosChanged(int x, int y)
{
    _renderer->mousePosChanges (x, y);
}

void common::regenerateMap()
{
    _renderer->regenerateMap ();
}

void common::mouseClick(int x, int y)
{
    _renderer->mouseClick (x, y);
}

void common::selectBuilding(int index)
{
    _renderer->selectBuilding (index);
}

void common::changeDir()
{
    _renderer->changeDir ();
}

void common::timerTick()
{
    // Called every 1s
    _map = _renderer->getCurrentMap ();
    QList<tileData *> _f = _map->firstLayer ();
    QList<tileData *> _s = _map->secondLayer ();
    QList<tileData *> _t = _map->thirdLayer ();

    for(int y = 0; y < _map->height (); y++){
        for (int x = 0; x < _map->width (); x++){
            int pos = (y * _map->width ()) + x;
            int pos2 = (y * _map->width ()) + x + 1;
            int pos3 = ((y+1) * _map->width ()) + x;
            int pos4 = ((y+1) * _map->width ()) + x +1;
            if (pos4 < _t.size()){
                tileData *building = _t.at (pos);
                if (building->tileType () == tile_MINER){
                    int copper = 0; int coal = 0;
                    if (_s.at (pos)->tileType () == tile_COPPER){
                        copper += _s.at (pos)->tileResources ();
                    } else if (_s.at (pos2)->tileType () == tile_COPPER){
                        copper += _s.at (pos2)->tileResources ();
                    } else if (_s.at (pos3)->tileType () == tile_COPPER){
                        copper += _s.at (pos3)->tileResources ();
                    } else if (_s.at (pos4)->tileType () == tile_COPPER){
                        copper += _s.at (pos4)->tileResources ();
                    } else if (_s.at (pos)->tileType () == tile_COAL){
                        coal += _s.at (pos)->tileResources ();
                    } else if (_s.at (pos2)->tileType () == tile_COAL){
                        coal += _s.at (pos2)->tileResources ();
                    } else if (_s.at (pos3)->tileType () == tile_COAL){
                        coal += _s.at (pos3)->tileResources ();
                    } else if (_s.at (pos4)->tileType () == tile_COAL){
                        coal += _s.at (pos4)->tileResources ();
                    }

                    if (copper > 0){
                        // Just mined one copper
                        if (building->tileDirection () == dir_UP){
                            int dest_pos = pos2 -
                        } else if (building->tileDirection () == dir_RIGHT){

                        } else if (building->tileDirection () == dir_DOWN){

                        } else {

                        }
                    }
                }
            }
        }
    }
}
