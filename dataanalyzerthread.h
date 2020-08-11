#ifndef DATAANALYZERTHREAD_H
#define DATAANALYZERTHREAD_H


#include "common.h"

class DataAnalyzerThread : public QThread
{
    Q_OBJECT
public:

    //Samples per bit is 160
    //CE1A917D or 1100 1110 0001 1010 1001 0001 0111 1101
    //
    DataAnalyzerThread(QSize frameSizeIn, const QByteArray& SoF, QObject *parent = nullptr);
    ~DataAnalyzerThread();

    void addData(std::complex<float>* data, size_t size);

    void pause();
    void resume();
    void stop();
    void complete();
    void generatePicture(const float& maxVal, const QVector<float>& dataIn);

signals:
    void sendImage(QImage& image);
    void foundStartOfFrame();
    void addNewFrame();

protected:
    void run() override;
    
private:
    QVector<float> m_magnitudeData;
    QByteArray m_phaseData;
    QMutex m_sync;
    QByteArrayMatcher m_SoFMatcher;
    QSize m_frameSize;
    const int m_maxDataSize;
    float m_maxVal;

    QWaitCondition m_pauseCond;
    bool m_pause;
    bool m_stop;
    bool m_complete;





    //QByteArray A(QByteArray(1, 160));
};

#endif // DATAANALYZERTHREAD_H
