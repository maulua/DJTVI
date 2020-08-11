#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , pause(false)
    , stop(true)
    , terminate(false)
{
    ui->setupUi(this);
    qRegisterMetaType<ConfigurationPanel::ConfigurationPanelData>("ConfigurationPanelData");
    qRegisterMetaType<QImage>("QImage&");

    InitData();
    InitGui();
    dataThread = new DataAnalyzerThread(QSize(214, 189), QByteArray(20, '1'), this);


    connect(dataThread, &DataAnalyzerThread::sendImage, IWidget, &IntensityWidget::SetImage);
    connect(dataThread, &DataAnalyzerThread::foundStartOfFrame, this, &MainWindow::StartOfFrameFound);
    connect(dataThread, &DataAnalyzerThread::addNewFrame, this, &MainWindow::AddNewFrame);
    connect(dataThread, &DataAnalyzerThread::finished, this, &MainWindow::CompleteDataAnalyzer);
}

MainWindow::~MainWindow()
{
    qDebug()<<"Destroying window...";
    stop = true;
    deleteLater();
    delete ui;

}

void MainWindow::showConfigPanelWidget()
{
    configPanelWidget->show();
}

void MainWindow::SaveConfigurationPanelWidgetData()
{
    Q_ASSERT(configPanelWidget != nullptr);

    //Update data from configPanelWidget
    configPanel.SetData(configPanelWidget->GetData());

    //Save current data to file
    configPanel.SaveToFile();

    //Update configPanelWidget data, and load new data
    configPanelWidget->SetData(configPanel.m_Data);

    //Close current widget
    configPanelWidget->close();
    QMessageBox::information(this, "Success!", "Success save current data to xml file....");


}

void MainWindow::UpdateConfigPanel()
{
    qDebug()<<"File changed...";
    if (!configPanel.isInitial)
        return;
    configPanel.LoadFileData();
    if (configPanelWidget != nullptr)
    {
        configPanelWidget->SetData(configPanel.m_Data);
    }
}

void MainWindow::StartModel()
{
    //If current thread is pause, then continue the thread
    if (pause)
    {
        ResumeModel();
    }
    else
        StartNewModel();
    pause = false;
    stop = false;
    ui->actionPause->setEnabled(true);
    ui->actionStop->setEnabled(true);
    ui->actionStart->setEnabled(false);
}

void MainWindow::PauseModel()
{
    qDebug()<<"Pause Model...";
    ui->actionStart->setEnabled(true);
    ui->actionPause->setEnabled(false);
    ui->actionStop->setEnabled(true);
    pause = true;
    stop =false;
    dataThread->pause();

}

void MainWindow::StopModel()
{
    qDebug()<<"Stop Model...";
    ui->actionStart->setEnabled(true);
    ui->actionPause->setEnabled(false);
    ui->actionStop->setEnabled(false);
    stop = true;
    pause = false;
    dataThread->stop();

}

void MainWindow::StartOfFrameFound()
{
    QLabel *label = ui->sofFoundIndicatorLabel;
    label->setStyleSheet("QLabel{background-color: blue; color: blue;}");
}

void MainWindow::AddNewFrame()
{
    QLabel *label = ui->sofFoundIndicatorLabel;
    label->setStyleSheet("QLabel{background-color: yellow; color: yellow;}");
}

void MainWindow::CompleteDataAnalyzer()
{
    qDebug()<<"Complete data analyzer thread....";
    stop = false;
    pause =false;
    ui->actionStart->setEnabled(true);
    ui->actionPause->setEnabled(false);
    ui->actionStop->setEnabled(false);
}



void MainWindow::InitGui()
{
    this->setWindowTitle("DJTVI");

    connect(ui->actionConfigure_Panel, &QAction::triggered, this, &MainWindow::showConfigPanelWidget);
    connect(ui->actionClose, &QAction::triggered, this, &QMainWindow::close);
    connect(ui->actionStart, &QAction::triggered, this, &MainWindow::StartModel);
    connect(ui->actionPause, &QAction::triggered, this, &MainWindow::PauseModel);
    connect(ui->actionStop, &QAction::triggered, this, &MainWindow::StopModel);
    connect(ui->actionXCorr, &QAction::triggered, crossCorrWidget, &QWidget::show);
    connect(ui->actionLoad, &QAction::triggered, this, &MainWindow::OpenImage);


    ui->actionPause->setEnabled(false);
    ui->actionStop->setEnabled(false);


    ui->runtimeConfigWidget->setMaximumWidth(0);
    ui->expandingConfigBtn->setText(ui->runtimeConfigWidget->maximumWidth() == 0 ? "<<" : ">>");

}

void MainWindow::InitData()
{
    InitConfigPanel();
    InitConfigPanelWidget();
    InitIntensityWidget();
    InitCrossCorrelationWidget();
}

