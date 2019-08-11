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
#ifndef LISTBREAK_H
#define LISTBREAK_H

#include <QDialog>
#include <queue>

namespace Ui {
class listbreak;
}

class listbreak : public QDialog
{
    Q_OBJECT

public:
    explicit listbreak(QWidget *parent = 0);
    listbreak(std::vector<unsigned long>& abps, std::vector<unsigned long>& pbps, QWidget* parent = 0);
    std::queue<int>& getRemoveReplay_arch() { return this->ModelreplayQueue_arch;}
    std::queue<int>& getRemoveReplay_prog() { return this->ModelreplayQueue_prog; }
    ~listbreak();

private slots:
    void on_PBP_removeButton_clicked();
    void on_ABP_removeButton_clicked();

private:
    Ui::listbreak *ui;
    std::queue<int> ModelreplayQueue_arch;
    std::queue<int> ModelreplayQueue_prog;

};

#endif // LISTBREAK_H
