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

    bool cmd_cap=false;

    //Halcon variable
    HObject  ho_Image1,ho_Image2,ho_Image3,ho_Image4;
    HTuple  hv_AcqHandle1,hv_AcqHandle2,hv_AcqHandle3,hv_AcqHandle4;

    bool action_enable=false;

signals:
    void signal_open_Camera(bool);
    void signal_disp_image1(HObject);
    void signal_disp_image2(HObject);
    void signal_disp_image3(HObject);
    void signal_disp_image4(HObject);

public slots:

};

#endif // IC_CAPTURE_H
