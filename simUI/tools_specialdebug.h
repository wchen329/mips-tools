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
#ifndef TOOLS_SPECIALDEBUG_H
#define TOOLS_SPECIALDEBUG_H

#include <QDialog>
#include <QTableWidget>
#include <QTreeWidget>
#include <vector>
#include "diag_cpu.h"

namespace Ui {
class tools_specialdebug;
}

class tools_specialdebug : public QDialog
{
    Q_OBJECT

public:
    explicit tools_specialdebug(QWidget *parent = 0);
    void setCPU(mips_tools::diag_cpu& dcpu);
    ~tools_specialdebug();


private slots:
    void on_comboBox_Views_currentIndexChanged(int index);

private:
    Ui::tools_specialdebug *ui;
    QWidget* foregroundWidget;
    std::vector<QWidget*> wList;
    void setWidgetInForeground(QWidget *);
};

#endif // TOOLS_SPECIALDEBUG_H
