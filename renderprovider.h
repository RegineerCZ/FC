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

private:
    int calc = 0;
    //simplexNoiseGenerator *noiseGen;
    mapData *_map;
    int _mouseX = 0;
    int _mouseY = 0;
    QPixmap _map_image;
};

#endif // RENDERPROVIDER_H
