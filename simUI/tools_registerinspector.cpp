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
#include "tools_registerinspector.h"
#include "ui_tools_registerinspector.h"

tools_registerInspector::tools_registerInspector(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::tools_registerInspector),
    cpu_count(0)
{
    ui->setupUi(this);
    this->setLayout(this->ui->gridLayoutWidget->layout());
    QStringList * qsl = new QStringList();
    qsl->append("Name");
    qsl->append("Signed Integer");
    qsl->append("Unsigned Integer");
    qsl->append("Floating Point");
    qsl->append("Hex");
    this->ui->treeViewRegisters->setHeaderLabels(*qsl);
}

void tools_registerInspector::addCPU(priscas::cpu &dcpu)
{

    QTreeWidgetItem * cpu_node = new QTreeWidgetItem();
    cpu_node->setText(0, QString("CPU ") + priscas_io::StrTypes::IntToStr(cpu_count).c_str());
    cpu_count++;
    this->cpuNode_allocList.push_back(cpu_node);

    QTreeWidgetItem * register_node = new QTreeWidgetItem();

    QString spfpr;
    spfpr.setNum(dcpu.get_PC().AsSPFloat(), 'g', 6);

    register_node->setText(0, QString("PC"));
    register_node->setText(1, QString(priscas_io::StrTypes::Int32ToStr(dcpu.get_PC().AsInt32()).c_str()));
    register_node->setText(2, QString(priscas_io::StrTypes::UInt32ToStr(dcpu.get_PC().AsUInt32()).c_str()));
    register_node->setText(3, spfpr);
    register_node->setText(4, dcpu.get_PC().toHexString().c_str());


    cpu_node->addChild(register_node);
    this->regNode_allocList.push_back(register_node);

    for(int itr = 0; itr < dcpu.get_ISA().get_reg_count(); itr++)
    {

        QTreeWidgetItem * register_node = new QTreeWidgetItem();
        QString spfpr;
        spfpr.setNum(dcpu.get_reg_data(itr).AsSPFloat(), 'g', 6);
        register_node->setText(0, QString(dcpu.get_ISA().get_reg_name(itr).c_str()));
        register_node->setText(1, QString(priscas_io::StrTypes::Int32ToStr(dcpu.get_reg_data(itr).AsInt32()).c_str()));
        register_node->setText(2, QString(priscas_io::StrTypes::UInt32ToStr(dcpu.get_reg_data(itr).AsUInt32()).c_str()));
        register_node->setText(3, spfpr);
        register_node->setText(4, dcpu.get_reg_data(itr).toHexString().c_str());
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
