#include "string"

struct preface{
    std::string tantai_yan="The most handsome boy!";
    std::string author=tantai_yan;
    std::string lao_zhang="Sha bi";
    std::string Zhang_Junjie=lao_zhang;
    std::string date="20170412";
};

//                       ::
//                      :;J7, :,                        ::;7:
//                      ,ivYi, ,                       ;LLLFS:
//                      :iv7Yi                       :7ri;j5PL
//                     ,:ivYLvr                    ,ivrrirrY2X,
//                     :;r@Wwz.7r:                :ivu@kexianli.
//                    :iL7::,:::iiirii:ii;::::,,irvF7rvvLujL7ur
//                   ri::,:,::i:iiiiiii:i:irrv177JX7rYXqZEkvv17
//                ;i:, , ::::iirrririi:i:::iiir2XXvii;L8OGJr71i
//              :,, ,,:   ,::ir@mingyi.irii:i:::j1jri7ZBOS7ivv,
//                 ,::,    ::rv77iiiriii:iii:i::,rvLq@huhao.Li
//             ,,      ,, ,:ir7ir::,:::i;ir:::i:i::rSGGYri712:
//           :::  ,v7r:: ::rrv77:, ,, ,:i7rrii:::::, ir7ri7Lri
//          ,     2OBBOi,iiir;r::        ,irriiii::,, ,iv7Luur:
//        ,,     i78MBBi,:,:::,:,  :7FSL: ,iriii:::i::,,:rLqXv::
//        :      iuMMP: :,:::,:ii;2GY7OBB0viiii:i:iii:i:::iJqL;::
//       ,     ::::i   ,,,,, ::LuBBu BBBBBErii:i:i:i:i:i:i:r77ii
//      ,       :       , ,,:::rruBZ1MBBqi, :,,,:::,::::::iiriri:
//     ,               ,,,,::::i:  @arqiao.       ,:,, ,:::ii;i7:
//    :,       rjujLYLi   ,,:::::,:::::::::,,   ,:i,:,,,,,::i:iii
//    ::      BBBBBBBBB0,    ,,::: , ,:::::: ,      ,,,, ,,:::::::
//    i,  ,  ,8BMMBBBBBBi     ,,:,,     ,,, , ,   , , , :,::ii::i::
//    :      iZMOMOMBBM2::::::::::,,,,     ,,,,,,:,,,::::i:irr:i:::,
//    i   ,,:;u0MBMOG1L:::i::::::  ,,,::,   ,,, ::::::i:i:iirii:i:i:
//    :    ,iuUuuXUkFu7i:iii:i:::, :,:,: ::::::::i:i:::::iirr7iiri::
//    :     :rk@Yizero.i:::::, ,:ii:::::::i:::::i::,::::iirrriiiri::,
//     :      5BMBBBBBBSr:,::rv2kuii:::iii::,:i:,, , ,,:,:i@petermu.,
//          , :r50EZ8MBBBBGOBBBZP7::::i::,:::::,: :,:,::i;rrririiii::
//              :jujYY7LS0ujJL7r::,::i::,::::::::::::::iirirrrrrrr:ii:
//           ,:  :@kevensun.:,:,,,::::i:i:::::,,::::::iir;ii;7v77;ii;i,
//           ,,,     ,,:,::::::i:iiiii:i::::,, ::::iiiir@xingjief.r;7:i,
//        , , ,,,:,,::::::::iiiiiiiiii:,:,:::::::::iiir;ri7vL77rrirri::
//         :,, , ::::::::i:::i:::i:i::,,,,,:,::i:i:::iir;@Secbone.ii:::



