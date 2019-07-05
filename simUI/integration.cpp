#include "integration.h"

namespace priscas_io
{
    void QtPTextWriter::operator<<(std::string& input)
    {
        QString in = input.c_str();
        QBufferReadyEvent* qbre;

        //QCoreApplication::sendEvent(progInst, qbre);
    }

}
