#include "cmd.h"
#include "ui_cmd.h"

CMD::CMD(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CMD)
{
    ui->setupUi(this);
    //set window attribute
    setWindowTitle(tr("运行调试"));
    setMouseTracking(true);
    //variable initialization
    GenEmptyObj(&ho_Region);
    configFile=new QSettings(".\\Lens-Ring\\Temporary_File.ini",QSettings::IniFormat);
    cap=new IC_Capture;
    light=new QSerialPort;
    send_data[0]='$';
    send_data[1]=1;
    bright_value=configFile->value("Brightness").toInt();
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

    //connect function must be called in mainwindow
    //cause the project needs connection the same one image capture thread
    //with the two windows.

    //initializer the speed
    on_LOW_SPEED_clicked();
    disp_parameters();
    if(light->open(QIODevice::ReadWrite))
    {
        light->setBaudRate(9600);
        light->setDataBits(QSerialPort::Data8);
        light->setParity(QSerialPort::NoParity);
        light->setFlowControl(QSerialPort::NoFlowControl);
        light->setStopBits(QSerialPort::OneStop);
    }
    //set static variable "cam_cap" to true
    //when the variable is true,mainwindow display process will be cut off
    cap->cmd_cap=true;
    cap->action_enable=false;
    qDebug()<<"call cmd constructor";
}

CMD::~CMD()
{
    cap->cmd_cap=false;
    cap->cmd_cut=false;
    cap->action_enable=true;
    delete ui;
    //delete image capture
    cap->deleteLater();
    //delete configure file
    configFile->deleteLater();
    light->deleteLater();
}

