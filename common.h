#ifndef COMMON_H
#define COMMON_H

#include <QObject>
#include <QTimer>
#include "global.h"
#include "renderprovider.h"

class common : public QObject
{
    Q_OBJECT
public:
    common();
    void bindRenderer(renderProvider *provider);
    Q_INVOKABLE void mousePosChanged(int x, int y);
    Q_INVOKABLE void regenerateMap();

private:
    QTimer *_tim;
    renderProvider *_renderer;
private slots:
    void timerTick();
};

#endif // COMMON_H
