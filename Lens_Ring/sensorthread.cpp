#include "sensorthread.h"

SensorThread::SensorThread()
{
    running=new Running;
    sensor1_previous=!d1000_in_bit(5);
    sensor2_previous=!d1000_in_bit(6);
}

SensorThread::~SensorThread()
{
    running->deleteLater();
}

void SensorThread::run()
{    
    int sensor1_present;
    int sensor2_present;
    while(true)
    {
        //cross sensor
        sensor1_present=!d1000_in_bit(5);
        if(sensor1_present-sensor1_previous==1)
        {
            running->rise_edge1=true;
        }
        sensor1_previous=sensor1_present;
        //plane sensor
        sensor2_present=!d1000_in_bit(6);
        if(sensor2_present-sensor2_previous==1)
        {
            running->rise_edge2=true;
        }
        sensor2_previous=sensor2_present;
    }
}
