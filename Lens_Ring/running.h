#ifndef Running_H
#define Running_H
#include <QThread>
#include <QSettings>
#include <QDebug>
#include <QMutex>
#include <QMutexLocker>

#include "ic_capture.h"
#include "Dmc1000.h"
#include "HalconCpp.h"

using namespace HalconCpp;
struct ThreadControl{
    QMutex mutex;
};

class Running:public QThread
{
    Q_OBJECT
public:
    Running();
    ~Running();
    void run();

    IC_Capture *cap;
    //running process control
    static bool pause;
    static bool resume;
    static bool stop;
    static bool rise_edge1;
    static bool rise_edge2;
    static bool rotation_running;
    static int detection;
    static bool reset_start;
    static bool move_to_detection_position_start;
    bool reset_finished=false;
    bool detection_position_arrive=false;

    QSettings *configFile;
    QSettings *configFile1;
    //running thread parameters
    double camera[2];
    double rotation[10];
    double speed_set[3][3];
    double limit;
    QStringList model_path;
    QList<HTuple> MODEL;
    QList<HObject> Region;
    QList<HObject> StandardImage;
    int model_number=0;
    int total_position=0;
    //temporary flag
    bool Interruption=false;
    bool model_path_changed=false;
    int detection_number=-1;
    double difference1=0;
    double difference2=0;
    //parameters load function
    void read_all_model();
    void move_to_detection_position();
    void all_axis_check_done();
    //running process function
    void single_axis_action_absolute(short,long);
    void single_axis_check_done(short,long);
    void single_origin_back(short);
    //Inspection images
    void Inspection_image1(HObject);
    void Inspection_image2(HObject);

signals:
    void signal_lock_all_buttons(bool);
    void signal_disp_result(int,int);


public slots:
    void get_config_param(int);
    void slot_read_model(int);
    void slot_reset();
};

#endif // Running_H
