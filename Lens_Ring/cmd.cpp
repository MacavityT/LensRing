#include "cmd.h"
#include "ui_cmd.h"

CMD::CMD(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CMD)
{
    ui->setupUi(this);
    configFile=new QSettings(".\\Lens-Ring\\Temporary_File.ini",QSettings::IniFormat);
// let the buttons of commissing be auto-repeat
    QList<QAbstractButton*> list=ui->CMD_Button->buttons();
    QList<QAbstractButton*>::iterator i;
    for(i=list.begin();i!=list.end();++i)
    {
        QAbstractButton *temporary_button=*i;
        temporary_button->setAutoRepeat(true);
        temporary_button->setAutoRepeatDelay(0);
        temporary_button->setAutoRepeatInterval(10);
    }
    //initializer the speed
    on_LOW_SPEED_clicked();
}

CMD::~CMD()
{
    delete ui;
    configFile->deleteLater();
}

void CMD::disp_parameters()
{

}

//High Speed parameters set.
void CMD::on_High_Speed_Initial_editingFinished()
{
    configFile->setValue("Speed_Set/High_Speed_Initial",ui->High_Speed_Initial->text());
}

void CMD::on_HIGH_SPEED_SET_editingFinished()
{
    configFile->setValue("Speed_Set/High_Speed",ui->HIGH_SPEED_SET->text());
}

void CMD::on_High_Speed_Acceleration_editingFinished()
{
    configFile->setValue("Speed_Set/High_Speed_Acceleration",ui->High_Speed_Acceleration->text());
}

//Low Speed parameters set.
void CMD::on_Low_Speed_Initial_editingFinished()
{
    configFile->setValue("Speed_Set/Low_Speed_Initial",ui->Low_Speed_Initial->text());
}

void CMD::on_LOW_SPEED_SET_editingFinished()
{
    configFile->setValue("Speed_Set/Low_Speed",ui->LOW_SPEED_SET->text());
}

void CMD::on_Low_Speed_Acceleration_editingFinished()
{
    configFile->setValue("Speed_Set/Low_Speed_Acceleration",ui->Low_Speed_Acceleration->text());
}

//Mode Select
void CMD::on_HIGH_SPEED_clicked()
{
    StrVel=configFile->value("Speed_Set/High_Speed_Initial").toDouble();
    MaxVel=configFile->value("Speed_Set/High_Speed").toDouble();
    Tacc=configFile->value("Speed_Set/High_Speed_Acceleration").toDouble();
}

void CMD::on_LOW_SPEED_clicked()
{
    StrVel=configFile->value("Speed_Set/Low_Speed_Initial").toDouble();
    MaxVel=configFile->value("Speed_Set/Low_Speed").toDouble();
    Tacc=configFile->value("Speed_Set/Low_Speed_Acceleration").toDouble();
}

//Commissioning Button function.

void CMD::on_Camera1_up_clicked()
{
    d1000_start_t_move(2,50,StrVel,MaxVel,Tacc);
    QString position=QString::number(d1000_get_command_pos(2)+50);
    ui->position2->setText(position);
}

void CMD::on_Camera1_down_clicked()
{
    d1000_start_t_move(2,-50,StrVel,MaxVel,Tacc);
    QString position=QString::number(d1000_get_command_pos(2)-50);
    ui->position2->setText(position);
}

void CMD::on_Camera2_up_clicked()
{
    d1000_start_t_move(1,50,StrVel,MaxVel,Tacc);
    QString position=QString::number(d1000_get_command_pos(1)+50);
    ui->position1->setText(position);
}

void CMD::on_Camera2_down_clicked()
{
    d1000_start_t_move(1,-50,StrVel,MaxVel,Tacc);
    QString position=QString::number(d1000_get_command_pos(1)-50);
    ui->position1->setText(position);
}

void CMD::on_Anticlockwise_clicked()
{
    d1000_start_t_move(0,-50,StrVel,MaxVel,Tacc);
    QString position=QString::number(d1000_get_command_pos(0)-50);
    ui->position0->setText(position);
}

void CMD::on_Clockwise_clicked()
{
    d1000_start_t_move(0,50,StrVel,MaxVel,Tacc);
    QString position=QString::number(d1000_get_command_pos(0)+50);
    ui->position0->setText(position);
}
