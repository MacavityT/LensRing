#ifndef PMD_H
#define PMD_H

#include <QDialog>

namespace Ui {
class pmd;
}

class pmd : public QDialog
{
    Q_OBJECT

public:
    explicit pmd(QWidget *parent = 0);
    ~pmd();

private:
    Ui::pmd *ui;
};

#endif // PMD_H
