#include "tools_specialdebug.h"
#include "ui_tools_specialdebug.h"

tools_specialdebug::tools_specialdebug(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::tools_specialdebug)
{
    ui->setupUi(this);
    this->setLayout(ui->verticalLayoutSpecialDebug);
}

tools_specialdebug::~tools_specialdebug()
{
    delete ui;
}
