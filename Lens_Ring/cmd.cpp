#include "cmd.h"
#include "ui_cmd.h"

CMD::CMD(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CMD)
{
    ui->setupUi(this);

    QList<QAbstractButton*> list=ui->CMD_Button->buttons();
    QList<QAbstractButton*>::iterator i;
    for(i=list.begin();i!=list.end();++i)
    {
        QAbstractButton *temporary_button=*i;
        temporary_button->setAutoRepeat(true);
        temporary_button->setAutoRepeatDelay(0);
        temporary_button->setAutoRepeatInterval(10);
    }
}

CMD::~CMD()
{
    delete ui;
}




void CMD::on_Camera1_up_clicked()
{
}

void CMD::on_Camera1_down_clicked()
{

}

void CMD::on_Camera2_up_clicked()
{

}

void CMD::on_Camera2_down_clicked()
{

}

void CMD::on_Anticlockwise_clicked()
{

}

void CMD::on_Clockwise_clicked()
{

}
