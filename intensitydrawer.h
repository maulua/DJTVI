#ifndef INTENSITYDRAWER_H
#define INTENSITYDRAWER_H

#include "common.h"


class IntensityDrawer
{
public:
    IntensityDrawer();
    void setWidth(int widthIn);
    void setHeight(int heightIn);
    void SetImage(const QImage& image);

    void paint(QPainter *painter, QPaintEvent *event);

private:
    double maxIntensity;
    double minIntensity;
    int width;
    int height;
    QBrush background;
    QBrush circleBrush;
    QFont textFont;
    QPen circlePen;
    QPen textPen;
    QImage image;


};

#endif // INTENSITYDRAWER_H
