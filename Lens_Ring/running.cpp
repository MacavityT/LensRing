#include "running.h"
#include <QDebug>

Running::Running()
{
    configFile=new QSettings(".\\Lens-Ring\\Parameters_Setting.ini",QSettings::IniFormat);
    configFile1=new QSettings(".\\Lens-Ring\\Temporary_File.ini",QSettings::IniFormat);
    read_all_model();
}

Running::~Running()
{
    configFile->deleteLater();
    configFile1->deleteLater();
}


void Running::run()
{

}


////initial parameters
void Running::get_config_param(int num)
{
    QString index=num+'0';
    QString x;
    camera[0]=configFile->value("Detection_Position/part"+index+"_camera1").toDouble();
    camera[1]=configFile->value("Detection_Position/part"+index+"_camera2").toDouble();
    for(int i=1;i<=10;i++)
    {
        if(i!=10)
        {
            x=i+'0';
        }
        else
        {
            x="10";
        }
        rotation[i-1]=configFile->value("Detection_Position/part"+index+"_rotation"+x).toDouble();
//        qDebug()<<"Detection_Position/part"+index+"_rotation"+x;
    }    
}

void Running::slot_read_model(int i)
{
    QString num=i+'0';
    model_path.insert(i-1,configFile1->value("Model_path/model"+num).toString());
//    qDebug()<<*(model_path.begin());
}

void Running::read_all_model()
{
    int i;
    for(i=1;;i++)
    {
        QString index=i+'0';
        if(configFile1->contains("Model_path/model"+index))
        {
            model_path.insert(i-1,configFile1->value("Model_path/model"+index).toString());
            model_number=i;
        }
        else
        {
            break;
        }
    }
//    qDebug()<<model_number;
//    qDebug()<<configFile1->contains("Model_path/model9");
}


////running process control
void Running::slot_reset()
{
    //origin back process
    reset_finished=true;
}
