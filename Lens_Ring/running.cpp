#include "running.h"
#include <QDebug>

bool Running::pause=false;
bool Running::resume=false;
bool Running::stop=false;
bool Running::rise_edge1=false;
bool Running::rise_edge2=false;
bool Running::reset_start=false;
bool Running::move_to_detection_position_start=false;
int Running::detection=0;

Running::Running()
{
    configFile=new QSettings(".\\Lens-Ring\\Parameters_Setting.ini",QSettings::IniFormat);
    configFile1=new QSettings(".\\Lens-Ring\\Temporary_File.ini",QSettings::IniFormat);
    memset(rotation,0,sizeof (double) *10);//fill specific value and its size to a memory
    read_all_model();
    HTuple OriginalClipRegion;
    GetSystem("clip_region",&OriginalClipRegion);
    SetSystem("clip_region","false");
}

Running::~Running()
{
    configFile->deleteLater();
    configFile1->deleteLater();
}


void Running::run()
{
    /*through the position number to confirm if the lens (1) image  need be detected.
     if total_position=0 , and the model path number equal to 1,the lens 1 needn't to be used.
     and when total_position=0 and the model path number equal to 2,both of two lens need be used. */
    int index=0;//this is the position number will be compared.
    while(true)
    {
        ///reset process
        if(reset_start)
        {
            slot_reset();
        }
        if(move_to_detection_position_start)
        {
            move_to_detection_position();
        }
        ///single camera(1) is used
        if(total_position!=0&&detection==0&&rise_edge1&&reset_finished&&detection_position_arrive)
        {
            rise_edge1=false;
            while(index!=total_position)//"status" is to describe the image detection result.
            {
                if(Interruption)
                {
                    d1000_immediate_stop(0);
                    Interruption=false;
                    break;
                }
                single_axis_action_absolute(0,rotation[index]);
                single_axis_check_done(0,rotation[index]);
                Sleep(200);
                detection_number=index;
                while(detection==1)
                {}
                detection=1;
                index++;
            }
            single_origin_back(0);
            index=0;
        }
        ///single camera(2) is used
        if(total_position==0&&detection==0&&rise_edge2&&model_number==1&&\
                reset_finished&&detection_position_arrive)
        {
            detection=2;
            rise_edge2=false;
            //request image 1 detection
            if(stop)
            {
                return;
            }
        }
        ///both of two camera are used
        if(total_position==0&&detection==0&&rise_edge2&&model_path.length()==2&&\
                reset_finished&&detection_position_arrive)
        {
            detection=3;
            rise_edge2=false;
            //request image 2 detection
            if(stop)
            {
                return;
            }
        }
    }
}


////initial parameters
void Running::get_config_param(int num)
{
    //get detection position
    QString index=num+'0';
    limit=configFile->value("limit/part"+index).toDouble();
    QString x;
    camera[0]=configFile->value("Detection_Position/part"+index+"_camera1").toDouble();
    camera[1]=configFile->value("Detection_Position/part"+index+"_camera2").toDouble();
    for(int i=1;i<=10;i++)
    {
        if(i!=10)
        {
            x=i+'0';
        }
        else
        {
            x="10";
        }
        rotation[i-1]=configFile->value("Detection_Position/part"+index+"_rotation"+x).toDouble();
//        qDebug()<<"Detection_Position/part"+index+"_rotation"+x;
    }    
    //get running speed
    QString axis;
    for(int axis_index=0;axis_index<=2;axis_index++)
    {
        axis=axis_index+'0';
        speed_set[axis_index][0]=configFile->value("Running_Speed/Speed_Initial"+axis).toDouble();
        speed_set[axis_index][1]=configFile->value("Running_Speed/Speed_Set"+axis).toDouble();
        speed_set[axis_index][2]=configFile->value("Running_Speed/Acceleration"+axis).toDouble();
    }
    //calculate the total number
    for(int in=0;in<10;in++)
    {
        if(rotation[in]==0)
        {
            total_position=in;//this value is equal to natural number (except 0),but arraies are start with 0
            break;
        }
    }
    qDebug()<<"total position ="<<total_position;
    qDebug()<<"rotation[0]="<<rotation[0];
    qDebug()<<"rotation[1]="<<rotation[1];
    qDebug()<<"rotation[2]="<<rotation[2];
    qDebug()<<"rotation[3]="<<rotation[3];
    qDebug()<<"rotation[4]="<<rotation[4];
    qDebug()<<"rotation[5]="<<rotation[5];
}

void Running::slot_read_model(int i)
{
    QString num=i+'0';
    model_path.insert(i,configFile1->value("Model_path/model"+num).toString());
    model_path_changed=true;
    //read shape model
//    qDebug()<<*(model_path.begin());
}

