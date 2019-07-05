#include "aboutdialog.h"
#include "ui_aboutdialog.h"

aboutDialog::aboutDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::aboutDialog)
{
    ui->setupUi(this);
    ui->pushButton->setDefault(true);
    ui->textEditAbout->setEnabled(false);
    ui->textEditAbout->
            append("WARNING: This software is currently in development and may have some bugs.\n---------\nCredits:wchen329@wisc.edu");
}

aboutDialog::~aboutDialog()
{
    delete ui;
}

void aboutDialog::on_pushButton_clicked()
{
    this->close();
}
