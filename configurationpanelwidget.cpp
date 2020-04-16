#include "configurationpanelwidget.h"
#include "ui_configurationpanelwidget.h"

ConfigurationPanelWidget::ConfigurationPanelWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ConfigurationPanelWidget)
{
    ui->setupUi(this);
    connect (this, &ConfigurationPanelWidget::DataChanged, this, &ConfigurationPanelWidget::CalculateConfigData);
    this->setWindowFlag(Qt::Dialog);
}

ConfigurationPanelWidget::~ConfigurationPanelWidget()
{
    delete ui;
}

void ConfigurationPanelWidget::SetData(const ConfigurationPanel::ConfigurationPanelData &data)
{
    m_data = data;
    LoadData();
}

void ConfigurationPanelWidget::LoadData()
{
    this->blockSignals(true);
    ui->horizScanRateLineEdit->setText(QString::number(m_data.HorizScanRate));
    ui->vertScanRateLineEdit->setText(QString::number(m_data.VertScanRate));
    ui->samplesPerLineLineEdit->setText(QString::number(m_data.SamplesPerLine));
    ui->dataRateLineEdit->setText(QString::number(m_data.DataRate));

    ui->sampleRateLineEdit->setText(QString::number(m_data.SampleRate));
    ui->USRPCenterFreqLineEdit->setText(QString::number(m_data.USRPCenterFreq));
    ui->USRPGainLineEdit->setText(QString::number(m_data.USRPGain));
    ui->samplesPerBitLineEdit->setText(QString::number(m_data.SamplesPerBit));

    ui->initFetchFactorLineEdit->setText(QString::number(m_data.InitFetchFactor));
    ui->timePerLineLineEdit->setText(QString::number(m_data.TimePerLine));
    ui->timePerFrameLineEdit->setText(QString::number(m_data.TimePerFrame));
    ui->linesPerFrameLineEdit->setText(QString::number(m_data.LinesPerFrame));

    ui->SoFCodeLineEdit->setText(QString::number(m_data.SoFCode, 16));
    ui->coercedRateLineEdit->setText(QString::number(m_data.CoercedRate));
    ui->codeSamplesNumberLineEdit->setText(QString::number(m_data.NumberCodeSamples));
    ui->frontPorchSamplesNumberLineEdit->setText(QString::number(m_data.NumberFrontPorchSamples));

    ui->backPorchSamplesNumberLineEdit->setText(QString::number(m_data.NumberBackPorchSamples));
    ui->samplePerFrameLineEdit->setText(QString::number(m_data.SamplePerFrame));
    ui->initFetchSizeLineEdit->setText(QString::number(m_data.InitFetchSize));
    QList<QHostAddress> hostList =  m_data.HostAddresses;
    QStringList hostStrList;
    for(auto hostItr : hostList)
        hostStrList<<hostItr.toString();
    ui->devicesIPsLineEdit->setText(hostStrList.join(HOST_DELEMITER));
    ui->saveBtn->setEnabled(false);
    this->blockSignals(false);
}

ConfigurationPanel::ConfigurationPanelData ConfigurationPanelWidget::GetData()
{
    ConfigurationPanel::ConfigurationPanelData currData;
    currData.HorizScanRate = ui->horizScanRateLineEdit->text().toDouble();
    currData.VertScanRate = ui->vertScanRateLineEdit->text().toDouble();
    currData.SamplesPerLine = ui->samplesPerLineLineEdit->text().toDouble();
    currData.DataRate = ui->dataRateLineEdit->text().toDouble();

    currData.SampleRate = ui->sampleRateLineEdit->text().toDouble();
    currData.USRPCenterFreq = ui->USRPCenterFreqLineEdit->text().toDouble();
    currData.USRPGain = ui->USRPGainLineEdit->text().toDouble();
    currData.SamplesPerBit = ui->samplesPerBitLineEdit->text().toDouble();

    currData.InitFetchFactor = ui->initFetchFactorLineEdit->text().toDouble();
    currData.TimePerLine = ui->timePerLineLineEdit->text().toDouble();
    currData.TimePerFrame = ui->timePerFrameLineEdit->text().toDouble();
    currData.LinesPerFrame = ui->linesPerFrameLineEdit->text().toDouble();

    bool ok;
    currData.SoFCode = ui->SoFCodeLineEdit->text().toULong(&ok, 16);

    currData.CoercedRate = ui->coercedRateLineEdit->text().toDouble();
    currData.NumberCodeSamples = ui->codeSamplesNumberLineEdit->text().toDouble();
    currData.NumberFrontPorchSamples = ui->frontPorchSamplesNumberLineEdit->text().toDouble();

    currData.NumberBackPorchSamples = ui->backPorchSamplesNumberLineEdit->text().toDouble();
    currData.SamplePerFrame = ui->samplePerFrameLineEdit->text().toDouble();
    currData.InitFetchSize = ui->initFetchSizeLineEdit->text().toDouble();
    QStringList hostStrList = ui->devicesIPsLineEdit->text().split(HOST_DELEMITER);
    currData.HostAddresses.clear();
    for(auto hostItr : hostStrList)
    {
        qDebug()<<"Host: "<<hostItr;
        Q_ASSERT(!QHostAddress(hostItr).isNull());
        currData.HostAddresses<<QHostAddress(hostItr);
    }
    return currData;

}

