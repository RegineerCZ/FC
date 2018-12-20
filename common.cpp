#include "common.h"

common::common()
{
    _tim = new QTimer();
    connect(_tim, &QTimer::timeout, this, &common::timerTick);
    _tim->start (1000);

    _quick_tim = new QTimer();
    connect(_quick_tim, &QTimer::timeout, this, &common::quickTick);
    _quick_tim->start (500); //TODO Make it much faster after debugging

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

void common::addItemToTile(tileData *src, tileData *dest, int type, int amount)
{
    if (dest->holderMax () > 0){
        // We can continue only if there is a place to store the item
        if (dest->holderType () == type){
            //We can add more only of the same item
            if ((dest->holderMax () - dest->holderAmount ()) >= amount){
                //If there is enough space to add new amount
                dest->setHolderAmount (dest->holderAmount () + amount);
            } else if (((src->holderMax () - src->holderAmount ()) + (dest->holderMax () - dest->holderAmount ())) >= amount) {
                //If there is enough space in dest and src at the same time
                if (src->holderType () == type){
                    //We can split the stack only if src has the same type of item as well
                    int split = (amount - (dest->holderMax () - dest->holderAmount ())); // Calculate how much will stay in src
                    dest->setHolderAmount (dest->holderMax ()); // Give all that is possible to dest
                    src->setHolderAmount (src->holderAmount () + split);
                } else if (src->holderAmount () == 0){
                    //Or we can add new item to src if it has none atm
                    int split = (amount - (dest->holderMax () - dest->holderAmount ())); // Calculate how much will stay in src
                    dest->setHolderAmount (dest->holderMax ()); // Give all that is possible to dest
                    src->setHolderType (type);
                    src->setHolderAmount (split);
                }
            }

        } else if (dest->holderAmount () == 0){
            //We can add new one if there is none yet
            if (dest->holderMax () >= amount){
                //If there is enough space to add new amount
                dest->setHolderType (type);
                dest->setHolderAmount (amount);
            } else if (((src->holderMax () - src->holderAmount ()) + dest->holderMax ()) >= amount) {
                //If there is enough space in dest and src at the same time
                if (src->holderType () == type){
                    //We can split the stack only if src has the same type of item as well
                    int split = amount - dest->holderMax (); // Calculate how much will stay in src
                    dest->setHolderType (type);
                    dest->setHolderAmount (dest->holderMax ()); // Give all that is possible to dest
                    src->setHolderAmount (src->holderAmount () + split);
                } else if (src->holderAmount () == 0){
                    //Or we can add new item to src if it has none atm
                    int split = amount - dest->holderMax (); // Calculate how much will stay in src
                    dest->setHolderType (type);
                    dest->setHolderAmount (dest->holderMax ()); // Give all that is possible to dest
                    src->setHolderType (type);
                    src->setHolderAmount (split);
                }
            }
        }

    } else {
        // Alternatively we can look if the source itself is able to hold this item until next movement of items
        if ((src->holderMax () - src->holderAmount ()) >= amount){
            //If there is enough space
            if (src->holderType () == type){
                // We can add only more of the same item as there already is
                src->setHolderAmount (src->holderAmount () + amount);
            } else if (src->holderAmount () == 0){
                // Or add new one if there is nothing yet
                src->setHolderType (type);
                src->setHolderAmount (amount);
            }
        }
    }
}

void common::quickTick()
{
    // Called very often, should have very little code in
    _map = _renderer->getCurrentMap ();
    QList<tileData *> _f = _map->firstLayer ();
    QList<tileData *> _s = _map->secondLayer ();
    QList<tileData *> _t = _map->thirdLayer ();

    for (int i = 0; i < _t.size(); i++){
        tileData *tile = _t.at (i);
        tile->resetUpdated ();
        _t.replace (i, tile);
    }
    for (int i = 0; i < _s.size(); i++){
        tileData *tile = _s.at (i);
        tile->resetUpdated ();
        _t.replace (i, tile);
    }
    for (int i = 0; i < _f.size(); i++){
        tileData *tile = _f.at (i);
        tile->resetUpdated ();
        _t.replace (i, tile);
    }

    QList<tempBeltChain *> chain = QList<tempBeltChain *>();
    // First we want to find first link in belt chains

    for(int y = 0; y < _map->height (); y++){
        for (int x = 0; x < _map->width (); x++){
            int pos = (y * _map->width ()) + x;
            if (pos < _t.size()){
                tileData *tile = _t.at (pos);
                if (tile->tileType () == tile_BELT){
                    // Found begging of the chain
                    int temp_pos = pos;
                    chain.clear ();
                    bool chaining = true;

                    while(chaining){
                        tileData *temp_tile = _t.at (temp_pos);

                        if ((temp_tile->tileType () == tile_BELT) && (temp_tile->getUpdateStatus () == false)){
                            chain.append (new tempBeltChain(temp_pos));

                            if (tile->tileDirection () == dir_UP){
                                temp_pos = temp_pos - _map->width ();
                            } else if (tile->tileDirection () == dir_LEFT) {
                                temp_pos = temp_pos -1;
                            } else if (tile->tileDirection () == dir_DOWN){
                                temp_pos = temp_pos + _map->width ();
                            } else {
                                temp_pos = temp_pos +1;
                            }

                            //TODO Treba skontrolovat ze dana pozicia existuje (ze nejsi mimo mapu)

                        } else {
                            // End of chain found
                            chaining = false;
                        }
                    }

                    while (chain.size () > 0){
                        //If any belts found, go from futherst one and update item positions
                        int temp_pos = chain.last ()->pos;
                        tileData *temp_tile = _t.at (temp_pos);
                        chain.removeLast ();

                        if (temp_tile->holderAmount () > 0){
                            int target_pos = temp_pos;
                            if (temp_tile->tileDirection () == dir_UP){
                                target_pos = target_pos - _map->width ();
                            } else if (temp_tile->tileDirection () == dir_LEFT){
                                target_pos = target_pos -1;
                            } else if (temp_tile->tileDirection () == dir_DOWN){
                                target_pos = target_pos + _map->width ();
                            } else {
                                target_pos = target_pos +1;
                            }

                            //TODO Treba skontrolovat ze dana pozicia existuje (ze nejsi mimo mapu)
                            //TODO Dorobit

                        }
                    }

                }
            }
        }
    }

}

void common::timerTick()
{
    //qDebug() << "TICK !";
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
                    //qDebug() << "miner found !";
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

                    int dest_pos = pos;
                    if (copper > 0){
                        // Just mined copper
                        if (building->tileDirection () == dir_UP)
                            dest_pos = pos2 - _map->width ();
                        else if (building->tileDirection () == dir_RIGHT)
                            dest_pos = pos4 + 1;
                        else if (building->tileDirection () == dir_DOWN)
                            dest_pos = pos3 + _map->width ();
                        else
                            dest_pos = pos - 1;

                        if ((dest_pos >= 0) && (dest_pos < _t.size ())){
                            tileData *dest_tile = _t.at (dest_pos);
                            addItemToTile (building, dest_tile, item_COPPER_ORE, 4);
                        }
                    } else if (coal > 0){
                        // Just mined coal
                        if (building->tileDirection () == dir_UP)
                            dest_pos = pos2 - _map->width ();
                        else if (building->tileDirection () == dir_RIGHT)
                            dest_pos = pos4 + 1;
                        else if (building->tileDirection () == dir_DOWN)
                            dest_pos = pos3 + _map->width ();
                        else
                            dest_pos = pos - 1;

                        if ((dest_pos >= 0) && (dest_pos < _t.size ())){
                            tileData *dest_tile = _t.at (dest_pos);
                            addItemToTile (building, dest_tile, item_COAL_ORE, 4);
                        }
                    }
                }
            }
        }
    }
}

tempBeltChain::tempBeltChain(int p)
{
    pos = p;
}