//                          神兽护体！永无BUG!
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    //All sql drivers output
    qDebug()<<"drivers:";
    QStringList drivers=QSqlDatabase::drivers();
    foreach(QString driver,drivers)  //遍历所支持的数据库驱动
        qDebug()<<driver;
    //Qt serial information output.
    foreach (const QSerialPortInfo &info, QSerialPortInfo::availablePorts())
    {
        qDebug() << "Name : " << info.portName();
        qDebug() << "Description : " << info.description();
        qDebug() << "Manufacturer: " << info.manufacturer();
        qDebug() << "Serial Number: " << info.serialNumber();
        qDebug() << "System Location: " << info.systemLocation();
    }
    //initializer the program
    ui->setupUi(this);
    setWindowTitle(tr("Control System"));
    qRegisterMetaType<DWORD>("DWORD");
    qRegisterMetaType<HObject>("HObject");
    qRegisterMetaType<HTuple>("HTuple");

    configFile=new QSettings(".\\Lens-Ring\\Parameters_Setting.ini",QSettings::IniFormat);
    configFile1=new QSettings(".\\Lens-Ring\\Temporary_File.ini",QSettings::IniFormat);
    ic_cap=new IC_Capture;
    run=new Running;

    //connect ic_cap thread and mainwindow
    connect(ic_cap,SIGNAL(signal_open_Camera(bool)),this,SLOT(slot_open_Camera(bool)));
    connect(ic_cap,SIGNAL(signal_disp_image1(HObject)),this,SLOT(slot_disp_image1(HObject)));
    connect(ic_cap,SIGNAL(signal_disp_image2(HObject)),this,SLOT(slot_disp_image2(HObject)));
    //connect all the buttons with enable function
    QList<QAction *> select_part=ui->menu_2->actions();
    QList<QAction *>::iterator iter;
    for(iter=select_part.begin();iter!=select_part.end();++iter)
    {
        QAction *action=*iter;
        //if the action is checkable,
        //checked is true if the action is checked, or false if the action is unchecked.
        connect(action,SIGNAL(triggered(bool)),SLOT(lock_all_buttons(bool)));
    }
    //connect running thread and mainwindow
    connect(this,SIGNAL(signal_part_select(int)),run,SLOT(get_config_param(int)));
    connect(this,SIGNAL(reload_parameters(int)),run,SLOT(get_config_param(int)));
    connect(this,SIGNAL(signal_read_model(int)),run,SLOT(slot_read_model(int)));
    connect(this,SIGNAL(signal_reset()),run,SLOT(slot_reset()));

    //set progressbar
    ui->progressBar->setMinimum(0);
    ui->progressBar->setMaximum(100);
    ui->progressBar->setValue(0);
    ui->progressBar->setFormat("检测进度：%p%");
    //control buttons
    lock_all_buttons(true);//true is to lock all buttons
//initialization the card and it's parameters
    ControlCard_Initialization();
    disp_path();
    ic_cap->start();
    if(board_initialization)
    {
        run->start();
    }
}

MainWindow::~MainWindow()
{
    delete ui;
    //the configFile1 pointer
    configFile->deleteLater();
    configFile1->deleteLater();
    //Running thread
    run->terminate();
    run->wait(1);
    run->deleteLater();
    //image captuer thread
    ic_cap->terminate();
    ic_cap->wait(1);
    ic_cap->deleteLater();
    //close the control card
    d1000_board_close();
}

void MainWindow::closeEvent( QCloseEvent * event )
{
  switch( QMessageBox::information( this, tr("Control System"),tr("Really Exit?"),\
          tr("Yes"), tr("No"),0, 1 ) ) //  the symbol "\" is a sign to shift the index and line feed
{
  case 0:
      event->accept();
      break;
  case 1:
      event->ignore();
      break;
  default:
      event->ignore();
      break;
}
}

/////Load Halcon Model
QString MainWindow::load_model()
{
    QFileDialog *fileDialog=new QFileDialog(this);
    fileDialog->setWindowTitle(tr("保存模板"));
    //set defalut path
    fileDialog->setDirectory("./");
    //file name fliter
    fileDialog->setNameFilter(tr("Images(*.png *.jpg *.jpeg *.bmp)"));
    //set view mode
    fileDialog->setViewMode(QFileDialog::Detail);

//    //set the number could be selected,QFileDialog::ExistingFiles; The default is one
//    fileDialog->setFileMode(QFileDialog::ExistingFiles);
//    //print all file name
//    QStringList fileNames;
//    if(fileDialog->exec())
//    {
//        fileNames = fileDialog->selectedFiles();
//    }
//    for(auto tmp:fileNames)
//        qDebug()<<tmp<<endl;

    QStringList fileName;
    if(fileDialog->exec())
    {
        fileName=fileDialog->selectedFiles();
        QString tmp=*fileName.begin();
        return tmp;
    }
    else
    {
        return "";
    }
}

void MainWindow::disp_path()
{
    ui->disp_path1->setText(configFile1->value("Model_path/model1").toString());
    ui->disp_path2->setText(configFile1->value("Model_path/model2").toString());
    ui->disp_path3->setText(configFile1->value("Model_path/model3").toString());
    ui->disp_path4->setText(configFile1->value("Model_path/model4").toString());
    ui->disp_path5->setText(configFile1->value("Model_path/model5").toString());
    ui->disp_path6->setText(configFile1->value("Model_path/model6").toString());
    ui->disp_path7->setText(configFile1->value("Model_path/model7").toString());
    ui->disp_path8->setText(configFile1->value("Model_path/model8").toString());
    ui->disp_path9->setText(configFile1->value("Model_path/model9").toString());
    ui->disp_path10->setText(configFile1->value("Model_path/model10").toString());
}

