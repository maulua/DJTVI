#ifndef INTENSITYWIDGET_H
#define INTENSITYWIDGET_H

#include "common.h"
#include "intensitydrawer.h"

class IntensityWidget : public QWidget
{
    Q_OBJECT
public:
    explicit IntensityWidget(IntensityDrawer* drawer, QWidget *parent = nullptr);
    void SetImage(const QImage& image);

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    IntensityDrawer *IDrawer;
    std::complex<float>* currData;

};

#endif // INTENSITYWIDGET_H
