#ifndef SENSOR_H
#define SENSOR_H
#include <QThread>


class Sensor:public QThread
{
    Q_OBJECT
public:
    Sensor();
    ~Sensor();
    void run();
};

#endif // SENSOR_H
