#include "ic_capture.h"

IC_Capture::IC_Capture()
{
}

IC_Capture::~IC_Capture()
{
    CloseFramegrabber(hv_AcqHandle);
    CloseFramegrabber(hv_AcqHandle1);
}

void IC_Capture::run()
{
    try
    {
        OpenFramegrabber("DirectShow", 1, 1, 0, 0, 0, 0, "default", 8, "gray", -1, "false",
            "[0] Y800 (1024x768)", "[0]", 0, -1, &hv_AcqHandle);
        OpenFramegrabber("DirectShow", 1, 1, 0, 0, 0, 0, "default", 8, "gray", -1, "false",
            "[0] Y800 (1024x768)", "[1]", 0, -1, &hv_AcqHandle1);
    }
    catch(...)
    {
        emit signal_open_Camera(false);
    }
    emit signal_open_Camera(true);
    GrabImageStart(hv_AcqHandle, -1);
    GrabImageStart(hv_AcqHandle1, -1);
    while(1)//循环采集图像
    {
        if(action_enable)
        {
            GrabImageAsync(&ho_Image, hv_AcqHandle, -1);
            GrabImageAsync(&ho_Image1, hv_AcqHandle1, -1);
            emit signal_disp_image(ho_Image);
            emit signal_disp_image1(ho_Image1);
        }
    }
}


void IC_Capture::slot_action_enable(bool y_n)
{
    action_enable=y_n;
}
