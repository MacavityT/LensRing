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

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    //initializer the program
    ui->setupUi(this);
    setWindowTitle(tr("Control System"));
    qRegisterMetaType<DWORD>("DWORD");

    configFile=new QSettings(".\\Lens-Ring\\Temporary_File.ini",QSettings::IniFormat);
    ic_cap=new IC_Capture;
    sen=new Sensor;

    //connect
    connect(this,SIGNAL(signal_action_enable(bool)),ic_cap,SLOT(slot_action_enable(bool)));

    connect(ic_cap,SIGNAL(signal_open_Camera(bool)),this,SLOT(slot_open_Camera(bool)));

    //set button enable
    QList<QAbstractButton*> list=ui->Control_Buttons->buttons();
    QList<QAbstractButton*>::iterator i;
    for(i=list.begin();i!=list.end();++i)
    {
        QAbstractButton *temporary_button=*i;
        temporary_button->setEnabled(false);
    }
//initialization the card and it's parameters
    ControlCard_Initialization();
    disp_path();
    if(board_initialization)
    {
//        sen->start();
        ic_cap->start();
    }
}

MainWindow::~MainWindow()
{
    delete ui;
    //the configFile pointer
    configFile->deleteLater();
    //sensor thread
    sen->terminate();
    sen->wait(1);
    sen->deleteLater();
    //image captuer thread
    ic_cap->terminate();
    ic_cap->wait(1);
    ic_cap->deleteLater();
    //close the control card
    d1000_board_close();
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
        return "Empty Model";
    }
}

void MainWindow::disp_path()
{
    ui->disp_path1->setText(configFile->value("Model_path/model1").toString());
    ui->disp_path2->setText(configFile->value("Model_path/model2").toString());
    ui->disp_path3->setText(configFile->value("Model_path/model3").toString());
    ui->disp_path4->setText(configFile->value("Model_path/model4").toString());
    ui->disp_path5->setText(configFile->value("Model_path/model5").toString());
    ui->disp_path6->setText(configFile->value("Model_path/model6").toString());
    ui->disp_path7->setText(configFile->value("Model_path/model7").toString());
    ui->disp_path8->setText(configFile->value("Model_path/model8").toString());
    ui->disp_path9->setText(configFile->value("Model_path/model9").toString());
    ui->disp_path10->setText(configFile->value("Model_path/model10").toString());
}

void MainWindow::on_load_model_1_clicked()
{
    QString tmp=load_model();
    ui->disp_path1->setText(tmp);
    configFile->setValue("Model_path/model1",tmp);
}

void MainWindow::on_load_model_2_clicked()
{
    QString tmp=load_model();
    ui->disp_path2->setText(tmp);
    configFile->setValue("Model_path/model2",tmp);
}

void MainWindow::on_load_model_3_clicked()
{
    QString tmp=load_model();
    ui->disp_path3->setText(tmp);
    configFile->setValue("Model_path/model3",tmp);
}

void MainWindow::on_load_model_4_clicked()
{
    QString tmp=load_model();
    ui->disp_path4->setText(tmp);
    configFile->setValue("Model_path/model4",tmp);
}

void MainWindow::on_load_model_5_clicked()
{
    QString tmp=load_model();
    ui->disp_path5->setText(tmp);
    configFile->setValue("Model_path/model5",tmp);
}

void MainWindow::on_load_model_6_clicked()
{
    QString tmp=load_model();
    ui->disp_path6->setText(tmp);
    configFile->setValue("Model_path/model6",tmp);
}

void MainWindow::on_load_model_7_clicked()
{
    QString tmp=load_model();
    ui->disp_path7->setText(tmp);
    configFile->setValue("Model_path/model7",tmp);
}

void MainWindow::on_load_model_8_clicked()
{
    QString tmp=load_model();
    ui->disp_path8->setText(tmp);
    configFile->setValue("Model_path/model8",tmp);
}

void MainWindow::on_load_model_9_clicked()
{
    QString tmp=load_model();
    ui->disp_path9->setText(tmp);
    configFile->setValue("Model_path/model9",tmp);
}

void MainWindow::on_load_model_10_clicked()
{
    QString tmp=load_model();
    ui->disp_path10->setText(tmp);
    configFile->setValue("Model_path/model10",tmp);
}


/////UI Control function
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

void MainWindow::slot_open_Camera(bool y_n)
{
    qDebug()<<"display error messages";
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
    NewDialog->setAttribute(Qt::WA_DeleteOnClose);//let the dialog auto delete when click the X.
    NewDialog->setAttribute(Qt::WA_QuitOnClose,false);//let the dialog close with mianwindow exit.
    NewDialog->show();
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

void MainWindow::on_START_clicked()
{
    emit signal_action_enable(true);
    if(First_Start)
    {
        First_Start=false;
        ALL_Origin_Back();
    }
    //move to position of detection
}

void MainWindow::on_PAUSE_clicked()
{

}

void MainWindow::on_RESUME_clicked()
{

}

void MainWindow::on_STOP_clicked()
{

}

void MainWindow::on_RESET_clicked()
{

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
        QMessageBox::information(this,tr("Warning"),tr("Initialization Failed!"),tr("OK"),0);
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

void MainWindow::slot_disp_image(HObject image)
{
}

void MainWindow::slot_disp_image1(HObject image)
{}

void MainWindow::slot_detection_image(HObject image)
{}

void MainWindow::slot_detection_image1(HObject image)
{}