void MainWindow::on_load_model_1_clicked()
{
    QString tmp=load_model();
    ui->disp_path1->setText(tmp);
    if(tmp!="")
    {
        configFile1->setValue("Model_path/model1",tmp);
        emit signal_read_model(1);
    }
    else
    {
        configFile1->remove("Model_path/model1");
    }
}

void MainWindow::on_load_model_2_clicked()
{
    QString tmp=load_model();
    ui->disp_path2->setText(tmp);
    if(tmp!="")
    {
        configFile1->setValue("Model_path/model2",tmp);
        emit signal_read_model(2);
    }
    else
    {
        configFile1->remove("Model_path/model2");
    }
}

void MainWindow::on_load_model_3_clicked()
{
    QString tmp=load_model();
    ui->disp_path3->setText(tmp);
    if(tmp!="")
    {
        configFile1->setValue("Model_path/model3",tmp);
        emit signal_read_model(3);
    }
    else
    {
        configFile1->remove("Model_path/model3");
    }
}

void MainWindow::on_load_model_4_clicked()
{
    QString tmp=load_model();
    ui->disp_path4->setText(tmp);
    if(tmp!="")
    {
        configFile1->setValue("Model_path/model4",tmp);
        emit signal_read_model(4);
    }
    else
    {
        configFile1->remove("Model_path/model4");
    }
}

void MainWindow::on_load_model_5_clicked()
{
    QString tmp=load_model();
    ui->disp_path5->setText(tmp);
    if(tmp!="")
    {
        configFile1->setValue("Model_path/model5",tmp);
        emit signal_read_model(5);
    }
    else
    {
        configFile1->remove("Model_path/model5");
    }
}

void MainWindow::on_load_model_6_clicked()
{
    QString tmp=load_model();
    ui->disp_path6->setText(tmp);
    if(tmp!="")
    {
        configFile1->setValue("Model_path/model6",tmp);
        emit signal_read_model(6);
    }
    else
    {
        configFile1->remove("Model_path/model6");
    }
}

void MainWindow::on_load_model_7_clicked()
{
    QString tmp=load_model();
    ui->disp_path7->setText(tmp);
    if(tmp!="")
    {
        configFile1->setValue("Model_path/model7",tmp);
        emit signal_read_model(7);
    }
    else
    {
        configFile1->remove("Model_path/model7");
    }
}

void MainWindow::on_load_model_8_clicked()
{
    QString tmp=load_model();
    ui->disp_path8->setText(tmp);
    if(tmp!="")
    {
        configFile1->setValue("Model_path/model8",tmp);
        emit signal_read_model(8);
    }
    else
    {
        configFile1->remove("Model_path/model8");
    }
}

void MainWindow::on_load_model_9_clicked()
{
    QString tmp=load_model();
    ui->disp_path9->setText(tmp);
    if(tmp!="")
    {
        configFile1->setValue("Model_path/model9",tmp);
        emit signal_read_model(9);
    }
    else
    {
        configFile1->remove("Model_path/model9");
    }
}

void MainWindow::on_load_model_10_clicked()
{
    QString tmp=load_model();
    ui->disp_path10->setText(tmp);
    if(tmp!="")
    {
        configFile1->setValue("Model_path/model10",tmp);
        emit signal_read_model(10);
    }
    else
    {
        configFile1->remove("Model_path/model10");
    }
}


/////UI Control function

void MainWindow::slot_open_Camera(bool y_n)
{
    if(y_n)
    {
        ui->statusBar->showMessage(tr("打开相机成功"));
    }
    else
    {
        ui->statusBar->showMessage(tr("打开相机失败"));
    }
}

void MainWindow::on_actionCMD_triggered()
{
    auto NewDialog = new CMD();
    connect(ic_cap,SIGNAL(signal_disp_image3(HObject)),NewDialog,SLOT(slot_disp_image1(HObject)));
    connect(ic_cap,SIGNAL(signal_disp_image4(HObject)),NewDialog,SLOT(slot_disp_image2(HObject)));
    NewDialog->setAttribute(Qt::WA_DeleteOnClose);//let the dialog auto delete when click the X.
    NewDialog->setAttribute(Qt::WA_QuitOnClose,false);//let the dialog close with mianwindow exit.
    NewDialog->show();
//    qDebug()<<"call triggered cmd";
}

void MainWindow::on_actionPMD_triggered()
{
    auto NewDialog = new pmd();
    NewDialog->setAttribute(Qt::WA_DeleteOnClose);//let the dialog auto delete when click the X.
    NewDialog->setAttribute(Qt::WA_QuitOnClose,false);//let the dialog close with mianwindow exit.
    NewDialog->show();
}

