#include "integration.h"

namespace priscas_io
{
    void QtPTextWriter::operator<<(std::string& input)
    {
        this->qm.lock();
        rep += input;
        this->qm.unlock();
    }

    void QtPTextWriter::operator>>(std::string& output)
    {
        this->qm.lock();
        output += rep;
        rep.clear();
        this->qm.unlock();
    }

}
