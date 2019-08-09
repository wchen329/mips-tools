#include "addbreak.h"
#include "listbreak.h"
#include "simui.h"
#include "ui_simui.h"
#include "aboutdialog.h"
#include "shell.h"
#include "states.h"
#include "integration.h"
#include "simoptionsdialog.h"
#include "tools_meminspector.h"
#include "tools_registerinspector.h"
#include "tools_runtimedirective.h"
#include "tools_specialdebug.h"

namespace simulation
{
    SimCntrlRun runner;
    int mem_bits = 16;
    int cpu_type = 0;
    mipsshell::Shell* sh = nullptr;
}

simUI::simUI(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::simUI),
    simStart_Txt("(Simulation Running)"),
    simStop_Txt("(Simulation Stopped)"),
    simBreak_Txt("(Simulation Paused)")
{
    this->buf_poller = new QTimer(this);
    ui->setupUi(this);
    ui->consoleScreen->append("sim UI Runtime Console\n---------------\n");
    this->signifySimOff();
    this->curFileHeader = "Current File: ";
    this->setCentralWidget(ui->consoleScreen);
    this->buf_poller->setInterval(5);
    this->ui->statusLabel->setText(this->curFileHeader + "None" + " " + this->simStop_Txt);
    this->ui->statusBar->addPermanentWidget(this->ui->statusLabel);
    connect(buf_poller, SIGNAL(timeout()), this, SLOT(bufferUpdate_Timer_Triggered()));
    connect(&simulation::runner, SIGNAL(finished()), this, SLOT(cleanUpSim()));
}

simUI::~simUI()
{
    simulation::runner.terminate();
    delete ui;
}

void simUI::bufferUpdate_Timer_Triggered()
{
    std::string buf;
    this->simTextO >> buf;

    if(buf != "")
        this->ui->consoleScreen->append(buf.c_str());

    // Check state
    if(mipsshell::INTERACTIVE && mipsshell::SUSPEND)
    {
        this->signifySimSuspended();
    }
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
    this->ui->actionSet_Simulation_Source->setEnabled(false);
    this->ui->actionMemory_Inspector->setEnabled(false);
    this->ui->actionRegister_Inspector->setEnabled(false);
    this->buf_poller->start();
    this->ui->actionCycle->setEnabled(false);
    this->ui->statusLabel->setText(this->curFileHeader + sourcefile + " " + this->simStart_Txt);
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
    this->ui->actionSet_Simulation_Source->setEnabled(false);
    this->ui->actionMemory_Inspector->setEnabled(true);
    this->ui->actionRegister_Inspector->setEnabled(true);
    this->ui->actionCycle->setEnabled(true);
    this->ui->statusLabel->setText(this->curFileHeader + sourcefile + " " + this->simBreak_Txt);
}

void simUI::signifySimOff()
{
    this->ui->actionStop_Simulation->setEnabled(false);
    this->ui->actionBreak_Execution->setEnabled(false);
    this->ui->actionRuntime_Directive->setEnabled(false);
    this->ui->actionStart_Simulation->setEnabled(true);
    this->buf_poller->stop();
    this->ui->actionContinue->setEnabled(false);
    this->ui->actionAdd_Breakpoint->setEnabled(true);
    this->ui->actionList_Current_Breakpoints->setEnabled(true);
    this->ui->actionSet_Simulation_Source->setEnabled(true);
    this->ui->actionMemory_Inspector->setEnabled(true);
    this->ui->actionRegister_Inspector->setEnabled(true);
    this->ui->actionCycle->setEnabled(false);
    this->ui->statusLabel->setText(this->curFileHeader + sourcefile + " " + this->simStop_Txt);
    mipsshell::INTERACTIVE = false;
    mipsshell::SUSPEND = false;

    // Clear buffer
    std::string buf;
    this->simTextO >> buf;
    if(buf != "")
        this->ui->consoleScreen->append(buf.c_str());
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
    simulation::sh->setErrorTextStream(this->simTextO);
    simulation::sh->setOutputTextStream(this->simTextO);
    simulation::sh->setInputTextStream(this->simTextI);
    simulation::sh->setNoConsoleOutput(true);

    for(size_t abp_i = 0; abp_i < this->archBreakPoints.size(); abp_i++)
    {
        //if(!simulation::sh->has_ma_break_at(abp_i))
            simulation::sh->add_microarch_breakpoint(archBreakPoints[abp_i]);
    }

    for(size_t pbp_i = 0; pbp_i < this->programBreakpoints.size(); pbp_i++)
    {
        //if(!simulation::sh->has_prog_break_at(abp_i))
            simulation::sh->declare_program_breakpoint(programBreakpoints[pbp_i]);
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
    QFont f = qfd.getFont(&okToGet);
    if(okToGet)
    {
        this->ui->consoleScreen->setFont(f);
    }
}

void simUI::on_actionSet_Simulation_Source_triggered()
{
    QFileDialog qfd;
    QString filter = "Assembly Source (*.asm | *.s)";
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
        QStringList qst = qfd.selectedFiles();

        for(int s = 0; s < qst.size() || s < 1; s++)
        {
            this->sourcefile = qst[s];
        }

        this->ui->statusLabel->setText(this->curFileHeader + this->sourcefile + " " + this->simStop_Txt);
    }
}

