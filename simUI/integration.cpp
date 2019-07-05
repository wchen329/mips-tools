#include "integration.h"

namespace priscas_io
{
    void QtPTextWriter::operator<<(std::string& input)
    {
        simulation::obuf_mutex.lock();
        simulation::output_buffer += input;
        simulation::obuf_mutex.unlock();
    }

}
