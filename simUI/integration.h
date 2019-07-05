#ifndef INTEGRATION_H
#define INTEGRATION_H
#include <QCoreApplication>
#include <QString>
#include <QTextEdit>
#include <string>
#include <iostream>
#include "simui.h"
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
        QTextEdit* qte;

        // disable constructors

        void operator<<(std::string& input) override;

        public:
            QtPTextWriter(QTextEdit& qte)
                : qte(&qte)
            {

            }
    };
}

#endif // INTEGRATION_H
