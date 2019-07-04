#include "simui.h"
#include "ui_simui.h"
#include "aboutdialog.h"
#include "shell.h"

namespace simulation
{
    SimCntrlRun simThread;

    mipsshell::Shell sh;

    void startSim()
    {
        sh.Run();
    }

    void endSim()
    {
        //sh = mipsshell::Shell();
    }

    void breakSim()
    {
        sh.SetState(mipsshell::Shell::SLEEPING);
    }
}

simUI::simUI(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::simUI)
{
    ui->setupUi(this);
    this->signifySimOff();
    this->setCentralWidget(ui->consoleScreen);
}

simUI::~simUI()
{
    delete ui;
}

void simUI::signifySimOn()
{
    this->ui->actionStop_Simulation->setEnabled(true);
    this->ui->actionBreak_Execution->setEnabled(true);
    this->ui->actionRuntime_Directive->setEnabled(true);
    this->ui->actionStart_Simulation->setEnabled(false);
}

void simUI::signifySimOff()
{
    this->ui->actionStop_Simulation->setEnabled(false);
    this->ui->actionBreak_Execution->setEnabled(false);
    this->ui->actionRuntime_Directive->setEnabled(false);
    this->ui->actionStart_Simulation->setEnabled(true);
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

void simUI::on_actionStart_Simulation_triggered()
{
    this->signifySimOn();
    simulation::simThread.start();
}
