#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , pause(false)
    , stop(false)
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
    qDebug()<<"Start Model....";
    ui->actionPause->setEnabled(true);
    ui->actionStop->setEnabled(true);
    ui->actionStart->setEnabled(false);

    QHostAddress radioIp = configPanel.m_Data.HostAddresses.first();
    QHostAddress hostIp = configPanel.m_Data.HostAddresses.last();
    double freq = configPanel.m_Data.USRPCenterFreq;
    double dataRate = configPanel.m_Data.DataRate;
    double gain = configPanel.m_Data.USRPGain;

    data = new std::complex<float>[3000];
    if (pause)
        dataThread->resume();
    else
    {
        dataThread->start();
        QMetaObject::invokeMethod(this, "generateRandomData", Qt::QueuedConnection);
    }

    stop = false;
    pause = false;

}

void MainWindow::PauseModel()
{
    qDebug()<<"Pause Model...";
    ui->actionStart->setEnabled(true);
    ui->actionPause->setEnabled(false);
    ui->actionStop->setEnabled(true);
    pause = true;
}

void MainWindow::StopModel()
{
    qDebug()<<"Stop Model...";
    ui->actionStart->setEnabled(true);
    ui->actionPause->setEnabled(false);
    ui->actionStop->setEnabled(false);
    stop = true;
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
    stop = true;
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
            dataThread->stop();
            return;
        }
        if (pause)
        {
            dataThread->pause();
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
