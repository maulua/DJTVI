#ifndef CONFIGURATIONPANELWIDGET_H
#define CONFIGURATIONPANELWIDGET_H

#include "common.h"
#include "configurationpanel.h"




namespace Ui {
class ConfigurationPanelWidget;
}

class ConfigurationPanelWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ConfigurationPanelWidget(QWidget *parent = nullptr);
    ~ConfigurationPanelWidget();

    //Set Data for the Widget
    void SetData(const ConfigurationPanel::ConfigurationPanelData &data);



    //Get current data from GUI
    ConfigurationPanel::ConfigurationPanelData GetData();



private slots:
    void on_saveBtn_clicked();

    void on_cancelBtn_clicked();

    void on_horizScanRateLineEdit_textChanged(const QString &arg1);

    void on_vertScanRateLineEdit_textChanged(const QString &arg1);

    void on_samplesPerLineLineEdit_textChanged(const QString &arg1);

    void on_dataRateLineEdit_textChanged(const QString &arg1);

    void CalculateConfigData();

    void on_sampleRateLineEdit_textChanged(const QString &arg1);

    void on_USRPCenterFreqLineEdit_textChanged(const QString &arg1);

    void on_USRPGainLineEdit_textChanged(const QString &arg1);

    void on_samplesPerBitLineEdit_textChanged(const QString &arg1);

    void on_initFetchFactorLineEdit_textChanged(const QString &arg1);

    void on_timePerLineLineEdit_textChanged(const QString &arg1);

    void on_timePerFrameLineEdit_textChanged(const QString &arg1);

    void on_linesPerFrameLineEdit_cursorPositionChanged(int arg1, int arg2);

    void on_SoFCodeLineEdit_textChanged(const QString &arg1);


    void on_coercedRateLineEdit_textChanged(const QString &arg1);

    void on_codeSamplesNumberLineEdit_textChanged(const QString &arg1);

    void on_frontPorchSamplesNumberLineEdit_textChanged(const QString &arg1);

    void on_backPorchSamplesNumberLineEdit_textChanged(const QString &arg1);

    void on_samplePerFrameLineEdit_textChanged(const QString &arg1);

    void on_initFetchSizeLineEdit_textChanged(const QString &arg1);

    void on_devicesIPsLineEdit_textChanged(const QString &arg1);

signals:
    void SaveConfigurationPanel();
    void DataChanged();

private:
    QList<QHostAddress> GetHost(QString host, char delemiter);
    //Load current data to update GUI
    void LoadData();

private:
    Ui::ConfigurationPanelWidget *ui;
    ConfigurationPanel::ConfigurationPanelData m_data;
};

#endif // CONFIGURATIONPANELWIDGET_H
