#include "intensitydrawer.h"

IntensityDrawer::IntensityDrawer():
    maxIntensity(0),
    minIntensity(0),
    width(0),
    height(0)
{
    QLinearGradient gradient(QPointF(50, -20), QPointF(80, 20));
    gradient.setColorAt(0.0, Qt::white);
    gradient.setColorAt(1.0, QColor(0xa6, 0xce, 0x39));

    background = QBrush(QColor(64, 32, 64));
    circleBrush = QBrush(gradient);
    circlePen = QPen(Qt::black);
    circlePen.setWidth(1);
    textPen = QPen(Qt::white);
    textFont.setPixelSize(50);
}

void IntensityDrawer::setWidth(int widthIn)
{
    width = widthIn;
}

void IntensityDrawer::setHeight(int heightIn)
{
    height = heightIn;
}

void IntensityDrawer::SetImage(const QImage &image)
{
    this->image = image;
}

void IntensityDrawer::paint(QPainter *painter, QPaintEvent *event)
{
    painter->fillRect(event->rect(), Qt::white);
    painter->drawImage(event->rect(), this->image);
}

