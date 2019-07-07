#ifndef TOOLS_SPECIALDEBUG_H
#define TOOLS_SPECIALDEBUG_H

#include <QDialog>

namespace Ui {
class tools_specialdebug;
}

class tools_specialdebug : public QDialog
{
    Q_OBJECT

public:
    explicit tools_specialdebug(QWidget *parent = 0);
    ~tools_specialdebug();

private:
    Ui::tools_specialdebug *ui;
};

#endif // TOOLS_SPECIALDEBUG_H
