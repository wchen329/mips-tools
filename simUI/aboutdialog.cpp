#include "aboutdialog.h"
#include "ui_aboutdialog.h"

aboutDialog::aboutDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::aboutDialog)
{
    ui->setupUi(this);
    ui->pushButton->setDefault(true);
    ui->layoutAbout->addWidget(ui->textEditAbout);
    ui->layoutAbout->addWidget(ui->pushButton);

    //ui->layoutAbout->addItem();
    this->setLayout(ui->layoutAbout);
    ui->textEditAbout->
            append("PRISCAS simUI - GUI frontend for PRISCAS\n");
    ui->textEditAbout->
            append("Version 0.2.2019.7\n\n");
    ui->textEditAbout->
            append("This software is licensed under the GNU General Public License (GPL) Version 2.\n");
    ui->textEditAbout->
            append("This software utilizes the Qt Widgets GUI toolkit which follows its own licenses.");
    ui->textEditAbout->
            append("These licenses are included with this software.\n");
    ui->textEditAbout->
            append("\n");
    ui->textEditAbout->
            append("WARNING: This software is currently in development and may have some bugs.\nAuthor:wchen329@wisc.edu");
}

aboutDialog::~aboutDialog()
{
    delete ui;
}

void aboutDialog::on_pushButton_clicked()
{
    this->close();
}
