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

    bool reset_finished=false;

    QSettings *configFile;
    QSettings *configFile1;
    double camera[2];
    double rotation[10];
    double speed_set[3][3];
    QStringList model_path;

    int model_number=0;
    void read_all_model();


public slots:
    void get_config_param(int);
    void slot_read_model(int);
    void slot_reset();
};

#endif // Running_H
