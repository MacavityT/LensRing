#ifndef CMD_H
#define CMD_H

#include <QDialog>
#include <QSettings>

namespace Ui {
class CMD;
}

class CMD : public QDialog
{
    Q_OBJECT

public:
    explicit CMD(QWidget *parent = 0);
    ~CMD();

private:
    Ui::CMD *ui;
};

#endif // CMD_H
