#ifndef CMD_H
#define CMD_H

#include <QDialog>
#include <QSettings>
#include <QList>

namespace Ui {
class CMD;
}

class CMD : public QDialog
{
    Q_OBJECT

public:
    explicit CMD(QWidget *parent = 0);
    ~CMD();

private slots:
    void on_Camera1_up_clicked();

    void on_Camera1_down_clicked();

    void on_Camera2_up_clicked();

    void on_Camera2_down_clicked();

    void on_Anticlockwise_clicked();

    void on_Clockwise_clicked();

private:
    Ui::CMD *ui;
};

#endif // CMD_H
