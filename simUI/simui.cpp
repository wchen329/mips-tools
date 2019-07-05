#include "simui.h"
#include "ui_simui.h"
#include "aboutdialog.h"
#include "shell.h"
#include "integration.h"
#include "simoptionsdialog.h"

namespace simulation
{
    SimCntrlRun simThread;
    std::string output_buffer;
    std::string error_buffer;
    QMutex obuf_mutex;
    QMutex ebuf_mutex;
    int mem_bits = 16;
    int cpu_type = 0;
    mipsshell::Shell sh;
    priscas_io::QtPTextWriter * err_str = nullptr;
    priscas_io::QtPTextWriter * out_str = nullptr;

    void startSim()
    {
        sh.Run();
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
    QEvent::registerEventType(QEventReady);
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
    simulation::sh.SetArgs(args);

    // Start Simulation!
    simulation::simThread.start();
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

bool simUI::event(QEvent * qev)
{

    if(qev->type() == QEventReady)
    {
        QBufferReadyEvent* qbre = dynamic_cast<QBufferReadyEvent*>(qev);
        std::string& cont = qbre->getContent();
        this->ui->consoleScreen->append(cont.c_str());
    }

    return QMainWindow::event(qev);
}

void simUI::on_actionStop_Simulation_triggered()
{
    simulation::simThread.exit(0);
}
