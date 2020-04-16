#include "configurationpanel.h"

ConfigurationPanel::ConfigurationPanel():
    FileName(QString()),
    isInitial(false)
{

}

ConfigurationPanel::~ConfigurationPanel()
{

}

void ConfigurationPanel::SetXmlFile(QString fileName)
{
    this->FileName = fileName;
}


void ConfigurationPanel::LoadFileData()
{
    isInitial = true;
    QFile file(FileName);
    if (!file.open(QFile::ReadOnly | QFile::Text))
    {
        qDebug()<<"Cannot read file "<<FileName<<"; "<<file.errorString();
        return;
    }
    QXmlStreamReader xml(&file);
    while (!xml.atEnd() && !xml.hasError())
    {
        QXmlStreamReader::TokenType token = xml.readNext();
        if (token == QXmlStreamReader::StartDocument)
            continue;
        if (token == QXmlStreamReader::StartElement)
        {
            if (xml.name() == XML_ELEMENT_CONFIGURE_PANEL)
                continue;
            if (xml.name() == XML_ELEMENT_VARIABLE)
            {
                QXmlStreamAttributes attributes = xml.attributes();
                if (attributes.hasAttribute(XML_ATTRIBUTE_NAME) && attributes.hasAttribute(XML_ATTRIBUTE_VALUE))
                {
                    if (attributes.value(XML_ATTRIBUTE_NAME).toString() == NAME_HORIZ_SCAN_RATE)
                    {
                        this->m_Data.HorizScanRate = attributes.value(XML_ATTRIBUTE_VALUE).toDouble();
                    }

                    if (attributes.value(XML_ATTRIBUTE_NAME).toString() == NAME_VERT_SCAN_RATE)
                        this->m_Data.VertScanRate = attributes.value(XML_ATTRIBUTE_VALUE).toDouble();

                    if (attributes.value(XML_ATTRIBUTE_NAME).toString() == NAME_SAMPLES_PER_LINE)
                        this->m_Data.SamplesPerLine = attributes.value(XML_ATTRIBUTE_VALUE).toDouble();

                    if (attributes.value(XML_ATTRIBUTE_NAME).toString() == NAME_DATA_RATE)
                        this->m_Data.DataRate = attributes.value(XML_ATTRIBUTE_VALUE).toDouble();

                    if (attributes.value(XML_ATTRIBUTE_NAME).toString() == NAME_SAMPLE_RATE)
                        this->m_Data.SampleRate = attributes.value(XML_ATTRIBUTE_VALUE).toDouble();

                    if (attributes.value(XML_ATTRIBUTE_NAME).toString() == NAME_USRP_CENTER_FREQ)
                        this->m_Data.USRPCenterFreq = attributes.value(XML_ATTRIBUTE_VALUE).toDouble();

                    if (attributes.value(XML_ATTRIBUTE_NAME).toString() == NAME_USRP_GAIN)
                        this->m_Data.USRPGain = attributes.value(XML_ATTRIBUTE_VALUE).toDouble();

                    if (attributes.value(XML_ATTRIBUTE_NAME).toString() == NAME_SAMPLES_PER_BIT)
                        this->m_Data.SamplesPerBit = attributes.value(XML_ATTRIBUTE_VALUE).toDouble();

                    if (attributes.value(XML_ATTRIBUTE_NAME).toString() == NAME_INIT_FETCH_FACTOR)
                        this->m_Data.InitFetchFactor = attributes.value(XML_ATTRIBUTE_VALUE).toDouble();

                    if (attributes.value(XML_ATTRIBUTE_NAME).toString() == NAME_TIME_PER_LINE)
                        this->m_Data.TimePerLine = attributes.value(XML_ATTRIBUTE_VALUE).toDouble();

                    if (attributes.value(XML_ATTRIBUTE_NAME).toString() == NAME_TIME_PER_FRAME)
                        this->m_Data.TimePerFrame = attributes.value(XML_ATTRIBUTE_VALUE).toDouble();

                    if (attributes.value(XML_ATTRIBUTE_NAME).toString() == NAME_LINES_PER_FRAME)
                        this->m_Data.LinesPerFrame = attributes.value(XML_ATTRIBUTE_VALUE).toDouble();

                    bool ok;
                    if (attributes.value(XML_ATTRIBUTE_NAME).toString() == NAME_SOF_CODE)
                        this->m_Data.SoFCode = attributes.value(XML_ATTRIBUTE_VALUE).toULong(&ok, 16);

                    if (attributes.value(XML_ATTRIBUTE_NAME).toString() == NAME_COERCED_RATE)
                        this->m_Data.CoercedRate = attributes.value(XML_ATTRIBUTE_VALUE).toDouble();

                    if (attributes.value(XML_ATTRIBUTE_NAME).toString() == NAME_NUMBER_CODE_SAMPLES)
                        this->m_Data.NumberCodeSamples = attributes.value(XML_ATTRIBUTE_VALUE).toDouble();

                    if (attributes.value(XML_ATTRIBUTE_NAME).toString() == NAME_NUMBER_FRONT_PORCH_SAMPLES)
                        this->m_Data.NumberFrontPorchSamples = attributes.value(XML_ATTRIBUTE_VALUE).toDouble();

                    if (attributes.value(XML_ATTRIBUTE_NAME).toString() == NAME_NUMBER_BACK_PORCH_SAMPLES)
                        this->m_Data.NumberBackPorchSamples = attributes.value(XML_ATTRIBUTE_VALUE).toDouble();

                    if (attributes.value(XML_ATTRIBUTE_NAME).toString() == NAME_SAMPLE_PER_FRAME)
                        this->m_Data.SamplePerFrame = attributes.value(XML_ATTRIBUTE_VALUE).toDouble();

                    if (attributes.value(XML_ATTRIBUTE_NAME).toString() == NAME_INIT_FETCH_SIZE)
                        this->m_Data.InitFetchSize = attributes.value(XML_ATTRIBUTE_VALUE).toDouble();

                    this->m_Data.HostAddresses.clear();
                    if (attributes.value(XML_ATTRIBUTE_NAME).toString() == NAME_DEVICE_IPS)
                    {
                        QString Addres = attributes.value(XML_ATTRIBUTE_VALUE).toString();
                        QStringList AddrList = Addres.split(HOST_DELEMITER);
                        for(auto addrItr : AddrList)
                        {
                            Q_ASSERT(!QHostAddress(addrItr).isNull());
                            this->m_Data.HostAddresses.append(QHostAddress(addrItr));
                        }
                    }
                }
                xml.readNext();
            }
        }
    }
    file.close();
}

