#include "model.h"
#include "ui_model.h"

MODEL::MODEL(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MODEL)
{
    ui->setupUi(this);
}

MODEL::~MODEL()
{
    delete ui;
}