void MainWindow::InitConfigPanel()
{
    QString iniFile = "Config.ini";
    QFile configFile(iniFile);
    if (!configFile.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QSettings settings(QSettings::IniFormat, QSettings::UserScope, "IC6");
        if (!settings.contains("configFile"))
        {
            iniFile = QFileDialog::getOpenFileName(this, "Configuration", ".", "ini (*.ini)");
            if (iniFile.isEmpty())
                return;
            settings.setValue("configFile", iniFile);
            qDebug()<<"Save config file to user's setting "<<iniFile;
            InitConfigPanel();
            return;
        }
        iniFile = settings.value("configFile").toString();
    }
    QSettings settings(iniFile, QSettings::Format::IniFormat);
    QString configPanelFileName = settings.value("configPanelFile").toString();
    configPanel.SetXmlFile(configPanelFileName);
    configPanel.LoadFileData();

    QFileSystemWatcher * watcher = new QFileSystemWatcher( this);
    watcher->addPath(configPanelFileName);
    connect(watcher, &QFileSystemWatcher::fileChanged, this, &MainWindow::UpdateConfigPanel);
}

void MainWindow::InitConfigPanelWidget()
{
    configPanelWidget = new ConfigurationPanelWidget(this);
    configPanelWidget->SetData(configPanel.m_Data);
    configPanelWidget->setWindowTitle("Configuration");
    connect (configPanelWidget, &ConfigurationPanelWidget::SaveConfigurationPanel, this, &MainWindow::SaveConfigurationPanelWidgetData);

}

void MainWindow::InitIntensityWidget()
{

    IWidget = new IntensityWidget(&drawer, this);
    QVBoxLayout *widgetLayout = new QVBoxLayout();
    widgetLayout->addWidget(IWidget);
    ui->intensityWidget->setLayout(widgetLayout);
}

void MainWindow::InitCrossCorrelationWidget()
{
    crossCorrWidget = new CrossCorrelationWidget();
    crossCorrWidget->setWindowTitle("Cross-Correlation");
}

void MainWindow::OpenImage()
{
    QStringList formats;
    static QString path;
    foreach (QByteArray format, QImageReader::supportedImageFormats())
        if (format.toLower() == format)
            formats.append("*." + format);

    QString newPath = QFileDialog::getOpenFileName(this, tr("Open Image"),
        path, tr("Image files (%1)").arg(formats.join(' ')));

    if (newPath.isEmpty())
        return;

    QImage image(newPath);

    //Cut into pixmap



    if (!image.isNull()) {
        LoadImage(image);
        path = newPath;
    }
}

void MainWindow::LoadImage(QImage &image)
{
//    IWidget->SetImage(image);
//    if (ui->actionStart->isEnabled())
//    {
//        IWidget->setUpdatesEnabled(false);
//    }
//    else
    //        IWidget->setUpdatesEnabled(true);
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    qDebug()<<"Terminate event.......................********************";
    StopModel();
}

void MainWindow::ResumeModel()
{
    dataThread->resume();
}

void MainWindow::StartNewModel()
{
    testFunction();
    return;
//    qDebug()<<"Start Model....";

//    QHostAddress radioIp = configPanel.m_Data.HostAddresses.first();
//    QHostAddress hostIp = configPanel.m_Data.HostAddresses.last();
//    double freq = configPanel.m_Data.USRPCenterFreq;
//    double dataRate = configPanel.m_Data.DataRate;
//    double gain = configPanel.m_Data.USRPGain;
//    double bandwidth = 1e6;


//    data = new std::complex<float>[3000];
//    dataThread->start();

//    std::string device_args("addr=192.168.10.2");

//    uhd::set_thread_priority_safe();

//    //Make USRP
//    uhd::usrp::multi_usrp::sptr usrp = uhd::usrp::multi_usrp::make(device_args);

//    //Set Clock
//    usrp->set_clock_source("internal");

//    //SubDevice
//    std::string subdev("A:0");
//    qDebug()<<"Set sub device: "<<QString::fromStdString(subdev);
//    usrp->set_rx_subdev_spec(subdev);
//    qDebug()<<"Sub device: "<<QString::fromStdString(usrp->get_pp_string().c_str());

//    //Rx rate
//    if (dataRate <= 0)
//    {
//        qWarning()<<"Invalid data rate...";
//        return;
//    }
//    qDebug()<<"Setting rx rate";
//    usrp->set_rx_rate(dataRate);
//    qDebug()<<"Rx rate: "<<usrp->get_rx_freq();

//    //Rx frequency
//    qDebug()<<"Set rx freq: ";
//    usrp->set_tx_freq(uhd::tune_request_t(freq));
//    qDebug()<<"Freq: "<<usrp->get_rx_freq();

//    // set the rf gain
//    qDebug()<<"Set RF gain: ";
//    usrp->set_rx_gain(gain);
//    qDebug()<<"Rf gain: "<<usrp->get_rx_gain();

//    // set the IF filter bandwidth
//    qDebug()<<"Setting RX Bandwidth: "<< bandwidth;
//    usrp->set_rx_bandwidth(bandwidth);
//    qDebug()<<"Bandwidth: "<<usrp->get_rx_bandwidth();

//    // set the antenna
//    qDebug()<<"Set rx antena...";
//    std::string ant("TX/RX");
//    usrp->set_rx_antenna(ant);
//    qDebug()<<"RX antenna: "<<QString::fromStdString(usrp->get_rx_antenna());






    //QMetaObject::invokeMethod(this, "generateRandomData", Qt::QueuedConnection);
}

