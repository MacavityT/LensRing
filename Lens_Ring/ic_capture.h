#ifndef IC_CAPTURE_H
#define IC_CAPTURE_H
#include <QThread>
#include <QDebug>

#include "HalconCpp.h"
#include "running.h"

using namespace HalconCpp;

class IC_Capture:public QThread
{
    Q_OBJECT
public:
    IC_Capture();
    ~IC_Capture();
    void run();

    //Halcon variable
    static HObject  ho_Image1,ho_Image2;
    HTuple  hv_AcqHandle1,hv_AcqHandle2;

    static bool action_enable;
    static bool cmd_cut;
    static bool cmd_cap;

    Running *ic_run;

signals:
    void signal_open_Camera(bool);
    void signal_disp_image1(HObject);
    void signal_disp_image2(HObject);
    void signal_detection_image1(HObject);
    void signal_detection_image2(HObject);

    void signal_disp_image3(HObject);
    void signal_disp_image4(HObject);

    void signal_cmd_image1(HObject);
    void signal_cmd_image2(HObject);

public slots:
//    void slot_cmd_image1();
//    void slot_cmd_image2();

};

#endif // IC_CAPTURE_H
