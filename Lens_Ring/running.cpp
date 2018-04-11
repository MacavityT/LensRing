#include "running.h"

Running::Running()
{
    configFile=new QSettings(".\\Lens-Ring\\Parameters_Setting.ini",QSettings::IniFormat);
}

Running::~Running()
{
    configFile->deleteLater();
}


void Running::run()
{}

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
