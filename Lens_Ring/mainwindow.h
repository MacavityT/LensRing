#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QCloseEvent>
#include <QMessageBox>
#include <QDebug>
#include<QMetaType>

#include "Dmc1000.h"
#include "HalconCpp.h"
#include "cmd.h"
#include "sensor.h"
#include "model.h"
#include "ic_capture.h"

using namespace HalconCpp;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();    
//pointer define
    QApplication *app;
    IC_Capture *ic_cap;
    Sensor *sen;
//variable define
    bool board_initialization=true;
    bool First_Start=true;
    bool First_OpenWindow=true;
//function define
    void closeEvent(QCloseEvent *);
    void ControlCard_Initialization();
    void ALL_Origin_Back();


signals:
    void signal_action_enable(bool);

public slots:
    void slot_open_Camera(bool);
    void slot_disp_image(HObject);
    void slot_disp_image1(HObject);
    void slot_detection_image(HObject);
    void slot_detection_image1(HObject);

private slots:
    void on_actionCMD_triggered();

    void on_actionMODEL_triggered();

    void on_START_clicked();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
