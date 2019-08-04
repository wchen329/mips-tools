#include "simoptionsdialog.h"
#include "ui_simoptionsdialog.h"
#include "streams.h"

simoptionsdialog::simoptionsdialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::simoptionsdialog)
{
    ui->setupUi(this);
    ui->comboBoxCPUs->addItem("MIPS32: Single Cycle");
    ui->comboBoxCPUs->addItem("MIPS32: Five Stage Pipeline");
    this->setLayout(ui->cpuOptsLayout);
}

simoptionsdialog::~simoptionsdialog()
{
    delete ui;
}

int simoptionsdialog::getSelectedCPUID()
{
    return ui->comboBoxCPUs->currentIndex();
}

void simoptionsdialog::setSelectedCPUID(int in)
{
    this->ui->comboBoxCPUs->setCurrentIndex(in);
}

int simoptionsdialog::getMemBitWidth()
{
    return ui->horizontalSliderMemBits->value();
}

void simoptionsdialog::setMemBitWidth(int in)
{
    ui->horizontalSliderMemBits->setValue(in);
}

void simoptionsdialog::on_horizontalSliderMemBits_valueChanged(int value)
{
    ui->label_TotalSize->setText(QString("Total Size: ") + QString(priscas_io::StrTypes::IntToStr(2 << (value - 1)).c_str()) +
                                 QString(" bytes"));
}
