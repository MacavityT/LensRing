#ifndef CMD_H
#define CMD_H


#include <QDialog>
#include <QSettings>
#include <QList>

#include "Dmc1000.h"

namespace Ui {
class CMD;
}

class CMD : public QDialog
{
    Q_OBJECT

public:
    explicit CMD(QWidget *parent = 0);
    ~CMD();

    QSettings *configFile;

    long StrVel=0;
    long MaxVel=0;
    double Tacc=0;

    void disp_parameters();

private slots:

    void on_HIGH_SPEED_SET_editingFinished();

    void on_LOW_SPEED_SET_editingFinished();

    void on_High_Speed_Initial_editingFinished();

    void on_High_Speed_Acceleration_editingFinished();

    void on_Low_Speed_Initial_editingFinished();

    void on_Low_Speed_Acceleration_editingFinished();

    void on_HIGH_SPEED_clicked();

    void on_LOW_SPEED_clicked();
    void on_Camera1_up_clicked();

    void on_Camera1_down_clicked();

    void on_Camera2_up_clicked();

    void on_Camera2_down_clicked();

    void on_Anticlockwise_clicked();

    void on_Clockwise_clicked();

private:
    Ui::CMD *ui;
};

#endif // CMD_H