void MainWindow::testFunction()
{
    uhd::set_thread_priority_safe();

    std::string device_args("addr=192.168.10.2");
    std::string subdev("A:0");
    std::string ant("TX/RX");
    std::string ref("internal");

    double rate(1e6);
    double freq(915e6);
    double gain(10);
    double bw(1e6);

    //create a usrp device
    std::cout << std::endl;
    std::cout << boost::format("Creating the usrp device with: %s...") % device_args << std::endl;
//    uhd::usrp::multi_usrp::sptr usrp = uhd::usrp::multi_usrp::make(device_args);

//    // Lock mboard clocks
//    std::cout << boost::format("Lock mboard clocks: %f") % ref << std::endl;
//    usrp->set_clock_source(ref);

//    //always select the subdevice first, the channel mapping affects the other settings
//    std::cout << boost::format("subdev set to: %f") % subdev << std::endl;
//    usrp->set_rx_subdev_spec(subdev);
//    std::cout << boost::format("Using Device: %s") % usrp->get_pp_string() << std::endl;

//    //set the sample rate
//    if (rate <= 0.0) {
//        std::cerr << "Please specify a valid sample rate" << std::endl;
//        return;
//    }

//    // set sample rate
//    std::cout << boost::format("Setting RX Rate: %f Msps...") % (rate / 1e6) << std::endl;
//    usrp->set_rx_rate(rate);
//    std::cout << boost::format("Actual RX Rate: %f Msps...") % (usrp->get_rx_rate() / 1e6) << std::endl << std::endl;

//    // set freq
//    std::cout << boost::format("Setting RX Freq: %f MHz...") % (freq / 1e6) << std::endl;
//    uhd::tune_request_t tune_request(freq);
//    usrp->set_rx_freq(tune_request);
//    std::cout << boost::format("Actual RX Freq: %f MHz...") % (usrp->get_rx_freq() / 1e6) << std::endl << std::endl;

//    // set the rf gain
//    std::cout << boost::format("Setting RX Gain: %f dB...") % gain << std::endl;
//    usrp->set_rx_gain(gain);
//    std::cout << boost::format("Actual RX Gain: %f dB...") % usrp->get_rx_gain() << std::endl << std::endl;

//    // set the IF filter bandwidth
//    std::cout << boost::format("Setting RX Bandwidth: %f MHz...") % (bw / 1e6) << std::endl;
//    usrp->set_rx_bandwidth(bw);
//    std::cout << boost::format("Actual RX Bandwidth: %f MHz...") % (usrp->get_rx_bandwidth() / 1e6) << std::endl << std::endl;

//    // set the antenna
//    std::cout << boost::format("Setting RX Antenna: %s") % ant << std::endl;
//    usrp->set_rx_antenna(ant);
//    std::cout << boost::format("Actual RX Antenna: %s") % usrp->get_rx_antenna() << std::endl << std::endl;
}

void MainWindow::generateRandomData()
{
    static int i = 0;
    static int j = 0;

    if (j >= 1000)
    {
        i = 0;
        j = 0;
        ui->actionStart->setEnabled(true);
        ui->actionStop->setEnabled(false);
        ui->actionPause->setEnabled(false);
        dataThread->complete();

        if (!pause)
        {
            stop = true;
            pause = false;
        }
        return;
    }
    while (i < 3000)
    {
        if (i < 107)
            *(data + i) = std::complex<float>(1, 1);
        else
            *(data+i) = std::complex<float>(qrand(), qrand());
        if (stop)
        {
            i = 0;
            j = 0;
            return;
        }
        if (pause)
        {
            //return;
        }
        i++;

    }
    i = 0;
    j++;
    dataThread->addData(data, 3000);
    qDebug()<<"Sending sample "<<j;
    QMetaObject::invokeMethod(this, "generateRandomData", Qt::QueuedConnection);
}


void MainWindow::on_expandingConfigBtn_clicked()
{
    QPropertyAnimation *animation = new QPropertyAnimation(ui->runtimeConfigWidget, "maximumWidth");

    if (ui->runtimeConfigWidget->maximumWidth() != 0)
    {   //Hide
        animation->setDuration(500);
        animation->setStartValue(ui->runtimeConfigWidget->width());
        animation->setEndValue(0);

    }
    else
    {
        //Show
        animation->setDuration(1000);
        animation->setStartValue(0);
        animation->setEndValue(1000);
    }
    animation->start();
    ui->expandingConfigBtn->setText(ui->runtimeConfigWidget->maximumWidth() == 0 ? ">>" : "<<");
}
