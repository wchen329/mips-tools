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
#ifndef TOOLS_MEMINSPECTOR_H
#define TOOLS_MEMINSPECTOR_H

#include <QDialog>
#include <QTreeWidgetItem>
#include "mmem.h"

namespace Ui {
class tools_meminspector;
}

class tools_meminspector : public QDialog
{
    Q_OBJECT

public:
    explicit tools_meminspector(QWidget *parent = 0);
    ~tools_meminspector();
    void addMemoryChannel(mips_tools::mmem* m);

private slots:
    void on_pushButton_Display_clicked();

private:
    Ui::tools_meminspector *ui;
    std::vector<mips_tools::mmem*> memoryChannels;
    std::vector<QTreeWidgetItem*> channel_alloc;
    std::vector<QTreeWidgetItem*> mement_alloc;
    void resetTree();
};

#endif // TOOLS_MEMINSPECTOR_H
