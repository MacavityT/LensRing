#include "pmd.h"
#include "ui_pmd.h"

pmd::pmd(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::pmd)
{
    ui->setupUi(this);
    setWindowTitle(tr("参数设置"));
    configFile=new QSettings(".\\Lens-Ring\\Parameters_Setting.ini",QSettings::IniFormat);
    disp_configure_parameter();
    lock_ui(true);
}

pmd::~pmd()
{
    delete ui;
    configFile->deleteLater();
}

//Configure file parameters initializer.
void pmd::set_configure_parameter()
{
    //Part 1
    configFile->setValue("Part_Name/part1_name",ui->Part1_name->text());
    configFile->setValue("Detection_Position/part1_camera1",ui->part1_camera1->text());
    configFile->setValue("Detection_Position/part1_camera2",ui->part1_camera2->text());
    configFile->setValue("Detection_Position/part1_rotation1",ui->part1_rotation1->text());
    configFile->setValue("Detection_Position/part1_rotation2",ui->part1_rotation2->text());
    configFile->setValue("Detection_Position/part1_rotation3",ui->part1_rotation3->text());
    configFile->setValue("Detection_Position/part1_rotation4",ui->part1_rotation4->text());
    configFile->setValue("Detection_Position/part1_rotation5",ui->part1_rotation5->text());
    configFile->setValue("Detection_Position/part1_rotation6",ui->part1_rotation6->text());
    configFile->setValue("Detection_Position/part1_rotation7",ui->part1_rotation7->text());
    configFile->setValue("Detection_Position/part1_rotation8",ui->part1_rotation8->text());
    configFile->setValue("Detection_Position/part1_rotation9",ui->part1_rotation9->text());
    configFile->setValue("Detection_Position/part1_rotation10",ui->part1_rotation10->text());
    //Part 2
    configFile->setValue("Part_Name/part2_name",ui->Part2_name->text());
    configFile->setValue("Detection_Position/part2_camera1",ui->part2_camera1->text());
    configFile->setValue("Detection_Position/part2_camera2",ui->part2_camera2->text());
    configFile->setValue("Detection_Position/part2_rotation1",ui->part2_rotation1->text());
    configFile->setValue("Detection_Position/part2_rotation2",ui->part2_rotation2->text());
    configFile->setValue("Detection_Position/part2_rotation3",ui->part2_rotation3->text());
    configFile->setValue("Detection_Position/part2_rotation4",ui->part2_rotation4->text());
    configFile->setValue("Detection_Position/part2_rotation5",ui->part2_rotation5->text());
    configFile->setValue("Detection_Position/part2_rotation6",ui->part2_rotation6->text());
    configFile->setValue("Detection_Position/part2_rotation7",ui->part2_rotation7->text());
    configFile->setValue("Detection_Position/part2_rotation8",ui->part2_rotation8->text());
    configFile->setValue("Detection_Position/part2_rotation9",ui->part2_rotation9->text());
    configFile->setValue("Detection_Position/part2_rotation10",ui->part2_rotation10->text());
    //Part 3
    configFile->setValue("Part_Name/part3_name",ui->Part3_name->text());
    configFile->setValue("Detection_Position/part3_camera1",ui->part3_camera1->text());
    configFile->setValue("Detection_Position/part3_camera2",ui->part3_camera2->text());
    configFile->setValue("Detection_Position/part3_rotation1",ui->part3_rotation1->text());
    configFile->setValue("Detection_Position/part3_rotation2",ui->part3_rotation2->text());
    configFile->setValue("Detection_Position/part3_rotation3",ui->part3_rotation3->text());
    configFile->setValue("Detection_Position/part3_rotation4",ui->part3_rotation4->text());
    configFile->setValue("Detection_Position/part3_rotation5",ui->part3_rotation5->text());
    configFile->setValue("Detection_Position/part3_rotation6",ui->part3_rotation6->text());
    configFile->setValue("Detection_Position/part3_rotation7",ui->part3_rotation7->text());
    configFile->setValue("Detection_Position/part3_rotation8",ui->part3_rotation8->text());
    configFile->setValue("Detection_Position/part3_rotation9",ui->part3_rotation9->text());
    configFile->setValue("Detection_Position/part3_rotation10",ui->part3_rotation10->text());
    //Part 4
    configFile->setValue("Part_Name/part4_name",ui->Part4_name->text());
    configFile->setValue("Detection_Position/part4_camera1",ui->part4_camera1->text());
    configFile->setValue("Detection_Position/part4_camera2",ui->part4_camera2->text());
    configFile->setValue("Detection_Position/part4_rotation1",ui->part4_rotation1->text());
    configFile->setValue("Detection_Position/part4_rotation2",ui->part4_rotation2->text());
    configFile->setValue("Detection_Position/part4_rotation3",ui->part4_rotation3->text());
    configFile->setValue("Detection_Position/part4_rotation4",ui->part4_rotation4->text());
    configFile->setValue("Detection_Position/part4_rotation5",ui->part4_rotation5->text());
    configFile->setValue("Detection_Position/part4_rotation6",ui->part4_rotation6->text());
    configFile->setValue("Detection_Position/part4_rotation7",ui->part4_rotation7->text());
    configFile->setValue("Detection_Position/part4_rotation8",ui->part4_rotation8->text());
    configFile->setValue("Detection_Position/part4_rotation9",ui->part4_rotation9->text());
    configFile->setValue("Detection_Position/part4_rotation10",ui->part4_rotation10->text());
    //Part 5
    configFile->setValue("Part_Name/part5_name",ui->Part5_name->text());
    configFile->setValue("Detection_Position/part5_camera1",ui->part5_camera1->text());
    configFile->setValue("Detection_Position/part5_camera2",ui->part5_camera2->text());
    configFile->setValue("Detection_Position/part5_rotation1",ui->part5_rotation1->text());
    configFile->setValue("Detection_Position/part5_rotation2",ui->part5_rotation2->text());
    configFile->setValue("Detection_Position/part5_rotation3",ui->part5_rotation3->text());
    configFile->setValue("Detection_Position/part5_rotation4",ui->part5_rotation4->text());
    configFile->setValue("Detection_Position/part5_rotation5",ui->part5_rotation5->text());
    configFile->setValue("Detection_Position/part5_rotation6",ui->part5_rotation6->text());
    configFile->setValue("Detection_Position/part5_rotation7",ui->part5_rotation7->text());
    configFile->setValue("Detection_Position/part5_rotation8",ui->part5_rotation8->text());
    configFile->setValue("Detection_Position/part5_rotation9",ui->part5_rotation9->text());
    configFile->setValue("Detection_Position/part5_rotation10",ui->part5_rotation10->text());
    //Running Speed.
    configFile->setValue("Running_Speed/Speed_Initial0",ui->Speed_Initial0->text());
    configFile->setValue("Running_Speed/Speed_Set0",ui->Speed_Set0->text());
    configFile->setValue("Running_Speed/Acceleration0",ui->Acceleration0->text());

    configFile->setValue("Running_Speed/Speed_Initial1",ui->Speed_Initial1->text());
    configFile->setValue("Running_Speed/Speed_Set1",ui->Speed_Set1->text());
    configFile->setValue("Running_Speed/Acceleration1",ui->Acceleration1->text());

    configFile->setValue("Running_Speed/Speed_Initial2",ui->Speed_Initial2->text());
    configFile->setValue("Running_Speed/Speed_Set2",ui->Speed_Set2->text());
    configFile->setValue("Running_Speed/Acceleration2",ui->Acceleration2->text());
}

