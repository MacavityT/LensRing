#include "ic_capture.h"

IC_Capture::IC_Capture()
{
}

IC_Capture::~IC_Capture()
{
    CloseFramegrabber(hv_AcqHandle1);
    CloseFramegrabber(hv_AcqHandle2);
}

void IC_Capture::run()
{
    qDebug()<<"image capture start";
    try
    {
        OpenFramegrabber("DirectShow", 1, 1, 0, 0, 0, 0, "default", 8, "gray", -1, "false",
            "[0] Y800 (1024x768)", "[0]", 0, -1, &hv_AcqHandle1);
        OpenFramegrabber("DirectShow", 1, 1, 0, 0, 0, 0, "default", 8, "gray", -1, "false",
            "[0] Y800 (1024x768)", "[1]", 0, -1, &hv_AcqHandle2);
    }
    catch(...)
    {
        emit signal_open_Camera(false);
    }
    emit signal_open_Camera(true);
    GrabImageStart(hv_AcqHandle1, -1);
    GrabImageStart(hv_AcqHandle2, -1);
    while(1)//循环采集图像
    {
        if(action_enable)
        {
            cmd_cap=false;
            GrabImageAsync(&ho_Image1, hv_AcqHandle1, -1);
            GrabImageAsync(&ho_Image2, hv_AcqHandle2, -1);
            emit signal_disp_image1(ho_Image1);
            emit signal_disp_image2(ho_Image2);
        }
        if(cmd_cap)
        {
            action_enable=false;
            GrabImageAsync(&ho_Image3, hv_AcqHandle3, -1);
            GrabImageAsync(&ho_Image4, hv_AcqHandle4, -1);
            emit signal_disp_image3(ho_Image3);
            emit signal_disp_image4(ho_Image4);
        }
    }
}
