#include "simui.h"
#include "ui_simui.h"
#include "aboutdialog.h"

simUI::simUI(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::simUI)
{
    ui->setupUi(this);
    this->setCentralWidget(ui->consoleScreen);
}

simUI::~simUI()
{
    delete ui;
}

void simUI::on_actionExit_triggered()
{
    this->close();
}

void simUI::on_actionAbout_simUI_triggered()
{
    aboutDialog ad;
    ad.exec();
}
