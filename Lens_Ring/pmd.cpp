#include "pmd.h"
#include "ui_pmd.h"

pmd::pmd(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::pmd)
{
    ui->setupUi(this);
}

pmd::~pmd()
{
    delete ui;
}
