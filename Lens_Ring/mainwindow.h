#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QCloseEvent>
#include <QMessageBox>
#include <QDebug>
#include<QMetaType>

#include "Dmc1000.h"
#include "cmd.h"
#include "sensor.h"
#include "model.h"
#include "ic_capture.h"

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
//function define
    void closeEvent(QCloseEvent *);
    void ControlCard_Initialization();

private slots:
    void on_actionCMD_triggered();

    void on_actionMODEL_triggered();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
