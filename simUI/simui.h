#ifndef SIMUI_H
#define SIMUI_H

#include <QFileDialog>
#include <QFontDialog>
#include <QThread>
#include <QMainWindow>
#include <string>
#include "integration.h"
#include "shell.h"

// simUI: simulation related

namespace simulation
{
    using namespace mipsshell;
    extern Shell sh;
}

class SimCntrlRun : public QThread
{

    void run() override
    {
        simulation::sh.Run();
    }

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

private:
    QString sourcefile;
    void signifySimOn();
    void signifySimOff();
    Ui::simUI *ui;
};

#endif // SIMUI_H
