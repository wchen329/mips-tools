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
#ifndef TOOLS_RUNTIMEDIRECTIVE_H
#define TOOLS_RUNTIMEDIRECTIVE_H

#include <QDialog>

namespace Ui {
class tools_runtimeDirective;
}

class tools_runtimeDirective : public QDialog
{
    Q_OBJECT

public:
    explicit tools_runtimeDirective(QWidget *parent = 0);
    QString getTextValue();
    ~tools_runtimeDirective();

private slots:
    void on_Run_button_clicked();

private:
    Ui::tools_runtimeDirective *ui;
};

#endif // RUNTIMEDIRECTIVE_H
