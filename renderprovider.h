#ifndef RENDERPROVIDER_H
#define RENDERPROVIDER_H

#include <QTime>
#include <QColor>
#include <QPixmap>
#include <QPainter>
#include <QImage>
#include <QQuickImageProvider>
#include "global.h"
#include "mapdata.h"
#include "tiledata.h"
//#include "simplexnoisegenerator.h"

class renderProvider: public QQuickImageProvider
{
public:
    renderProvider();
    void generateImage();
    QPixmap requestPixmap(const QString &id, QSize *size, const QSize &requestedSize) override;
    void mousePosChanges(int x, int y);
    void regenerateMap();
    void mouseClick(int x, int y);
    void selectBuilding(int index);
    void changeDir();
    mapData *getCurrentMap();

private:

    int tile_size = 10;
    int calc = 0;
    int build_type = 1;
    int build_w = 1;
    int build_h = 1;
    int build_dir = dir_UP;
    //simplexNoiseGenerator *noiseGen;
    mapData *_map;
    int _mouseX = 0;
    int _mouseY = 0;
    QPixmap _map_image;

    void renderBelt(QPainter *painter, int x, int y, int dir);
    void renderMiner(QPainter *painter, int x, int y, int dir);
};

#endif // RENDERPROVIDER_H