QList<QHostAddress> ConfigurationPanelWidget::GetHost(QString host, char delemiter)
{
    QList<QHostAddress> addrs = QList<QHostAddress>();
    QStringList hostList = host.split(delemiter);
    for(auto host : hostList)
    {
        QHostAddress currHost = QHostAddress(host);
        Q_ASSERT(!currHost.isNull());
        addrs.append(currHost);
    }
    return addrs;

}

void ConfigurationPanelWidget::on_saveBtn_clicked()
{
    //Check if current data is valid
    bool conversionOk = false;
    int value = ui->SoFCodeLineEdit->text().toULong(&conversionOk, 16);
    if (conversionOk)
    {
        ui->saveBtn->setEnabled(false);
        emit SaveConfigurationPanel();
    }
    else
    {
        QMessageBox::warning(this, "Invalid Data", "SoF should be in Heximal Format.");
        return;
    }



}

void ConfigurationPanelWidget::on_cancelBtn_clicked()
{
    LoadData();
    this->close();
}

void ConfigurationPanelWidget::on_horizScanRateLineEdit_textChanged(const QString &arg1)
{
    Q_UNUSED(arg1);
    emit DataChanged();
}

void ConfigurationPanelWidget::on_vertScanRateLineEdit_textChanged(const QString &arg1)
{
    Q_UNUSED(arg1);
    emit DataChanged();
}

void ConfigurationPanelWidget::on_samplesPerLineLineEdit_textChanged(const QString &arg1)
{
    Q_UNUSED(arg1);
    emit DataChanged();
}

void ConfigurationPanelWidget::on_dataRateLineEdit_textChanged(const QString &arg1)
{
    Q_UNUSED(arg1);
    emit DataChanged();
}

void ConfigurationPanelWidget::CalculateConfigData()
{
    ConfigurationPanel::ConfigurationPanelData currData  = GetData();
    currData.TimePerLine = 1/currData.HorizScanRate;
    currData.TimePerFrame = 1/currData.VertScanRate;
    currData.LinesPerFrame = 189;
    currData.SamplesPerLine = (int) (currData.TimePerLine * currData.CoercedRate);
    currData.SamplePerFrame = currData.LinesPerFrame * currData.SamplesPerLine;
    currData.SamplesPerBit = currData.CoercedRate/currData.DataRate;
    currData.NumberCodeSamples = currData.SamplesPerBit * 16;
    currData.NumberBackPorchSamples = 60;
    currData.NumberFrontPorchSamples = 50;
    currData.InitFetchSize = currData.SamplesPerLine * currData.InitFetchFactor;
    SetData(currData);
    ui->saveBtn->setEnabled(true);

}

void ConfigurationPanelWidget::on_sampleRateLineEdit_textChanged(const QString &arg1)
{
    Q_UNUSED(arg1);
    emit DataChanged();
}

void ConfigurationPanelWidget::on_USRPCenterFreqLineEdit_textChanged(const QString &arg1)
{
    Q_UNUSED(arg1);
    emit DataChanged();
}

void ConfigurationPanelWidget::on_USRPGainLineEdit_textChanged(const QString &arg1)
{
    Q_UNUSED(arg1);
    emit DataChanged();
}

void ConfigurationPanelWidget::on_samplesPerBitLineEdit_textChanged(const QString &arg1)
{
    Q_UNUSED(arg1);
    emit DataChanged();
}

void ConfigurationPanelWidget::on_initFetchFactorLineEdit_textChanged(const QString &arg1)
{
    Q_UNUSED(arg1);
    emit DataChanged();
}

void ConfigurationPanelWidget::on_timePerLineLineEdit_textChanged(const QString &arg1)
{
    Q_UNUSED(arg1);
    emit DataChanged();
}

void ConfigurationPanelWidget::on_timePerFrameLineEdit_textChanged(const QString &arg1)
{
    Q_UNUSED(arg1);
    emit DataChanged();
}

void ConfigurationPanelWidget::on_linesPerFrameLineEdit_cursorPositionChanged(int arg1, int arg2)
{
    Q_UNUSED(arg1);
    emit DataChanged();
}

void ConfigurationPanelWidget::on_SoFCodeLineEdit_textChanged(const QString &arg1)
{
    Q_UNUSED(arg1);
    emit DataChanged();
}


void ConfigurationPanelWidget::on_coercedRateLineEdit_textChanged(const QString &arg1)
{
    Q_UNUSED(arg1);
    emit DataChanged();
}

void ConfigurationPanelWidget::on_codeSamplesNumberLineEdit_textChanged(const QString &arg1)
{
    Q_UNUSED(arg1);
    emit DataChanged();
}

void ConfigurationPanelWidget::on_frontPorchSamplesNumberLineEdit_textChanged(const QString &arg1)
{
    Q_UNUSED(arg1);
    emit DataChanged();
}

void ConfigurationPanelWidget::on_backPorchSamplesNumberLineEdit_textChanged(const QString &arg1)
{
    Q_UNUSED(arg1);
    emit DataChanged();
}

void ConfigurationPanelWidget::on_samplePerFrameLineEdit_textChanged(const QString &arg1)
{
    Q_UNUSED(arg1);
    emit DataChanged();
}

void ConfigurationPanelWidget::on_initFetchSizeLineEdit_textChanged(const QString &arg1)
{
    Q_UNUSED(arg1);
    Q_UNUSED(arg1);
    emit DataChanged();
}

void ConfigurationPanelWidget::on_devicesIPsLineEdit_textChanged(const QString &arg1)
{
    Q_UNUSED(arg1);
    emit DataChanged();
}