void ConfigurationPanel::SaveToFile()
{
    QFile file(this->FileName);
    if (!file.open(QFile::WriteOnly | QFile::Text))
    {
        qDebug()<<"Cannot write file "<<file.errorString();
        exit(0);
    }
    QXmlStreamWriter xml(&file);
    xml.setAutoFormatting(true);
    xml.setAutoFormattingIndent(-1);
    {        
        xml.writeStartDocument();
        {
            xml.writeStartElement(XML_ELEMENT_CONFIGURE_PANEL);
            {
                WriteVariable(NAME_HORIZ_SCAN_RATE, this->m_Data.HorizScanRate, xml);
                WriteVariable(NAME_VERT_SCAN_RATE, this->m_Data.VertScanRate, xml);
                WriteVariable(NAME_SAMPLES_PER_LINE, this->m_Data.SamplesPerLine, xml);
                WriteVariable(NAME_DATA_RATE, this->m_Data.DataRate, xml);
                WriteVariable(NAME_SAMPLE_RATE, this->m_Data.SampleRate, xml);
                WriteVariable(NAME_USRP_CENTER_FREQ, this->m_Data.USRPCenterFreq, xml);
                WriteVariable(NAME_USRP_GAIN, this->m_Data.USRPGain, xml);
                WriteVariable(NAME_SAMPLES_PER_BIT, this->m_Data.SamplesPerBit, xml);
                WriteVariable(NAME_INIT_FETCH_FACTOR, this->m_Data.InitFetchFactor, xml);
                WriteVariable(NAME_TIME_PER_LINE, this->m_Data.TimePerLine, xml);
                WriteVariable(NAME_TIME_PER_FRAME, this->m_Data.TimePerFrame, xml);
                WriteVariable(NAME_LINES_PER_FRAME, this->m_Data.LinesPerFrame, xml);
                WriteVariable(NAME_SOF_CODE, this->m_Data.SoFCode, xml);
                WriteVariable(NAME_COERCED_RATE, this->m_Data.CoercedRate, xml);
                WriteVariable(NAME_NUMBER_CODE_SAMPLES, this->m_Data.NumberCodeSamples, xml);
                WriteVariable(NAME_NUMBER_FRONT_PORCH_SAMPLES, this->m_Data.NumberFrontPorchSamples, xml);
                WriteVariable(NAME_NUMBER_BACK_PORCH_SAMPLES, this->m_Data.NumberBackPorchSamples, xml);
                WriteVariable(NAME_SAMPLE_PER_FRAME, this->m_Data.SamplePerFrame, xml);
                WriteVariable(NAME_INIT_FETCH_SIZE, this->m_Data.InitFetchSize, xml);
                WriteVariable(NAME_DEVICE_IPS, this->m_Data.HostAddresses, xml);
            }
            xml.writeEndElement();
        }
        xml.writeEndDocument();
    }
    file.close();
}

void ConfigurationPanel::SetData(const ConfigurationPanelData &value)
{
    m_Data = value;
}

void ConfigurationPanel::WriteVariable(QString Name, QString Value, QXmlStreamWriter &xml)
{
    xml.writeStartElement(XML_ELEMENT_VARIABLE);
    {
        xml.writeAttribute(XML_ATTRIBUTE_NAME, Name);
        xml.writeAttribute(XML_ATTRIBUTE_VALUE, Value);
    }
    xml.writeEndElement();
}

void ConfigurationPanel::WriteVariable(QString Name, double Value, QXmlStreamWriter &xml)
{
    QString valueStr = QString::number(Value, 'f');
    WriteVariable(Name, valueStr, xml);
}

void ConfigurationPanel::WriteVariable(QString Name, ulong Value, QXmlStreamWriter &xml)
{
    QString valueStr = QString::number(Value,16);
    WriteVariable(Name, valueStr, xml);
}

void ConfigurationPanel::WriteVariable(QString Name, QList<QHostAddress> Values, QXmlStreamWriter &xml)
{
    QStringList hostList;
    for(auto hostItr : Values)
        hostList<<hostItr.toString();
    QString hostStr = hostList.join(HOST_DELEMITER);
    WriteVariable(NAME_DEVICE_IPS, hostStr, xml);
}