void simUI::on_actionCPU_Options_triggered()
{
    simoptionsdialog sod;
    sod.setMemBitWidth(simulation::mem_bits);
    sod.setSelectedCPUID(simulation::cpu_type);
    sod.exec();
    simulation::mem_bits = sod.getMemBitWidth();
    simulation::cpu_type = sod.getSelectedCPUID();
}

void simUI::on_actionStop_Simulation_triggered()
{
    mipsshell::INTERACTIVE = false;
    simulation::sh->SetState(mipsshell::Shell::KILLED);
    while(!simulation::runner.isFinished())
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

    std::queue<int>& arm = lb.getRemoveReplay_arch();
    std::queue<int>& prm = lb.getRemoveReplay_prog();

    while(!arm.empty())
    {
        std::vector<unsigned long>::iterator erasable = this->archBreakPoints.begin();

        for(int itr = 0; itr < arm.front(); itr++)
        {
           erasable++;
        }

        this->archBreakPoints.erase(erasable);

        arm.pop();
    }

    while(!prm.empty())
    {
        std::vector<unsigned long>::iterator erasable = this->programBreakpoints.begin();

        for(int itr = 0; itr < prm.front(); itr++)
        {
           erasable++;
        }

        this->programBreakpoints.erase(erasable);
        prm.pop();
    }
}

void simUI::on_actionBreak_Execution_triggered()
{
    mipsshell::INTERACTIVE = true;
    mipsshell::SUSPEND = true;
    this->signifySimSuspended();
}

void simUI::on_actionRegister_Inspector_triggered()
{
    tools_registerInspector tri;
    try
    {
        if(simulation::sh != nullptr)
            tri.addCPU(dynamic_cast<mips_tools::diag_cpu&>(simulation::sh->GetMotherboard().get_cpu()));
    }

    catch(std::bad_cast)
    {

    }

    tri.exec();
}

void simUI::on_actionContinue_triggered()
{
    mipsshell::INTERACTIVE = false;
    mipsshell::SUSPEND = false;
    this->signifySimOn();
}

void simUI::on_actionCycle_triggered()
{
    std::string cycle_cmd = ".cycle\n";
    this->simTextI << cycle_cmd;
}

void simUI::on_actionRuntime_Directive_triggered()
{
    tools_runtimeDirective trd;
    if(trd.exec())
    {
        std::string val = trd.getTextValue().toStdString() + priscas_io::newLine;
        this->simTextO << val;
        this->simTextI << val;
    }
}

void simUI::on_actionMemory_Inspector_triggered()
{
    tools_meminspector tmi;

    if(simulation::sh != nullptr)
        tmi.addMemoryChannel(&(simulation::sh->GetMotherboard().get_mmem()));
    tmi.exec();
}

void simUI::on_actionCPU_Specific_Debugging_Information_triggered()
{
    tools_specialdebug tsd;

    if(simulation::sh != nullptr)
    {
        mips_tools::diag_cpu& dcpu = static_cast<mips_tools::diag_cpu&>(simulation::sh->GetMotherboard().get_cpu());
        tsd.setCPU(dcpu);
    }
    tsd.exec();
}
