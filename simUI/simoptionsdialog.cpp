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
