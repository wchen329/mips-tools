#include "simui.h"
#include "ui_simui.h"
#include "aboutdialog.h"
#include "shell.h"
#include "integration.h"

namespace simulation
{
    SimCntrlRun simThread;
    mipsshell::Shell sh;
    priscas_io::QtPTextWriter * err_str = nullptr;
    priscas_io::QtPTextWriter * out_str = nullptr;

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
    ui->consoleScreen->append("sim UI Runtime Console\n---------------\n");
    simulation::err_str = new priscas_io::QtPTextWriter(*this->ui->consoleScreen);
    simulation::out_str = new priscas_io::QtPTextWriter(*this->ui->consoleScreen);
    simulation::sh.setErrorTextStream(*simulation::err_str);
    simulation::sh.setOutputTextStream(*simulation::out_str);
    simulation::sh.setNoConsoleOutput(true);

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
    std::vector<std::string> args; args.push_back("mtshell"); args.push_back("-h");
    simulation::sh.SetArgs(args);
    simulation::sh.Run();
    this->signifySimOn();
    this->signifySimOff();
}

QTextEdit& simUI::getConsoleWindowRef()
{
    return *this->ui->consoleScreen;
}

void simUI::on_actionClear_Console_Window_triggered()
{
    this->ui->consoleScreen->setText("");
}
