#include "addbreak.h"
#include "listbreak.h"
#include "simui.h"
#include "ui_simui.h"
#include "aboutdialog.h"
#include "shell.h"
#include "states.h"
#include "integration.h"
#include "simoptionsdialog.h"

namespace simulation
{
    SimCntrlRun runner;
    std::string output_buffer;
    std::string error_buffer;
    QMutex obuf_mutex;
    QMutex ebuf_mutex;
    int mem_bits = 16;
    int cpu_type = 0;
    mipsshell::Shell* sh = nullptr;
    priscas_io::QtPTextWriter * err_str = nullptr;
    priscas_io::QtPTextWriter * out_str = nullptr;
}

simUI::simUI(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::simUI)
{
    this->buf_poller = new QTimer(this);
    ui->setupUi(this);
    ui->consoleScreen->append("sim UI Runtime Console\n---------------\n");
    simulation::err_str = new priscas_io::QtPTextWriter(*this->ui->consoleScreen);
    simulation::out_str = new priscas_io::QtPTextWriter(*this->ui->consoleScreen);
    this->signifySimOff();
    this->setCentralWidget(ui->consoleScreen);
    this->buf_poller->setInterval(5);
    connect(buf_poller, SIGNAL(timeout()), this, SLOT(bufferUpdate_Timer_Triggered()));
    connect(&simulation::runner, SIGNAL(finished()), this, SLOT(cleanUpSim()));
}

simUI::~simUI()
{
    delete ui;
}

void simUI::bufferUpdate_Timer_Triggered()
{
    simulation::obuf_mutex.lock();
    this->ui->consoleScreen->append(simulation::output_buffer.c_str());
    simulation::output_buffer = "";
    simulation::obuf_mutex.unlock();
}

void simUI::signifySimOn()
{
    this->ui->actionStop_Simulation->setEnabled(true);
    this->ui->actionBreak_Execution->setEnabled(true);
    this->ui->actionRuntime_Directive->setEnabled(false);
    this->ui->actionStart_Simulation->setEnabled(false);
    this->ui->actionContinue->setEnabled(false);
    this->ui->actionAdd_Breakpoint->setEnabled(false);
    this->ui->actionList_Current_Breakpoints->setEnabled(false);
    this->buf_poller->start();

}

void simUI::signifySimSuspended()
{
    this->ui->actionStop_Simulation->setEnabled(true);
    this->ui->actionBreak_Execution->setEnabled(true);
    this->ui->actionRuntime_Directive->setEnabled(true);
    this->ui->actionStart_Simulation->setEnabled(false);
    this->ui->actionContinue->setEnabled(true);
    this->ui->actionAdd_Breakpoint->setEnabled(false);
    this->ui->actionList_Current_Breakpoints->setEnabled(false);
}

void simUI::signifySimOff()
{
    this->ui->actionStop_Simulation->setEnabled(false);
    this->ui->actionBreak_Execution->setEnabled(false);
    this->ui->actionRuntime_Directive->setEnabled(false);
    this->ui->actionStart_Simulation->setEnabled(true);
    this->buf_poller->stop();
    this->ui->consoleScreen->append(simulation::output_buffer.c_str());
    this->ui->actionContinue->setEnabled(false);
    this->ui->actionAdd_Breakpoint->setEnabled(true);
    this->ui->actionList_Current_Breakpoints->setEnabled(true);
    simulation::output_buffer = "";
}

void simUI::cleanUpSim()
{
    this->signifySimOff();
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
    if(this->sourcefile == "")
    {
        this->ui->consoleScreen->append("A assembly source file must be first selected before simulation can begin.\n");
        return;
    }

    // Set arguments
    std::vector<std::string> args;
    args.push_back("mtshell");
    args.push_back("-i");
    args.push_back(this->sourcefile.toStdString());
    args.push_back("-m");
    args.push_back(priscas_io::StrTypes::IntToStr(simulation::mem_bits));
    args.push_back("-c");
    args.push_back(priscas_io::StrTypes::IntToStr(simulation::cpu_type));

    // Start Simulation!    
    //delete simulation::sh;
    this->signifySimOn();
    simulation::sh = new mipsshell::Shell();
    simulation::sh->SetArgs(args);
    simulation::sh->setErrorTextStream(*simulation::err_str);
    simulation::sh->setOutputTextStream(*simulation::out_str);
    simulation::sh->setNoConsoleOutput(true);

    for(size_t abp_i = 0; abp_i < this->archBreakPoints.size(); abp_i++)
    {
        //if(!simulation::sh->has_ma_break_at(abp_i))
            simulation::sh->add_microarch_breakpoint(abp_i);
    }

    for(size_t pbp_i = 0; pbp_i < this->programBreakpoints.size(); pbp_i++)
    {
        //if(!simulation::sh->has_prog_break_at(abp_i))
            simulation::sh->add_program_breakpoint(pbp_i);
    }

    simulation::runner.set_shell_ptr(simulation::sh);
    simulation::runner.start();
}

QTextEdit& simUI::getConsoleWindowRef()
{
    return *this->ui->consoleScreen;
}

void simUI::on_actionClear_Console_Window_triggered()
{
    this->ui->consoleScreen->setText("");

}

void simUI::on_actionFont_triggered()
{
    QFontDialog qfd;
    bool okToGet;
    QFont & f = qfd.getFont(&okToGet);
    if(okToGet)
    {
        this->ui->consoleScreen->setFont(f);
    }
}

void simUI::on_actionSet_Simulation_Source_triggered()
{
    QFileDialog qfd;
    QString filter = "Visual C ASM (*.asm);; UNIX Assembly (*.s)";
    QString URL;
#ifdef WIN32
    URL = "%userprofile%";
#else
    URL = "~";
#endif
    qfd.setDirectory(URL);
    qfd.setNameFilter(filter);
    qfd.setFileMode(QFileDialog::ExistingFile);

    if(qfd.exec())
    {
        QStringList& qst = qfd.selectedFiles();

        for(int s = 0; s < qst.size() || s < 1; s++)
        {
            this->sourcefile = qst[s];
        }
    }
}

void simUI::on_actionCPU_Options_triggered()
{
    simoptionsdialog sod;
    sod.exec();
    simulation::mem_bits = sod.getMemBitWidth();
    simulation::cpu_type = sod.getSelectedCPUID();
}

void simUI::on_actionStop_Simulation_triggered()
{
    simulation::sh->SetState(mipsshell::Shell::KILLED);
    simulation::runner.wait(1);
    this->signifySimOff();
}

void simUI::on_actionAdd_Breakpoint_triggered()
{
    addbreak ab;
    if(ab.exec())
    {
        QString qst = ab.getTextContent();
        unsigned long val_conv = qst.toULong();

        if(ab.isArchBreakPoint())
        {
            this->archBreakPoints.push_back(val_conv);
        }

        else if(ab.isProgBreakPoint())
        {
            this->programBreakpoints.push_back(val_conv);
        }
    }
}

void simUI::on_actionList_Current_Breakpoints_triggered()
{
    listbreak lb(this->archBreakPoints, this->programBreakpoints);
    lb.exec();
}

void simUI::on_actionBreak_Execution_triggered()
{
    mipsshell::INTERACTIVE = true;
    mipsshell::SUSPEND = true;
    this->signifySimSuspended();
}
