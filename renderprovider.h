#ifndef RENDERPROVIDER_H
#define RENDERPROVIDER_H

#include <QTime>
#include <QColor>
#include <QPixmap>
#include <QPainter>
#include <QImage>
#include <QQuickImageProvider>
#include "global.h"
#include "simplexnoisegenerator.h"

class renderProvider: public QQuickImageProvider
{
public:
    renderProvider();
    QPixmap requestPixmap(const QString &id, QSize *size, const QSize &requestedSize) override;

private:
    int calc = 0;
    simplexNoiseGenerator *noiseGen;
};

#endif // RENDERPROVIDER_H
