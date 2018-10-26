#include "renderprovider.h"

renderProvider::renderProvider()
    : QQuickImageProvider(QQuickImageProvider::Pixmap)
{
    noiseGen = new simplexNoiseGenerator();
}

QPixmap renderProvider::requestPixmap(const QString &id, QSize *size, const QSize &requestedSize)
{
    if (calc == 0){
        //If this is first time calling renderer let's generate current map data

    }

    calc = calc+1;
    QPixmap pixmap(640,480);
    pixmap.fill(QColor(0,0,0));

    QPixmap *tile = new QPixmap(":/images/tiles/grass.png");
    QPixmap *coal = new QPixmap(":/images/tiles/coal.png");
    QPainter painter(&pixmap);

    QTime t = QTime::currentTime();
    qsrand(t.msec());
    int offset_x = qrand() % 100000;
    int offset_y = qrand() % 100000;

    QRectF source(0,0,tile->width(),tile->height());
    for (int y = 0; y < pixmap.height(); y = y + tile->height()){
        for (int x = 0; x < pixmap.width(); x = x + tile->width()){
            QRectF target(x,y,tile->width(),tile->height());
            painter.drawPixmap(target, *tile, source);
        }
    }


    for (int y = 0; y < pixmap.height(); y = y + tile->height()){
        for (int x = 0; x < pixmap.width(); x = x + tile->width()){
            QRectF target(x,y,tile->width(),tile->height());
            double v = noiseGen->noise(((x+offset_x)/1000.0), ((y+offset_y)/1000.0));
            int a = (v*255);
            int b = a %254;
            b = b -200;
            if (b > 0){
                painter.drawPixmap(target, *coal, source);
            }
        }
    }

    painter.setPen(QColor(255,0,0));
    painter.drawText(10, 10, "Render: " + QString::number(calc));

    return pixmap;
}
