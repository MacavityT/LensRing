#ifndef MODEL_H
#define MODEL_H

#include <QDialog>

namespace Ui {
class MODEL;
}

class MODEL : public QDialog
{
    Q_OBJECT

public:
    explicit MODEL(QWidget *parent = 0);
    ~MODEL();

private:
    Ui::MODEL *ui;
};

#endif // MODEL_H
