#ifndef COMMON_H
#define COMMON_H

#include <QObject>
#include <QTimer>
#include "global.h"
#include "renderprovider.h"
#include "mapdata.h"
#include "tiledata.h"

class common : public QObject
{
    Q_OBJECT
public:
    common();
    void bindRenderer(renderProvider *provider);
    Q_INVOKABLE void mousePosChanged(int x, int y);
    Q_INVOKABLE void regenerateMap();
    Q_INVOKABLE void mouseClick(int x, int y);
    Q_INVOKABLE void selectBuilding(int index);
    Q_INVOKABLE void changeDir();

private:
    QTimer *_tim;
    QTimer *_quick_tim;
    renderProvider *_renderer;
    mapData *_map;

    void addItemToTile(tileData *src, tileData *dest, int type, int amount);

private slots:
    void quickTick();
    void timerTick();
};

#endif // COMMON_H