void Running::read_all_model()
{
    int i;
    for(i=1;;i++)
    {
        QString index=i+'0';
        if(configFile1->contains("Model_path/model"+index))
        {
            //get path.
            QString tmp=configFile1->value("Model_path/model"+index).toString();
            QString tmp_region=tmp+"Region.hobj";
            QString tmp_image=tmp+"Image.tif";
            qDebug()<<"read model path"<<tmp;
            model_path.insert(i,tmp);
            //save model to container
            HTuple hv_ModelID;
            QByteArray tmp_array=tmp.toLatin1();
            char* path=tmp_array.data();
            ReadShapeModel(path,&hv_ModelID);
            MODEL.insert(i,hv_ModelID);
            //save region to container
            HObject save_region;
            QByteArray tmp_region_array=tmp_region.toLatin1();
            char* region_path=tmp_region_array.data();
            ReadRegion(&save_region,region_path);
            Region.insert(i,save_region);
            //save image to standard image container
            HObject save_image;
            QByteArray tmp_image_array=tmp_image.toLatin1();
            char* image_path=tmp_image_array.data();
            ReadImage(&save_image,image_path);
            StandardImage.insert(i,save_image);
            //count model number.
            model_number=i;
            qDebug()<<"model number="<<model_number;
        }
        else
        {
            break;
        }
    }
    //read shape model

    model_path_changed=false;
//    qDebug()<<"model path size"<<model_path.size();
//    qDebug()<<model_number;
//    qDebug()<<configFile1->contains("Model_path/model9");   
}


////running process control
void Running::slot_reset()
{
    //set flag
    reset_start=false;
    reset_finished=false;
    detection_position_arrive=false;
    emit signal_lock_all_buttons(true);
    //origin back process
    for(int i=0;i<=2;i++)
    {
        d1000_home_move(i,speed_set[i][0],speed_set[i][1],speed_set[i][2]);
    }
    all_axis_check_done();
    //clear the position
    for(int k=0;k<=2;k++)
    {
        d1000_set_command_pos(k,0);
    }
    //set flag
    reset_finished=true;
    emit signal_lock_all_buttons(false);
    qDebug()<<"All reset finished";
}

void Running::move_to_detection_position()
{
    move_to_detection_position_start=false;
    detection_position_arrive=false;
    emit signal_lock_all_buttons(true);
    d1000_home_move(0,speed_set[0][0],speed_set[0][1],speed_set[0][2]);
    d1000_start_ta_move(1,camera[0],speed_set[1][0],speed_set[1][1],speed_set[1][2]);
    d1000_start_ta_move(2,camera[1],speed_set[2][0],speed_set[2][1],speed_set[2][2]);
    all_axis_check_done();
    detection_position_arrive=true;
    emit signal_lock_all_buttons(false);
}

void Running::all_axis_check_done()
{
    int x,y,z;
    do
    {
        x=d1000_check_done(0);
        y=d1000_check_done(1);
        z=d1000_check_done(2);
        while(pause)
        {
            //wait for resume
        }
    }while(x==0||y==0||z==0);
}

void Running::single_axis_check_done(short axis, long pulse)
{
    do
    {
        while(pause)
        {}
    }while(!(d1000_check_done(axis)));
    //here is the programmer of compare the position
    if(resume&&pulse&&pulse!=(d1000_get_command_pos(axis)))//if there are a pulse parameter and the position have not get the correct falg
    {

        resume=false;
        d1000_start_ta_move(axis,pulse,speed_set[axis][0],speed_set[axis][1],speed_set[axis][2]);
        single_axis_check_done(axis,pulse);
    }
}

void Running::single_origin_back(short axis)
{
    d1000_home_move(axis,speed_set[axis][0],speed_set[axis][1],speed_set[axis][2]);
    single_axis_check_done(axis,NULL);
    d1000_set_command_pos(axis,0);
}

void Running::single_axis_action_absolute(short axis, long pulse)
{
    d1000_start_ta_move(axis,pulse,speed_set[axis][0],speed_set[axis][1],speed_set[axis][2]);
    single_axis_check_done(axis,pulse);
}

