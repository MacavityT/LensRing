#include "cmd.h"
#include "ui_cmd.h"

CMD::CMD(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CMD)
{
    ui->setupUi(this);
    setWindowTitle(tr("运行调试"));
    configFile=new QSettings(".\\Lens-Ring\\Temporary_File.ini",QSettings::IniFormat);
    cap=new IC_Capture;
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

    //connect this to image capture thread
    connect(cap,SIGNAL(signal_disp_image3(HObject)),this,SLOT(slot_disp_image1(HObject)));
    connect(cap,SIGNAL(signal_disp_image4(HObject)),this,SLOT(slot_disp_image2(HObject)));
    //initializer the speed
    on_LOW_SPEED_clicked();
    disp_parameters();
    //set static variable "cam_cap" to true
    //when the variable is true,mainwindow display process will be cut off
    cap->cmd_cap=true;
}

CMD::~CMD()
{
    delete ui;
    //delete image capture
    cap->deleteLater();
    //delete configure file
    configFile->deleteLater();
}

////UI control and commissioning control
///
void CMD::disp_parameters()
{
    QString _position=QString::number(d1000_get_command_pos(0));
    QString _position1=QString::number(d1000_get_command_pos(1));
    QString _position2=QString::number(d1000_get_command_pos(2));
    ui->position0->setText(_position);
    ui->position1->setText(_position1);
    ui->position2->setText(_position2);
    ui->High_Speed_Initial->setText(configFile->value("Speed_Set/High_Speed_Initial").toString());
    ui->HIGH_SPEED_SET->setText(configFile->value("Speed_Set/High_Speed").toString());
    ui->High_Speed_Acceleration->setText(configFile->value("Speed_Set/High_Speed_Acceleration").toString());
    ui->Low_Speed_Initial->setText(configFile->value("Speed_Set/Low_Speed_Initial").toString());
    ui->LOW_SPEED_SET->setText(configFile->value("Speed_Set/Low_Speed").toString());
    ui->Low_Speed_Acceleration->setText(configFile->value("Speed_Set/Low_Speed_Acceleration").toString());
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
//camera2 = rise and descend
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





////Create model
///
//request image(cut image to pictre rather than video)
void CMD::on_cap_image1_clicked()
{
//    emit signal_cap_image1();
    cap->cmd_cut=true;
}

void CMD::on_cap_image2_clicked()
{
//    emit signal_cap_image2();
    cap->cmd_cut=true;
}

//acquire image
void CMD::slot_disp_image1(HObject ic)
{
    image1=ic;
}

void CMD::slot_disp_image2(HObject ic)
{
    image2=ic;
}

//create tools
void CMD::on_circle_tool_clicked()
{

}

void CMD::on_ellipse_tool_clicked()
{

}

void CMD::on_rectangle1_tool_clicked()
{

}

void CMD::on_rectangle2_tool_clicked()
{

}

void CMD::on_free_tool_clicked()
{

}

void CMD::on_Create_Model1_clicked()
{
    //Create shape model

    QString tmp=select_path();
    if(tmp=="Error")
    {
        QMessageBox::information(this,tr("Warning"),tr("Create Model Failed!"),tr("OK"),0);
        return;
    }
    //Write shape model
}

//union all regions and select path to save
void CMD::on_Create_Model2_clicked()
{
    //unite all the input regions "union1"
    //Create shape model

    QString tmp=select_path();
    if(tmp=="Error")
    {
        QMessageBox::information(this,tr("Warning"),tr("Create Model Failed!"),tr("OK"),0);
        return;
    }
    //Write shape model
}

QString CMD::select_path()
{
    QFileDialog *fileDialog=new QFileDialog(this);
    QString filename =fileDialog->getSaveFileName(this,tr("Model name"),"",tr("Images(*.png *.jpg *.jpeg *.bmp)"));
    //define fileDialog title
    fileDialog->setWindowTitle(tr("保存图片"));
    //set default path
    fileDialog->setDirectory("./");
    if(filename.isEmpty())
    {
        return "Error";
    }
    else
    {
//        qDebug()<<filename;
        return filename;
    }
}
