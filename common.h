#ifndef COMMON_H
#define COMMON_H


#include <QtMath>
#include <QString>
#include <QHostAddress>
#include <QFile>
#include <QXmlStreamReader>
#include <QXmlStreamWriter>
#include <QList>
#include <QDebug>
#include <QWidget>
#include <QMessageBox>
#include <QRegularExpression>
#include <QRegularExpressionMatch>
#include <QPainter>
#include <QPaintEvent>
#include <complex>
#include <QtGlobal>
#include <QSettings>
#include <QThread>
#include <QByteArrayMatcher>
#include <QTimer>
#include <QPalette>
#include <QLabel>
#include <QWaitCondition>

#include <QMainWindow>
#include <QLayout>
#include <QLayout>
#include <QFileSystemWatcher>
#include <QGraphicsOpacityEffect>
#include <QPropertyAnimation>
#include <QImageReader>
#include <QFileDialog>
#include <QPixmap>
#include <iomanip>
#include <cmath>
#include <iostream>
#include <complex>
#include <QMutex>


#define PI 3.14159265359
#define INV2PI 0.15915494309

template<typename T>
T magnitude(T x, T y)
{
    return static_cast<T>(qSqrt(qPow(x, 2) + qPow(y, 2)));
}

template <typename T>
T arctan2(T x, T y)
{
    const T coeff_1 = PI / 4, coeff_2 = 3 * coeff_1; //For Atan2, Pi/4 and 3Pi/4
    T abs_y, angle, r;
    abs_y = fabs(y)+1e-10;      // kludge to prevent 0/0 condition
    if (x>=0)
    {
        r = (x - abs_y) / (x + abs_y);
        angle = coeff_1 - coeff_1 * r;
    }
    else
    {
        r = (x + abs_y) / (abs_y - x);
        angle = coeff_2 - coeff_1 * r;
    }
    if (y < 0)
        return(-angle);     // negate if in quad III or IV
    else
        return(angle);
}

template <typename T>
void Convolution(T* zOut, T* xIn, T* yIn, int xLenIn, int yLenIn)
{
    T* zPtr, s, *xp, *yp;
    int lenz;
    int i, n, n_lo, n_hi;
    lenz = xLenIn + yLenIn - 1;
    zPtr = zOut;

    for(int i = 0; i < lenz; i++)
    {
        s = 0;
        n_lo = 0 > (i - yLenIn + 1) ? 0 : i - yLenIn + 1;
        n_hi = xLenIn - 1 < i ? xLenIn - 1 : i;
        xp = xIn + n_lo;
        yp = yIn + i - n_lo;
        for(n = n_lo; n < n_hi; n++)
        {
            s+= *xp * *yp;
            xp++;
            yp--;
        }
        *zPtr = s;
        zPtr++;
    }

}
#endif // COMMON_H