void CMD::mouseMoveEvent(QMouseEvent *event)
{
    if(event->y()<370)//this value should be changed
    {
        hv_WindowHandle=hv_WindowHandle1;
    }
    else
    {
        hv_WindowHandle=hv_WindowHandle2;
    }
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
    ui->brightness->setText(configFile->value("Brightness").toString());
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

//Brightness adjust
void CMD::on_brightness_editingFinished()
{
    int tem=ui->brightness->text().toInt();
    if(tem>=0&&tem<=255)
    {
        configFile->setValue("Brightness",ui->brightness->text());
        bright_value=tem;
        send_data[2]=bright_value;
        send_data[3]=send_data[0]^send_data[1]^send_data[2];
        light->write(send_data);
    }
    else
    {
        QMessageBox::information(this,tr("Warning"),tr("Brightness value beyond limit!"),tr("OK"),0);
        return;
    }
}

void CMD::on_Brightness_add_clicked()
{
    bright_value+=1;
    send_data[2]=bright_value;
    send_data[3]=send_data[0]^send_data[1]^send_data[2];
    light->write(send_data);
    QString tem=bright_value+'0';
    ui->brightness->setText(tem);
    configFile->setValue("Brightness",bright_value);
}

void CMD::on_Brightness_decrease_clicked()
{
    bright_value-=1;
    send_data[2]=bright_value;
    send_data[3]=send_data[0]^send_data[1]^send_data[2];
    light->write(send_data);
    QString tem=bright_value+'0';
    ui->brightness->setText(tem);
    configFile->setValue("Brightness",bright_value);
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
    HTuple hv_Width, hv_Height;
//    ho_Image1=ic;
    ZoomImageSize(ic, &ic, 430, 310, "bilinear");
    if(first_open1)
    {
        GetImageSize(ic, &hv_Width, &hv_Height);
        SetWindowAttr("background_color","black");
        Hlong winID= this->winId();
        OpenWindow(30,70,430,310,winID,"visible","",&hv_WindowHandle1);
        SetPart(hv_WindowHandle1, 0, 0, hv_Height, hv_Width);
        first_open1=false;
        qDebug()<<"call cmd open window 1";
    }
    HDevWindowStack::Push(hv_WindowHandle1);
    if (HDevWindowStack::IsOpen())
      DispObj(ic, HDevWindowStack::GetActive());
}

void CMD::slot_disp_image2(HObject ic)
{
    HTuple hv_Width, hv_Height;
//    ho_Image2=ic;
    ZoomImageSize(ic, &ic, 430, 310, "bilinear");
    if(first_open2)
    {
        GetImageSize(ic, &hv_Width, &hv_Height);
        SetWindowAttr("background_color","black");
        Hlong winID= this->winId();
        OpenWindow(390,70,430,310,winID,"visible","",&hv_WindowHandle2);
        SetPart(hv_WindowHandle2, 0, 0, hv_Height, hv_Width);
        first_open2=false;
    }
    HDevWindowStack::Push(hv_WindowHandle2);
    if (HDevWindowStack::IsOpen())
      DispObj(ic, HDevWindowStack::GetActive());
}

//create tools
void CMD::on_circle_tool_clicked()
{
    if (HDevWindowStack::IsOpen())
    {
        SetColor(HDevWindowStack::GetActive(),"red");
//            SetColored(HDevWindowStack::GetActive(),12);
        SetDraw(HDevWindowStack::GetActive(),"margin");
        SetLineWidth(HDevWindowStack::GetActive(),3);
    }
    if(hv_WindowHandle.Length()!=0)
    {
//        qDebug()<<"gen circle";
        HObject ho_Circle;
        HTuple  hv_Row, hv_Column, hv_Radius;
        DrawCircle(hv_WindowHandle, &hv_Row, &hv_Column, &hv_Radius);
        GenCircle(&ho_Circle, hv_Row, hv_Column, hv_Radius);
        Union2(ho_Region, ho_Circle, &ho_Region);
        if (HDevWindowStack::IsOpen())
          DispObj(ho_Circle, HDevWindowStack::GetActive());
    }
}

void CMD::on_ellipse_tool_clicked()
{
    if (HDevWindowStack::IsOpen())
    {
        SetColor(HDevWindowStack::GetActive(),"red");
//            SetColored(HDevWindowStack::GetActive(),12);
        SetDraw(HDevWindowStack::GetActive(),"margin");
        SetLineWidth(HDevWindowStack::GetActive(),3);
    }
    if(hv_WindowHandle.Length()!=0)
    {
        HObject ho_Ellipse;
        HTuple  hv_Row1, hv_Column1, hv_Phi, hv_Radius1, hv_Radius2;
        DrawEllipse(hv_WindowHandle, &hv_Row1, &hv_Column1, &hv_Phi, &hv_Radius1, &hv_Radius2);
        GenEllipse(&ho_Ellipse, hv_Row1, hv_Column1, hv_Phi, hv_Radius1, hv_Radius2);
        Union2(ho_Region, ho_Ellipse, &ho_Region);
        if (HDevWindowStack::IsOpen())
          DispObj(ho_Ellipse, HDevWindowStack::GetActive());
    }
}

void CMD::on_rectangle1_tool_clicked()
{
    if (HDevWindowStack::IsOpen())
    {
        SetColor(HDevWindowStack::GetActive(),"red");
//            SetColored(HDevWindowStack::GetActive(),12);
        SetDraw(HDevWindowStack::GetActive(),"margin");
        SetLineWidth(HDevWindowStack::GetActive(),3);
    }
    if(hv_WindowHandle.Length()!=0)
    {
        HObject ho_Rectangle;
        HTuple hv_Row11, hv_Column11, hv_Row2, hv_Column2;
        DrawRectangle1(hv_WindowHandle, &hv_Row11, &hv_Column11, &hv_Row2, &hv_Column2);
        GenRectangle1(&ho_Rectangle, hv_Row11, hv_Column11, hv_Row2, hv_Column2);
        Union2(ho_Region, ho_Rectangle, &ho_Region);
        if (HDevWindowStack::IsOpen())
          DispObj(ho_Rectangle, HDevWindowStack::GetActive());
    }
}

void CMD::on_rectangle2_tool_clicked()
{
    if (HDevWindowStack::IsOpen())
    {
        SetColor(HDevWindowStack::GetActive(),"red");
//            SetColored(HDevWindowStack::GetActive(),12);
        SetDraw(HDevWindowStack::GetActive(),"margin");
        SetLineWidth(HDevWindowStack::GetActive(),3);
    }
    if(hv_WindowHandle.Length()!=0)
    {
        HObject ho_Rectangle1;
        HTuple hv_Row3, hv_Column3, hv_Phi1, hv_Length1, hv_Length2;
        DrawRectangle2(hv_WindowHandle, &hv_Row3, &hv_Column3, &hv_Phi1, &hv_Length1, &hv_Length2);
        GenRectangle2(&ho_Rectangle1, hv_Row3, hv_Column3, hv_Phi1, hv_Length1, hv_Length2);
        Union2(ho_Region, ho_Rectangle1, &ho_Region);
        if (HDevWindowStack::IsOpen())
          DispObj(ho_Rectangle1, HDevWindowStack::GetActive());
    }
}

void CMD::on_free_tool_clicked()
{
    if (HDevWindowStack::IsOpen())
    {
        SetColor(HDevWindowStack::GetActive(),"red");
//            SetColored(HDevWindowStack::GetActive(),12);
        SetDraw(HDevWindowStack::GetActive(),"margin");
        SetLineWidth(HDevWindowStack::GetActive(),3);
    }
    if(hv_WindowHandle.Length()!=0)
    {
        HObject ho_region;
        DrawRegion(&ho_region, hv_WindowHandle);
        Union2(ho_Region, ho_region, &ho_Region);
        if (HDevWindowStack::IsOpen())
          DispObj(ho_region, HDevWindowStack::GetActive());
    }
}

void CMD::on_Create_Model1_clicked()
{
    HTuple hv_Number;
    CountObj(ho_Region, &hv_Number);
    if(hv_Number==0)
    {
        return;
    }
    Union1(ho_Region, &ho_Region);

    QString tmp=select_path();
    if(tmp=="Error")
    {
        QMessageBox::information(this,tr("Warning"),tr("Create Model Failed!"),tr("OK"),0);
        cap->cmd_cut=false;
        return;
    }

    //Create shape model
    //Write shape model

    //finally clear the region
    GenEmptyObj(&ho_Region);
    cap->cmd_cut=false;
}

//union all regions and select path to save
void CMD::on_Create_Model2_clicked()
{
    HTuple hv_Number;
    CountObj(ho_Region, &hv_Number);
    if(hv_Number==0)
    {
        return;
    }
    Union1(ho_Region, &ho_Region);

    QString tmp=select_path();
    if(tmp=="Error")
    {
        QMessageBox::information(this,tr("Warning"),tr("Create Model Failed!"),tr("OK"),0);
        cap->cmd_cut=false;
        return;
    }

    //Create shape model
    //Write shape model
    //finally clear the region
    GenEmptyObj(&ho_Region);
    cap->cmd_cut=false;
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

