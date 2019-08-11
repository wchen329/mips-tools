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
#ifndef SIMOPTIONSDIALOG_H
#define SIMOPTIONSDIALOG_H

#include <QDialog>

namespace Ui {
class simoptionsdialog;
}

class simoptionsdialog : public QDialog
{
    Q_OBJECT

public:
    explicit simoptionsdialog(QWidget *parent = 0);
    ~simoptionsdialog();
    int getSelectedCPUID();
    int getMemBitWidth();
    void setSelectedCPUID(int);
    void setMemBitWidth(int);

private slots:
    void on_horizontalSliderMemBits_valueChanged(int value);

private:
    Ui::simoptionsdialog *ui;
};

#endif // SIMOPTIONSDIALOG_H
