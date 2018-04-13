#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QCloseEvent>
#include <QMessageBox>
#include <QDebug>
#include <QMetaType>
#include <QList>
#include <QFileDialog>
#include <QSettings>
#include<QMetaType>

#include "Dmc1000.h"
#include "HalconCpp.h"
#include "cmd.h"
#include "pmd.h"
#include "running.h"
#include "model.h"
#include "ic_capture.h"

using namespace HalconCpp;
using namespace std;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
protected:
    void closeEvent(QCloseEvent *);
//Halocn variable define
    HTuple hv_WindowHandle1, hv_WindowHandle2;
//pointer define
    QApplication *app;
    IC_Capture *ic_cap;
    Running *run;
    QSettings *configFile;
    QSettings *configFile1;
//variable define
    bool board_initialization=true;
    bool First_Start=true;
    bool First_OpenWindow1=true;
    bool First_OpenWindow2=true;
    int present_part=0;
//function define
    QString load_model();
    void disp_path();
    void disp_present_path(int);
    void ControlCard_Initialization();
    void ALL_Origin_Back();

signals:
    void signal_action_enable(bool);
    void signal_part_select(int);
    void reload_parameters(int);//it's function is same to (signal_part_select)
    void signal_read_model(int);
    void signal_reset();

public slots:
    void slot_open_Camera(bool);
    void slot_disp_image1(HObject);
    void slot_disp_image2(HObject);
    void slot_detection_image1(HObject);
    void slot_detection_image2(HObject);

    void lock_all_buttons(bool);

private slots:
    void on_actionCMD_triggered();

    void on_actionPMD_triggered();

    void on_actionMODEL_triggered();

    void on_START_clicked();

    void on_PAUSE_clicked();

    void on_RESUME_clicked();

    void on_STOP_clicked();

    void on_RESET_clicked();

    void on_load_model_1_clicked();

    void on_load_model_2_clicked();

    void on_load_model_3_clicked();

    void on_load_model_4_clicked();

    void on_load_model_5_clicked();

    void on_load_model_6_clicked();

    void on_load_model_7_clicked();

    void on_load_model_8_clicked();

    void on_load_model_9_clicked();

    void on_load_model_10_clicked();

    void on_actionPART1_triggered();

    void on_actionPART2_triggered();

    void on_actionPART3_triggered();

    void on_actionPART4_triggered();

    void on_actionPART5_triggered();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
