#ifndef SIMUI_H
#define SIMUI_H

#include <QTimer>
#include <QThread>
#include <QFileDialog>
#include <QFontDialog>
#include <QMainWindow>
#include <string>
#include <vector>
#include "integration.h"
#include "shell.h"

// simUI: simulation related

namespace simulation
{
    using namespace mipsshell;
    extern Shell * sh;
}

class SimCntrlRun : public QThread
{
    public:

        void run() override
        {
            shell_ptr->Run();
            this->exit();
        }

        void set_shell_ptr(mipsshell::Shell* s) { this->shell_ptr = s; }

    private:
        mipsshell::Shell* shell_ptr;

};

// simUI: Ui and Layout Related

namespace Ui {
class simUI;
}

class simUI : public QMainWindow
{
    Q_OBJECT

public:
    explicit simUI(QWidget *parent = 0);
    QTextEdit& getConsoleWindowRef();
    ~simUI();

private slots:
    void on_actionExit_triggered();

    void on_actionAbout_simUI_triggered();

    void on_actionStart_Simulation_triggered();

    void on_actionClear_Console_Window_triggered();

    void on_actionFont_triggered();

    void on_actionSet_Simulation_Source_triggered();

    void on_actionStop_Simulation_triggered();

    void on_actionCPU_Options_triggered();

    void bufferUpdate_Timer_Triggered();

    void cleanUpSim();

    void on_actionAdd_Breakpoint_triggered();

    void on_actionList_Current_Breakpoints_triggered();

    void on_actionBreak_Execution_triggered();

    void on_actionRegister_Inspector_triggered();

    void on_actionContinue_triggered();

private:
    QTimer * buf_poller;
    QString sourcefile;
    void signifySimOn();
    void signifySimSuspended();
    void signifySimOff();
    Ui::simUI *ui;
    std::vector<unsigned long> programBreakpoints;
    std::vector<unsigned long> archBreakPoints;
    priscas_io::QtPTextWriter simTextI;
    priscas_io::QtPTextWriter simTextO;
};

#endif // SIMUI_H
