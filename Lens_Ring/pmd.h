#ifndef PMD_H
#define PMD_H

#include <QDialog>
#include <QMessageBox>
#include <QSettings>

namespace Ui {
class pmd;
}

class pmd : public QDialog
{
    Q_OBJECT

public:
    explicit pmd(QWidget *parent = 0);
    ~pmd();

    QSettings *configFile;
    void set_configure_parameter();
    void disp_configure_parameter();
    void lock_ui(bool);

private slots:
    void on_CHANGE_clicked();

    void on_ALL_CONFIRM_clicked();

    void on_CLOSE_clicked();

private:
    Ui::pmd *ui;
};

#endif // PMD_H
