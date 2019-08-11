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
#ifndef INTEGRATION_H
#define INTEGRATION_H
#include <QCoreApplication>
#include <QMutex>
#include <QString>
#include <QTextEdit>
#include <string>
#include <iostream>
#include "streams.h"

namespace priscas_io
{

    enum stream_type
    {
        ST_OUTPUT,
        ST_ERROR
    };

    class QtPTextWriter : public text_stream
    {
        std::string rep;
        QMutex qm;

        // disable constructors
       public:
            void operator<<(std::string& input) override;
            void operator>>(std::string& output) override;
    };
}

#endif // INTEGRATION_H
