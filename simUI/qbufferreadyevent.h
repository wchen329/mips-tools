#ifndef QBUFFERREADYEVENT_H
#define QBUFFERREADYEVENT_H
#include <QEvent>

const int QEventReady = 1002;

class QBufferReadyEvent : public QEvent
{

    public:
        QBufferReadyEvent(std::string& buf_in) : buf(buf_in), QEvent(static_cast<Type>(QEventReady)) {}
        std::string& getContent() { return this->buf; }

    private:
        std::string buf;
};

#endif // QBUFFERREADYEVENT_H
