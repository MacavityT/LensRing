#ifndef SENSORTHREAD_H
#define SENSORTHREAD_H

#include <QDebug>
#include <QThread>

#include "Dmc1000.h"
#include "running.h"

class SensorThread:public QThread
{
    Q_OBJECT
public:
    SensorThread();
    ~SensorThread();
    void run();

    Running *running;

    int sensor1_previous=0;
    int sensor2_previous=0;
};

#endif // SENSORTHREAD_H
