#ifndef SENSOR_H
#define SENSOR_H
#include <QThread>
#include "Dmc1000.h"


class Sensor:public QThread
{
    Q_OBJECT
public:
    Sensor();
    ~Sensor();
    void run();
};

#endif // SENSOR_H
