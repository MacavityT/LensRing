#ifndef IC_CAPTURE_H
#define IC_CAPTURE_H
#include <QThread>
#include <QDebug>

#include "HalconCpp.h"

using namespace HalconCpp;

class IC_Capture:public QThread
{
    Q_OBJECT
public:
    IC_Capture();
    ~IC_Capture();
    void run();

    //Halcon variable
    HObject  ho_Image,ho_Image1;
    HTuple  hv_AcqHandle,hv_AcqHandle1;

    bool action_enable=false;

signals:
    void signal_open_Camera(bool);
    void signal_disp_image(HObject);
    void signal_disp_image1(HObject);

public slots:
    void slot_action_enable(bool);
};

#endif // IC_CAPTURE_H