////image detection functions
void Running::slot_detection_image1(HObject image)
{
    //variable define
    HObject ho_ImageMedian,RegionAffineTrans,RegionMoved;
    HObject ho_ThresholdRegion, ho_ConnectedRegions,ho_SelectedRegions;
    HObject ImageDetect,DetectionRegion,ImageSub,RegionSub;
//    HTuple HomMat2DIdentity,HomMat2DScale;
    HTuple Area, Row, Column;
    HTuple AreaSub,RowSub,ColumnSub;
    HTuple hv_Row1, hv_Column1, hv_Angle1, hv_Score1, Scale1;
    HTuple HomMat2D;
    //preprocess
    MedianImage(image,&ho_ImageMedian,"circle",2,"mirrored");
//    HomMat2dIdentity(&HomMat2DIdentity);
//    HomMat2dScale(HomMat2DIdentity,2,2,0,0,&HomMat2DScale);
//    AffineTransRegion(*(Region.begin()+detection_number),&RegionAffineTrans,HomMat2DScale,"nearest_neighbor");
    if(detection_number>=0)
    {
        qDebug()<<"image detection 1 (rotatation)";
        //single camera 1 , need to rotate      
        //image detection
        AreaCenter(*(Region.begin()+detection_number),&Area,&Row,&Column);
        MoveRegion(*(Region.begin()+detection_number),&RegionMoved,-Row,-Column);
        FindScaledShapeModel(ho_ImageMedian, *(MODEL.begin()+detection_number), 0, 6.29, 0.1, 10, 0.5, 1, 0.5, "least_squares",
            0, 0.9, &hv_Row1, &hv_Column1, &hv_Angle1, &Scale1, &hv_Score1);
        if(hv_Score1.Length()<=0)
        {
            Interruption=true;
            difference1=0;
            emit signal_disp_result(1,2);
            return;
        }
        VectorAngleToRigid(0,0,0,hv_Row1,hv_Column1,hv_Angle1,&HomMat2D);
        AffineTransRegion(RegionMoved,&DetectionRegion,HomMat2D,"nearest_neighbor");
        ReduceDomain(ho_ImageMedian,DetectionRegion,&ImageDetect);

        FastThreshold(ImageDetect, &ho_ThresholdRegion, 128, 255, 20);
        Connection(ho_ThresholdRegion, &ho_ConnectedRegions);
        SelectShape(ho_ConnectedRegions, &ho_SelectedRegions, "area", "and", 150, 10e4);
        Union1(ho_SelectedRegions, &DetectionRegion);
        ReduceDomain(ImageDetect,DetectionRegion,&ImageDetect);

        SubImage(*(StandardImage.begin()+detection_number),ImageDetect,&ImageSub,1,0);
        Threshold(ImageSub,&RegionSub,100,255);
        AreaCenter(RegionSub,&AreaSub,&RowSub,&ColumnSub);
        difference1=difference1+AreaSub;

        if(detection_number!=total_position-1)
        {
            if(difference1>=limit)
            {
                Interruption=true;
                difference1=0;
                emit signal_disp_result(1,2);
            }
            else
            {
                emit signal_disp_result(1,3);
            }
        }
        else
        {
            if(difference1<limit)
            {
                emit signal_disp_result(1,1);
            }
            else
            {
                emit signal_disp_result(1,2);
            }
            difference1=0;
        }
    }
    else
    {
        //both of two cameras,model path is equal to model_path[0]
        qDebug()<<"image detection 1 no rotation";
        AreaCenter(*(Region.begin()),&Area,&Row,&Column);
        MoveRegion(*(Region.begin()),&RegionMoved,-Row,-Column);
        FindScaledShapeModel(ho_ImageMedian, *(MODEL.begin()), 0, 6.29, 0.1, 10, 0.5, 1, 0.5, "least_squares",
            0, 0.9, &hv_Row1, &hv_Column1, &hv_Angle1, &Scale1, &hv_Score1);
        if(hv_Score1.Length()<=0)
        {
            Interruption=true;
            difference1=0;
            emit signal_disp_result(1,2);
            return;
        }
        VectorAngleToRigid(0,0,0,hv_Row1,hv_Column1,hv_Angle1,&HomMat2D);
        AffineTransRegion(RegionMoved,&DetectionRegion,HomMat2D,"nearest_neighbor");
        ReduceDomain(ho_ImageMedian,DetectionRegion,&ImageDetect);

        FastThreshold(ImageDetect, &ho_ThresholdRegion, 128, 255, 20);
        Connection(ho_ThresholdRegion, &ho_ConnectedRegions);
        SelectShape(ho_ConnectedRegions, &ho_SelectedRegions, "area", "and", 150, 10e4);
        Union1(ho_SelectedRegions, &DetectionRegion);
        ReduceDomain(ImageDetect,DetectionRegion,&ImageDetect);

        SubImage(*(StandardImage.begin()),ImageDetect,&ImageSub,1,0);
        Threshold(ImageSub,&RegionSub,100,255);
        AreaCenter(RegionSub,&AreaSub,&RowSub,&ColumnSub);
        difference1=difference1+AreaSub;
        if(difference1<limit)
        {
            emit signal_disp_result(1,1);
        }
        else
        {
            emit signal_disp_result(1,2);
        }
        difference1=0;
    }
}

void Running::slot_detection_image2(HObject image)
{
    qDebug()<<"image detection 2";
    model_path[1];
}
