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
            while(index!=total_position)
            {
                single_axis_action_absolute(0,rotation[index]);
                single_axis_check_done(0,rotation[index]);
                detection_number=index;
                while(detection==1)
                {}
                detection=1;
                index++;
            }
            single_origin_back(0);
        }
        ///single camera(2) is used
        if(total_position==0&&detection==0&&rise_edge2&&model_path.length()==1&&\
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
    model_path.insert(i-1,configFile1->value("Model_path/model"+num).toString());
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
            model_path.insert(i-1,configFile1->value("Model_path/model"+index).toString());
            model_number=i;
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
    //move to detection position
    d1000_start_ta_move(1,camera[0],speed_set[1][0],speed_set[1][1],speed_set[1][2]);
    d1000_start_ta_move(2,camera[1],speed_set[2][0],speed_set[2][1],speed_set[2][2]);
    all_axis_check_done();
    //set flag
    reset_finished=true;
    detection_position_arrive=true;
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
    if(detection_number>=0)
    {
        //single camera 1 , need to rotate
        qDebug()<<"image detection 1 rotatation";
        model_path[detection_number];//detection_number start with 0,model ID should equal to this
        if(detection_number!=total_position)
        {
            emit signal_disp_result(1,3);
        }
        else
        {
            emit signal_disp_result(1,1);
        }
    }
    else
    {
        //both of two cameras,model path is equal to model_path[0]
        qDebug()<<"image detection 1 no rotation";
        model_path[0];
    }
}

void Running::slot_detection_image2(HObject image)
{
    qDebug()<<"image detection 2";
    model_path[1];
}
