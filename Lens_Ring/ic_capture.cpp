#include "ic_capture.h"

bool IC_Capture::cmd_cap=false;
bool IC_Capture::cmd_cut=false;
bool IC_Capture::action_enable=false;

IC_Capture::IC_Capture()
{
}

IC_Capture::~IC_Capture()
{
    if(hv_AcqHandle1.Length()!=0)
    {
        CloseFramegrabber(hv_AcqHandle1);
    }
    if(hv_AcqHandle2.Length()!=0)
    {
        CloseFramegrabber(hv_AcqHandle2);
    }
}

void IC_Capture::run()
{
    qDebug()<<"image capture start";
    //open the frame grabber
    try
    {
        OpenFramegrabber("DirectShow", 1, 1, 0, 0, 0, 0, "default", 8, "gray", -1, "false",
            "[0] Y800 (1024x768)", "[0]", 0, -1, &hv_AcqHandle1);
//        OpenFramegrabber("DirectShow", 1, 1, 0, 0, 0, 0, "default", 8, "gray", -1, "false",
//            "[0] Y800 (1024x768)", "[1]", 0, -1, &hv_AcqHandle2);
    }
    //this can also use code: catch(...)  to catch all kinds of abnormal
    catch(HalconCpp::HException &HDevExpDefaultException)
    {
        HTuple hv_Exception;
        HDevExpDefaultException.ToHTuple(&hv_Exception);
        emit signal_open_Camera(false);
        return;
    }
    emit signal_open_Camera(true);
    //Start an asynchronous grab from the specified image acquisition device.
    GrabImageStart(hv_AcqHandle1, -1);
//    GrabImageStart(hv_AcqHandle2, -1);
    //image capture
    while(1)
    {
        GrabImageAsync(&ho_Image1, hv_AcqHandle1, -1);
        ZoomImageSize(ho_Image1, &ho_Image1, 430, 310, "bilinear");
//        GrabImageAsync(&ho_Image2, hv_AcqHandle2, -1);
//        ZoomImageSize(ho_Image2, &ho_Image2, 430, 310, "bilinear");
        if(action_enable)
        {
            emit signal_disp_image1(ho_Image1);
//            emit signal_disp_image2(ho_Image2);
        }
        if(cmd_cap&&!cmd_cut)
        {
            emit signal_disp_image3(ho_Image1);
//            emit signal_disp_image4(ho_Image2);
        }
    }
}