//Configure file parameters show.
void pmd::disp_configure_parameter()
{
    //Part 1
    ui->Part1_name->setText(configFile->value("Part_Name/part1_name").toString());
    ui->part1_camera1->setText(configFile->value("Detection_Position/part1_camera1").toString());
    ui->part1_camera2->setText(configFile->value("Detection_Position/part1_camera2").toString());
    ui->part1_rotation1->setText(configFile->value("Detection_Position/part1_rotation1").toString());
    ui->part1_rotation2->setText(configFile->value("Detection_Position/part1_rotation2").toString());
    ui->part1_rotation3->setText(configFile->value("Detection_Position/part1_rotation3").toString());
    ui->part1_rotation4->setText(configFile->value("Detection_Position/part1_rotation4").toString());
    ui->part1_rotation5->setText(configFile->value("Detection_Position/part1_rotation5").toString());
    ui->part1_rotation6->setText(configFile->value("Detection_Position/part1_rotation6").toString());
    ui->part1_rotation7->setText(configFile->value("Detection_Position/part1_rotation7").toString());
    ui->part1_rotation8->setText(configFile->value("Detection_Position/part1_rotation8").toString());
    ui->part1_rotation9->setText(configFile->value("Detection_Position/part1_rotation9").toString());
    ui->part1_rotation10->setText(configFile->value("Detection_Position/part1_rotation10").toString());
    //Part 2
    ui->Part2_name->setText(configFile->value("Part_Name/part2_name").toString());
    ui->part2_camera1->setText(configFile->value("Detection_Position/part2_camera1").toString());
    ui->part2_camera2->setText(configFile->value("Detection_Position/part2_camera2").toString());
    ui->part2_rotation1->setText(configFile->value("Detection_Position/part2_rotation1").toString());
    ui->part2_rotation2->setText(configFile->value("Detection_Position/part2_rotation2").toString());
    ui->part2_rotation3->setText(configFile->value("Detection_Position/part2_rotation3").toString());
    ui->part2_rotation4->setText(configFile->value("Detection_Position/part2_rotation4").toString());
    ui->part2_rotation5->setText(configFile->value("Detection_Position/part2_rotation5").toString());
    ui->part2_rotation6->setText(configFile->value("Detection_Position/part2_rotation6").toString());
    ui->part2_rotation7->setText(configFile->value("Detection_Position/part2_rotation7").toString());
    ui->part2_rotation8->setText(configFile->value("Detection_Position/part2_rotation8").toString());
    ui->part2_rotation9->setText(configFile->value("Detection_Position/part2_rotation9").toString());
    ui->part2_rotation10->setText(configFile->value("Detection_Position/part2_rotation10").toString());
    //Part 3
    ui->Part3_name->setText(configFile->value("Part_Name/part3_name").toString());
    ui->part3_camera1->setText(configFile->value("Detection_Position/part3_camera1").toString());
    ui->part3_camera2->setText(configFile->value("Detection_Position/part3_camera2").toString());
    ui->part3_rotation1->setText(configFile->value("Detection_Position/part3_rotation1").toString());
    ui->part3_rotation2->setText(configFile->value("Detection_Position/part3_rotation2").toString());
    ui->part3_rotation3->setText(configFile->value("Detection_Position/part3_rotation3").toString());
    ui->part3_rotation4->setText(configFile->value("Detection_Position/part3_rotation4").toString());
    ui->part3_rotation5->setText(configFile->value("Detection_Position/part3_rotation5").toString());
    ui->part3_rotation6->setText(configFile->value("Detection_Position/part3_rotation6").toString());
    ui->part3_rotation7->setText(configFile->value("Detection_Position/part3_rotation7").toString());
    ui->part3_rotation8->setText(configFile->value("Detection_Position/part3_rotation8").toString());
    ui->part3_rotation9->setText(configFile->value("Detection_Position/part3_rotation9").toString());
    ui->part3_rotation10->setText(configFile->value("Detection_Position/part3_rotation10").toString());
    //Part 4
    ui->Part4_name->setText(configFile->value("Part_Name/part4_name").toString());
    ui->part4_camera1->setText(configFile->value("Detection_Position/part4_camera1").toString());
    ui->part4_camera2->setText(configFile->value("Detection_Position/part4_camera2").toString());
    ui->part4_rotation1->setText(configFile->value("Detection_Position/part4_rotation1").toString());
    ui->part4_rotation2->setText(configFile->value("Detection_Position/part4_rotation2").toString());
    ui->part4_rotation3->setText(configFile->value("Detection_Position/part4_rotation3").toString());
    ui->part4_rotation4->setText(configFile->value("Detection_Position/part4_rotation4").toString());
    ui->part4_rotation5->setText(configFile->value("Detection_Position/part4_rotation5").toString());
    ui->part4_rotation6->setText(configFile->value("Detection_Position/part4_rotation6").toString());
    ui->part4_rotation7->setText(configFile->value("Detection_Position/part4_rotation7").toString());
    ui->part4_rotation8->setText(configFile->value("Detection_Position/part4_rotation8").toString());
    ui->part4_rotation9->setText(configFile->value("Detection_Position/part4_rotation9").toString());
    ui->part4_rotation10->setText(configFile->value("Detection_Position/part4_rotation10").toString());
    //Part 5
    ui->Part5_name->setText(configFile->value("Part_Name/part5_name").toString());
    ui->part5_camera1->setText(configFile->value("Detection_Position/part5_camera1").toString());
    ui->part5_camera2->setText(configFile->value("Detection_Position/part5_camera2").toString());
    ui->part5_rotation1->setText(configFile->value("Detection_Position/part5_rotation1").toString());
    ui->part5_rotation2->setText(configFile->value("Detection_Position/part5_rotation2").toString());
    ui->part5_rotation3->setText(configFile->value("Detection_Position/part5_rotation3").toString());
    ui->part5_rotation4->setText(configFile->value("Detection_Position/part5_rotation4").toString());
    ui->part5_rotation5->setText(configFile->value("Detection_Position/part5_rotation5").toString());
    ui->part5_rotation6->setText(configFile->value("Detection_Position/part5_rotation6").toString());
    ui->part5_rotation7->setText(configFile->value("Detection_Position/part5_rotation7").toString());
    ui->part5_rotation8->setText(configFile->value("Detection_Position/part5_rotation8").toString());
    ui->part5_rotation9->setText(configFile->value("Detection_Position/part5_rotation9").toString());
    ui->part5_rotation10->setText(configFile->value("Detection_Position/part5_rotation10").toString());
    //Running Speed
    ui->Speed_Initial0->setText(configFile->value("Running_Speed/Speed_Initial0").toString());
    ui->Speed_Set0->setText(configFile->value("Running_Speed/Speed_Set0").toString());
    ui->Acceleration0->setText(configFile->value("Running_Speed/Acceleration0").toString());

    ui->Speed_Initial1->setText(configFile->value("Running_Speed/Speed_Initial1").toString());
    ui->Speed_Set1->setText(configFile->value("Running_Speed/Speed_Set1").toString());
    ui->Acceleration1->setText(configFile->value("Running_Speed/Acceleration1").toString());

    ui->Speed_Initial2->setText(configFile->value("Running_Speed/Speed_Initial2").toString());
    ui->Speed_Set2->setText(configFile->value("Running_Speed/Speed_Set2").toString());
    ui->Acceleration2->setText(configFile->value("Running_Speed/Acceleration2").toString());
}

//Lock the GUI when start program
void pmd::lock_ui(bool YN)
{
    auto all_QObject=this->children();
    QObjectList::iterator i;
    for(i=all_QObject.begin();i!=all_QObject.end();++i)
    {
        QObject *wid=*i;
        if(wid->isWidgetType()&&wid->objectName()!="CHANGE"&&wid->objectName()!="CLOSE")
        {
            static_cast<QWidget*>(wid)->setEnabled(!YN);//the code "static_cast" can also replace with "qobject_cast"
        }
    }
}

//GUI buttons control function.
void pmd::on_CHANGE_clicked()
{
    lock_ui(false);
    ui->CHANGE->setEnabled(false);
}

void pmd::on_ALL_CONFIRM_clicked()
{
    switch( QMessageBox::information( this, tr("Parameters Setting"),tr("Really Change?"),\
            tr("Yes"), tr("No"),0, 1 ) ) //  the symbol "\" is a sign to shift the index and line feed
  {
    case 0:
        set_configure_parameter();
        lock_ui(true);
        ui->CHANGE->setEnabled(true);
//        emit signal_parameters_changed(); for real-time change the parameter
        break;
    case 1:
        break;
    default:
        break;
  }
}

void pmd::on_CLOSE_clicked()
{
    close();
}
