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
#include "listbreak.h"
#include "ui_listbreak.h"
#include "streams.h"

listbreak::listbreak(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::listbreak)
{
    ui->setupUi(this);
    this->setLayout(this->ui->verticalLayoutBPList);
}

listbreak::listbreak(std::vector<unsigned long>& abps, std::vector<unsigned long>& pbps, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::listbreak)
{
    ui->setupUi(this);
    this->setLayout(this->ui->verticalLayoutBPList);

    for(size_t abp_i = 0; abp_i < abps.size(); abp_i++)
    {
        this->ui->ABP_List->addItem(QString("[Arch. Breakpoint ") + QString(priscas_io::StrTypes::SizeToStr(abp_i).c_str()) + QString("] - ")
                                    + QString("Cycle ") + QString(priscas_io::StrTypes::UInt32ToStr(abps[abp_i]).c_str()));
    }

    for(size_t pbp_i = 0; pbp_i < pbps.size(); pbp_i++)
    {
        this->ui->PBP_List->addItem(QString("[Program Breakpoint ") + QString(priscas_io::StrTypes::SizeToStr(pbp_i).c_str()) + QString("] - ")
                                    + QString("Line ") + QString(priscas_io::StrTypes::UInt32ToStr(pbps[pbp_i]).c_str()));
    }
}

listbreak::~listbreak()
{
    delete ui;
}

void listbreak::on_PBP_removeButton_clicked()
{
    QList<QListWidgetItem*> w = this->ui->PBP_List->selectedItems();
    for(int ws = 0; ws < w.size(); ws++)
    {
        int ww = this->ui->PBP_List->row(w[ws]);
        this->ModelreplayQueue_prog.push(ww);
        delete w[ws];
    }
}

void listbreak::on_ABP_removeButton_clicked()
{
    QList<QListWidgetItem*> e = this->ui->ABP_List->selectedItems();

    for(int es = 0; es < e.size(); es++)
    {
        int ee = this->ui->ABP_List->row(e[es]);
        this->ModelreplayQueue_arch.push(ee);
        delete e[es];
    }
}
