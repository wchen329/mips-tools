#ifndef ADDBREAK_H 
#define ADDBREAK_H 

#include <QDialog>

namespace Ui {
class addbreak;
}

class addbreak: public QDialog
{
    Q_OBJECT

public:
    explicit addbreak(QWidget *parent = 0);
    bool isArchBreakPoint();
    bool isProgBreakPoint();
    QString getTextContent();
    ~addbreak();

private:
    Ui::addbreak*ui;
};

#endif // ADDBREAK_H 
