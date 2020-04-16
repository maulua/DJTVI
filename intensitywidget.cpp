#include "intensitywidget.h"

IntensityWidget::IntensityWidget(IntensityDrawer* drawer, QWidget *parent) :
    QWidget(parent),
    IDrawer(drawer)
{
}

void IntensityWidget::SetImage(const QImage &image)
{
    IDrawer->SetImage(image);
    update();
}

void IntensityWidget::paintEvent(QPaintEvent *event)
{
    QPainter painter;
    painter.begin(this);
    painter.setRenderHint(QPainter::Antialiasing);
    IDrawer->paint(&painter, event);
    painter.end();
}