void MainWindow::on_actionMODEL_triggered()
{
    auto NewDialog = new MODEL();
    NewDialog->setAttribute(Qt::WA_DeleteOnClose);//let the dialog auto delete when click the X.
    NewDialog->setAttribute(Qt::WA_QuitOnClose,false);//let the dialog close with mianwindow exit.
    NewDialog->show();
}

void MainWindow::on_actionPART1_triggered()
{
    disp_present_path(1);
    emit signal_part_select(1);
    present_part=1;
}

void MainWindow::on_actionPART2_triggered()
{
    disp_present_path(2);
    emit signal_part_select(2);
    present_part=2;
}

void MainWindow::on_actionPART3_triggered()
{
    disp_present_path(3);
    emit signal_part_select(3);
    present_part=3;
}

void MainWindow::on_actionPART4_triggered()
{
    disp_present_path(4);
    emit signal_part_select(4);
    present_part=4;
}

void MainWindow::on_actionPART5_triggered()
{
    disp_present_path(5);
    emit signal_part_select(5);
    present_part=5;
}

void MainWindow::disp_present_path(int num)
{
    QString part_num=num+'0';
    QString part_name=configFile->value("Part_Name/part"+part_num+"_name").toString();
    QString part="部品";
    part+=part_num+":";
    ui->Part_Num->setText(part+part_name);
}

void MainWindow::lock_all_buttons(bool YN)
{
    //set button enable
    QList<QAbstractButton*> list=ui->Control_Buttons->buttons();
    QList<QAbstractButton*>::iterator i;
    for(i=list.begin();i!=list.end();++i)
    {
        QAbstractButton *temporary_button=*i;
        temporary_button->setEnabled(!YN);
    }
}

void MainWindow::on_START_clicked()
{
    ic_cap->start();
    ic_cap->action_enable=true;
//    ic_cap->cmd_cap=false;
    if(First_Start)
    {
        emit signal_reset();
        First_Start=false;
    }
    emit reload_parameters(present_part);
    //move to position of detection
}

void MainWindow::on_PAUSE_clicked()
{
    for(int i=0;i<3;i++)
    {
        if(d1000_check_done(i)==0)
        {
            d1000_immediate_stop(i);
        }
    }
}

void MainWindow::on_RESUME_clicked()
{

}

void MainWindow::on_STOP_clicked()
{
    ic_cap->action_enable=false;
    for(int i=0;i<3;i++)
    {
        if(d1000_check_done(i)==0)
        {
            d1000_immediate_stop(i);
        }
    }
}

void MainWindow::on_RESET_clicked()
{
    emit signal_reset();
}


//////////Control Card Function
///
void MainWindow::ControlCard_Initialization()
{
    int k=0;
    //initialize the control card
    if( d1000_board_init()<=0)
    {
        app->beep();
//        QMessageBox::information(this,tr("Warning"),tr("Initialization Failed!"),tr("OK"),0);
//        this->setEnabled(false);
        board_initialization=false;
        return;
    }
    //set the pulse mode
    for(k=0;k<2;k++)
    {
       d1000_set_pls_outmode(k,1);//pulse and direction
    }
}

void MainWindow::ALL_Origin_Back()
{}


///////////Image detection function

void MainWindow::slot_disp_image1(HObject image)
{
    if(First_OpenWindow1)
    {
        HTuple hv_Width, hv_Height;
        GetImageSize(image, &hv_Width, &hv_Height);
        SetWindowAttr("background_color","black");
        Hlong winID= this->winId();
        OpenWindow(85,70,430,310,winID,"visible","",&hv_WindowHandle1);
        SetPart(hv_WindowHandle1, 0, 0, hv_Height, hv_Width);
        First_OpenWindow1=false;
    }
    HDevWindowStack::Push(hv_WindowHandle1);
    if (HDevWindowStack::IsOpen())
      DispObj(image, HDevWindowStack::GetActive());
}

void MainWindow::slot_disp_image2(HObject image)
{
    if(First_OpenWindow2)
    {
        HTuple hv_Width, hv_Height;
        GetImageSize(image, &hv_Width, &hv_Height);
        SetWindowAttr("background_color","black");
        Hlong winID= this->winId();
        OpenWindow(85,850,430,310,winID,"visible","",&hv_WindowHandle2);
        SetPart(hv_WindowHandle2, 0, 0, hv_Height, hv_Width);
        First_OpenWindow2=false;
    }
    HDevWindowStack::Push(hv_WindowHandle2);
    if (HDevWindowStack::IsOpen())
      DispObj(image, HDevWindowStack::GetActive());
}

void MainWindow::slot_detection_image1(HObject image)
{}

void MainWindow::slot_detection_image2(HObject image)
{}
