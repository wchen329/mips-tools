#include "simoptionsdialog.h"
#include "ui_simoptionsdialog.h"

simoptionsdialog::simoptionsdialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::simoptionsdialog)
{
    ui->setupUi(this);
    ui->comboBoxCPUs->addItem("MIPS32: Single Cycle");
    ui->comboBoxCPUs->addItem("MIPS32: Five Stage Pipeline");
}

simoptionsdialog::~simoptionsdialog()
{
    delete ui;
}

int simoptionsdialog::getSelectedCPUID()
{
    return ui->comboBoxCPUs->currentIndex();
}

int simoptionsdialog::getMemBitWidth()
{
    return ui->horizontalSliderMemBits->value();
}
