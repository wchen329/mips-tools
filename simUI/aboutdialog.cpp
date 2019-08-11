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
#include "aboutdialog.h"
#include "branding.h"
#include "ui_aboutdialog.h"

aboutDialog::aboutDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::aboutDialog)
{
    ui->setupUi(this);
    ui->pushButton->setDefault(true);
    ui->layoutAbout->addWidget(ui->textEditAbout);
    ui->layoutAbout->addWidget(ui->pushButton);
    ui->labelVersion->setText(QString("Version ") + QString(branding::APPLICATION_VERSION.c_str()));

    std::string VerNotice = std::string("Version ") + std::string(PRISCAS_BRANDING_FULL_VERSION_STRING) + std::string("\n\n");

    //ui->layoutAbout->addItem();
    this->setLayout(ui->layoutAbout);
    ui->textEditAbout->
            append("PRISCAS simUI - GUI frontend for PRISCAS\n");
    ui->textEditAbout->
            append(VerNotice.c_str());
    ui->textEditAbout->
            append("This software is licensed under the GNU General Public License (GPL) Version 2.\n");
    ui->textEditAbout->
            append("This software utilizes the Qt Widgets GUI toolkit which follows its own licenses.");
    ui->textEditAbout->
            append("These licenses are included with this software.\n");
    ui->textEditAbout->
            append("\n");
    ui->textEditAbout->
            append("WARNING: This software is currently in development and may have some bugs.\nAuthor:wchen329@wisc.edu");
}

aboutDialog::~aboutDialog()
{
    delete ui;
}

void aboutDialog::on_pushButton_clicked()
{
    this->close();
}
