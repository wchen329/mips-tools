#include "tools_registerinspector.h"
#include "ui_tools_registerinspector.h"

tools_registerInspector::tools_registerInspector(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::tools_registerInspector),
    cpu_count(0)
{
    ui->setupUi(this);
    this->setLayout(this->ui->gridLayoutWidget->layout());
}

void tools_registerInspector::addCPU(mips_tools::diag_cpu &dcpu)
{
    QTreeWidgetItem * cpu_node = new QTreeWidgetItem();
    cpu_node->setText(0, QString("CPU ") + QString(this->cpu_count));
    cpu_count++;
    this->cpuNode_allocList.push_back(cpu_node);

    for(int itr = 0; itr < dcpu.get_reg_count(); itr++)
    {
        QTreeWidgetItem * register_node = new QTreeWidgetItem();
        register_node->setText(0, QString(dcpu.get_ISA().get_reg_name(itr).c_str()) +
                               QString(": ") +
                               QString(priscas_io::StrTypes::Int32ToStr(dcpu.get_reg_data(itr)).c_str()));
        cpu_node->addChild(register_node);
        this->regNode_allocList.push_back(register_node);
    }

    this->ui->treeViewRegisters->addTopLevelItem(cpu_node);
}

tools_registerInspector::~tools_registerInspector()
{
    delete ui;

    for(size_t itr = 0; itr < this->regNode_allocList.size(); itr++)
    {
        delete this->regNode_allocList[itr];
    }

    for(size_t itr = 0; itr < this->cpuNode_allocList.size(); itr++)
    {
        delete this->cpuNode_allocList[itr];
    }
}
