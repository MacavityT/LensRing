#ifndef Running_H
#define Running_H
#include <QThread>
#include <QSettings>
#include <QDebug>
#include "Dmc1000.h"


class Running:public QThread
{
    Q_OBJECT
public:
    Running();
    ~Running();
    void run();

    QSettings *configFile;
    double camera[2];
    double rotation[10];


public slots:
    void get_config_param(int);
};

#endif // Running_H
