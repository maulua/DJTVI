#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "common.h"
#include <uhd.h>


#include "intensitywidget.h"
#include "crosscorrelationwidget.h"
#include "dataanalyzerthread.h"



#include "configurationpanelwidget.h"


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE



class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();


private slots:
    void showConfigPanelWidget();
    void SaveConfigurationPanelWidgetData();
    void UpdateConfigPanel();
    void StartModel();
    void PauseModel();
    void StopModel();
    void StartOfFrameFound();
    void AddNewFrame();


    void on_expandingConfigBtn_clicked();

private:
    void InitGui();
    void InitData();
    void InitConfigPanel();
    void InitConfigPanelWidget();
    void InitIntensityWidget();
    void InitCrossCorrelationWidget();
    void OpenImage();
    void LoadImage(QImage& image);
    void closeEvent(QCloseEvent *event) override;



private:
    QMutex m_mutex;
    Ui::MainWindow *ui;
    ConfigurationPanel configPanel;
    ConfigurationPanelWidget* configPanelWidget;
    CrossCorrelationWidget* crossCorrWidget;
    IntensityWidget* IWidget;
    IntensityDrawer drawer;
    DataAnalyzerThread *dataThread;
    bool terminate;// = false;
    bool pause;//=false
    QWaitCondition m_pauseCond;

};
#endif // MAINWINDOW_H
