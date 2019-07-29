#include "addbreak.h"
#include "ui_addbreak.h"

addbreak::addbreak(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::addbreak)
{
    ui->setupUi(this);
    this->setLayout(this->ui->verticalLayoutAddBreak);
}

bool addbreak::isArchBreakPoint()
{
    return this->ui->ArchBP_radioButton->isChecked();
}

bool addbreak::isProgBreakPoint()
{
    return this->ui->ProgramBP_radioButton->isChecked();
}

QString addbreak::getTextContent()
{
    return this->ui->Value_lineEdit->text();
}

addbreak::~addbreak()
{
    delete ui;
}
