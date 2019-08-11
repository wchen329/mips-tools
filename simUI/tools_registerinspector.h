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
#ifndef TOOLS_REGISTERINSPECTOR_H
#define TOOLS_REGISTERINSPECTOR_H

#include <QDialog>
#include <QHeaderView>
#include <QTreeWidgetItem>
#include <vector>
#include "diag_cpu.h"
#include "streams.h"

namespace Ui {
class tools_registerInspector;
}

class tools_registerInspector : public QDialog
{
    Q_OBJECT

public:
    explicit tools_registerInspector(QWidget *parent = 0);
    ~tools_registerInspector();
    void addCPU(mips_tools::diag_cpu& dcpu);
private:
    Ui::tools_registerInspector *ui;
    std::vector<QTreeWidgetItem*> regNode_allocList;
    std::vector<QTreeWidgetItem*> cpuNode_allocList;
    int cpu_count;
    bool accepted;
};

#endif // TOOLS_REGISTERINSPECTOR_H
