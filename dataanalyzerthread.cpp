#include "dataanalyzerthread.h"

DataAnalyzerThread::DataAnalyzerThread(QSize frameSizeIn, const QByteArray &SoF, QObject *parent):
    QThread(parent),
    m_magnitudeData(QVector<float>()),
    m_phaseData(QByteArray()),
    m_SoFMatcher(SoF),
    m_frameSize(frameSizeIn),
    m_maxDataSize(90000),
    m_maxVal(0),
    m_terminate(false),
    m_pause(false)
{

    qDebug()<<"Search str: "<<SoF;
}

DataAnalyzerThread::~DataAnalyzerThread()
{
    qDebug()<<"Destroying thread...";
    m_sync.lock();
    m_terminate = true;
    m_sync.unlock();
    m_pauseCond.wakeAll();

    quit();
    wait();
}

void DataAnalyzerThread::addData(std::complex<float> *data, size_t size)
{
    QVector<float> magnitudeData = QVector<float>();
    QByteArray phaseData = QByteArray();
    float maxVal = 0;
    for(qulonglong i = 0; i < size; i++)
    {
        magnitudeData.append(abs(data[i]));
        phaseData.append(arg(data[i]) > 0 ? '1' : '0');
        maxVal = qMax(maxVal, abs(data[i]));
    }
    m_sync.lock();
    m_magnitudeData.append(magnitudeData);
    m_phaseData.append(phaseData);
    m_maxVal = qMax(m_maxVal, maxVal);
    //Truncate data if overflow
    m_sync.unlock();
    emit addNewFrame();
}

void DataAnalyzerThread::pause()
{
    m_sync.lock();
    m_pause = true;
    m_sync.unlock();
}

void DataAnalyzerThread::resume()
{
    m_sync.lock();
    m_pause = false;
    m_sync.unlock();
    m_pauseCond.wakeAll();
}

void DataAnalyzerThread::terminate()
{
    m_sync.lock();
    m_terminate = true;
    m_sync.unlock();
    m_pauseCond.wakeAll();
}

void DataAnalyzerThread::generatePicture(const float &maxVal, const QVector<float> &dataIn)
{
    QImage image(QImage(m_frameSize, QImage::Format_RGB32));
    for(int row = 0; row < m_frameSize.height(); row++)
    {
        for (int col = 0; col < m_frameSize.width(); col++)
        {
            float data = dataIn.at(row*m_frameSize.width() + col);
            int rgbColor = (int)(data/maxVal * 255);
            QColor color(rgbColor, rgbColor, rgbColor, 255);
            image.setPixelColor(col, row, color);
        }
    }
    emit sendImage(image);
}


void DataAnalyzerThread::run()
{
    const int samplePerFrame = m_frameSize.width()*m_frameSize.height();
    QImage image;
    QByteArray phaseData;
    QVector<float> magnitudeData;
    int pos = 0;
    float max;

    forever
    {
        m_sync.lock();
        if (m_pause)
            m_pauseCond.wait(&m_sync);
        if (m_terminate)
        {
            qDebug()<<"Terminating........";
            m_sync.unlock();
            m_pauseCond.wakeAll();
            return;
        }

        //If data is still large, then remove the extra data
        if (m_magnitudeData.size() > m_maxDataSize)
        {
            qDebug()<<"Truncate data outside";
            m_magnitudeData = m_magnitudeData.mid(m_magnitudeData.size() - m_maxDataSize, m_maxDataSize);
            m_phaseData = m_phaseData.mid(m_magnitudeData.size() - m_maxDataSize, m_maxDataSize);
        }
        else if (m_magnitudeData.size() < samplePerFrame)
        {
            m_sync.unlock();
            continue;
        }

        //Check data size
        phaseData = m_phaseData;
        magnitudeData = m_magnitudeData;
        max = m_maxVal;
        m_sync.unlock();
        //find position start of frame

        if ((pos = m_SoFMatcher.indexIn(phaseData, 0)) != -1)
        {
            //Size is small, do nothing
            if (magnitudeData.size() - pos < samplePerFrame)
            {
            }
            else
            {
                QVector<float> buffer = magnitudeData.mid(pos, samplePerFrame);
                generatePicture(max, buffer);
                //truncate the data that is generated
                m_sync.lock();
                m_magnitudeData.remove(0, pos + samplePerFrame);
                m_phaseData.remove(0, pos + samplePerFrame);

                //If data is still large, then remove the extra data
                if (m_magnitudeData.size() > m_maxDataSize)
                {
                    qDebug()<<"Truncate data inside";
                    m_magnitudeData = m_magnitudeData.mid(m_magnitudeData.size() - m_maxDataSize, m_maxDataSize);
                    m_phaseData = m_phaseData.mid(m_magnitudeData.size() - m_maxDataSize, m_maxDataSize);
                }

                m_sync.unlock();

                emit foundStartOfFrame();
            }
        }
        else
        {
            //No sof found, so the whole data is junk
            //for data integrity, only parts of data is remove because m_magnitude data might be changed during the operation
            m_sync.lock();

            //Remove only magnitude data that is invalid
            m_magnitudeData.remove(0, magnitudeData.size());
            m_phaseData.remove(0, magnitudeData.size());
            m_sync.unlock();
        }
        msleep(10);
    }
}
