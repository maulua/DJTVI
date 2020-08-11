#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "common.h"
#include <uhd.h>
#include <uhd/utils/thread_priority.hpp>
#include <uhd/utils/safe_main.hpp>
#include <uhd/usrp/multi_usrp.hpp>
#include <uhd/exception.hpp>
#include <uhd/types/tune_request.hpp>
#include <boost/program_options.hpp>
#include <boost/format.hpp>
#include <boost/thread.hpp>


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

public slots:
    void generateRandomData();

private slots:
    void showConfigPanelWidget();
    void SaveConfigurationPanelWidgetData();
    void UpdateConfigPanel();
    void StartModel();
    void PauseModel();
    void StopModel();
    void StartOfFrameFound();
    void AddNewFrame();
    void CompleteDataAnalyzer();


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
    void ResumeModel();
    void StartNewModel();
    void testFunction();



private:
    Ui::MainWindow *ui;
    ConfigurationPanel configPanel;
    ConfigurationPanelWidget* configPanelWidget;
    CrossCorrelationWidget* crossCorrWidget;
    IntensityWidget* IWidget;
    IntensityDrawer drawer;
    DataAnalyzerThread *dataThread;
    bool stop;// = false;
    bool pause;//=false
    bool terminate; //= false
    std::complex<float>* data;

};
#endif // MAINWINDOW_H
