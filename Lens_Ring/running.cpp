#include "running.h"
#include <QDebug>

bool Running::pause=false;
bool Running::resume=false;

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
    int total_position=0;
    for(int i=0;i<10;i++)
    {
        if(rotation[i]==0)
        {
            total_position=i;
            break;
        }
    }
    while(true)
    {
        if(reset_finished)
        {
            //rotation
            d1000_start_sv_move(0,speed_set[0][0],speed_set[0][1],speed_set[0][2]);
            while(d1000_check_done(0)==0)
            {
                //emit detection signals when arrive corresponding position

                while(pause)
                {}
            }
        }
    }
}


////initial parameters
void Running::get_config_param(int num)
{
    //get detection position
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
    //get running speed
    QString axis;
    for(int axis_index=0;axis_index<=2;axis_index++)
    {
        axis=axis_index+'0';
        speed_set[axis_index][0]=configFile->value("Running_Speed/Speed_Initial"+axis).toDouble();
        speed_set[axis_index][1]=configFile->value("Running_Speed/Speed_Set"+axis).toDouble();
        speed_set[axis_index][2]=configFile->value("Running_Speed/Acceleration"+axis).toDouble();
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
    for(int i=0;i<=2;i++)
    {
        d1000_home_move(i,speed_set[i][0],speed_set[i][1],speed_set[i][2]);
    }
    //when rotation origin sensor is triggered , stop it
    int x;
    do
    {
        x=0;
        for(int j=0;j<=2;j++)
        {
            x+=d1000_check_done(j);
        }
        while(pause)
        {
//wait for resume
        }
    }while(x!=3);
    //clear the position
    for(int k=0;k<=2;k++)
    {
        d1000_set_command_pos(k,0);
    }
    //move to detection position
    reset_finished=true;
    emit signal_lock_all_buttons(false);
}
