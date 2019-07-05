#ifndef INTEGRATION_H
#define INTEGRATION_H
#include <QString>
#include <QTextEdit>
#include <string>
#include "streams.h"
#include <iostream>

namespace priscas_io
{
    enum stream_type
    {
        ST_OUTPUT,
        ST_ERROR
    };

    class QtPTextWriter : public text_stream
    {
        QTextEdit * qte;

        // disable constructors

        void operator<<(std::string& input) override
        {
            QString in = input.c_str();
            qte->append(in);
            qte->repaint();
        }

        public:
            QtPTextWriter(QTextEdit& qte)
                : qte(&qte)
            {
            }
    };
}

#endif // INTEGRATION_H
