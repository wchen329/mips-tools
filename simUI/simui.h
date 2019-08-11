//////////////////////////////////////////////////////////////////////////////
//
//    PRISCAS - Computer architecture simulator
//    Copyright (C) 2019 Winor Chen
//
//    This program is free software; you can redistribute it and/or modify
//    it under the terms of the GNU General Public License as published by
//    the Free Software Foundation; either version 2 of the License, or
//    (at your option) any later version.
//
//    This program is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU General Public License for more details.
//
//    You should have received a copy of the GNU General Public License along
//    with this program; if not, write to the Free Software Foundation, Inc.,
//    51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
//
//////////////////////////////////////////////////////////////////////////////
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

    void on_actionCycle_triggered();

    void on_actionRuntime_Directive_triggered();

    void on_actionMemory_Inspector_triggered();

    void on_actionCPU_Specific_Debugging_Information_triggered();

private:
    QTimer * buf_poller;
    QString sourcefile;
    QString curFileHeader;
    QString simStatusTxt;
    const char * simStop_Txt;
    const char * simStart_Txt;
    const char * simBreak_Txt;
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
