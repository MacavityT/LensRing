
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
    qRegisterMetaType<DWORD>("DWORD");

    ic_cap=new IC_Capture;
    sen=new Sensor;

    //connect
    connect(this,SIGNAL(signal_action_enable(bool)),ic_cap,SLOT(slot_action_enable(bool)));

    connect(ic_cap,SIGNAL(signal_open_Camera(bool)),this,SLOT(slot_open_Camera(bool)));

//initialization the card and it's parameters
    ControlCard_Initialization();
    if(board_initialization)
    {
        sen->start();
        ic_cap->start();
    }
}

MainWindow::~MainWindow()
{
    delete ui;
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
        this->setEnabled(false);
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




