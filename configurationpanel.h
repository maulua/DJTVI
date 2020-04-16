#ifndef CONFIGURATIONPANEL_H
#define CONFIGURATIONPANEL_H

#include "common.h"


#define XML_ELEMENT_CONFIGURE_PANEL "ConfigurePanel"
#define XML_ELEMENT_VARIABLE "Variable"
#define XML_ATTRIBUTE_NAME "Name"
#define XML_ATTRIBUTE_VALUE "Value"
#define NAME_HORIZ_SCAN_RATE "HorizScanRate"
#define NAME_VERT_SCAN_RATE "VertScanRate"
#define NAME_SAMPLES_PER_LINE "SamplesPerLine"
#define NAME_DATA_RATE "DataRate"
#define NAME_SAMPLE_RATE "SampleRate"
#define NAME_USRP_CENTER_FREQ "USRPCenterFrequency"
#define NAME_USRP_GAIN "USRPGain"
#define NAME_SAMPLES_PER_BIT "SamplesPerBit"
#define NAME_INIT_FETCH_FACTOR "InitFetchFactor"
#define NAME_TIME_PER_LINE "TimePerLine"
#define NAME_TIME_PER_FRAME "TimePerFrame"
#define NAME_LINES_PER_FRAME "LinePerFrame"
#define NAME_SOF_CODE "SoFCode"
#define NAME_COERCED_RATE "CoercedRate"
#define NAME_NUMBER_CODE_SAMPLES "NumberCodeSamples"
#define NAME_NUMBER_FRONT_PORCH_SAMPLES "NumberFrontPorchSamples"
#define NAME_NUMBER_BACK_PORCH_SAMPLES "NumberBackPorchSamples"
#define NAME_SAMPLE_PER_FRAME "SamplePerFrame"
#define NAME_INIT_FETCH_SIZE "InitFetchSize"
#define NAME_DEVICE_IPS "DeviceIPs"

#define HOST_DELEMITER ';'





class ConfigurationPanel
{
public:
    ConfigurationPanel();
    ~ConfigurationPanel();

    struct ConfigurationPanelData
    {
        double HorizScanRate;
        double VertScanRate;
        double SamplesPerLine;
        double DataRate;
        double SampleRate;
        double USRPCenterFreq;
        double USRPGain;
        double SamplesPerBit;
        double InitFetchFactor;
        double TimePerLine;
        double TimePerFrame;
        double LinesPerFrame;
        ulong SoFCode;
        double CoercedRate;
        double NumberCodeSamples;
        double NumberFrontPorchSamples;
        double NumberBackPorchSamples;
        double SamplePerFrame;
        double InitFetchSize;
        QList<QHostAddress> HostAddresses;
    };


    //Set XML file that associate with this model
    void SetXmlFile(QString fileName);

    //Set curent data
    void SetData(const ConfigurationPanelData& _dataIn);

    //Load XML file and update current data
    void LoadFileData();

    //Write current data to current XML file
    void SaveToFile();

public:
    ConfigurationPanelData m_Data;
    bool isInitial;


private:
    QString FileName;
    void WriteVariable(QString Name, QString Value, QXmlStreamWriter &xml);
    void WriteVariable(QString Name, double Value, QXmlStreamWriter &xml);
    void WriteVariable(QString Name, ulong Value, QXmlStreamWriter &xml);
    void WriteVariable(QString Name, QList<QHostAddress> Values, QXmlStreamWriter &xml);

};

#endif // CONFIGURATIONPANEL_H
