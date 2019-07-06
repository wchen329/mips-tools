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
