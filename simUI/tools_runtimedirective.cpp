#include "tools_runtimedirective.h"
#include "ui_tools_runtimedirective.h"

tools_runtimeDirective::tools_runtimeDirective(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::tools_runtimeDirective)
{
    ui->setupUi(this);
    ui->Run_button->setDefault(true);
}

QString tools_runtimeDirective::getTextValue()
{
    return ui->lineEditCommand->text();
}

tools_runtimeDirective::~tools_runtimeDirective()
{
    delete ui;
}

void tools_runtimeDirective::on_Run_button_clicked()
{
    this->accept();
    this->close();
}
