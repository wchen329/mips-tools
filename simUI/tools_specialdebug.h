#ifndef TOOLS_SPECIALDEBUG_H
#define TOOLS_SPECIALDEBUG_H

#include <QDialog>
#include <QTreeView>
#include <QTableView>
#include <QTreeWidget>
#include "diag_cpu.h"

namespace Ui {
class tools_specialdebug;
}

class tools_specialdebug : public QDialog
{
    Q_OBJECT

public:
    explicit tools_specialdebug(QWidget *parent = 0);
    void setCPU(mips_tools::diag_cpu& dcpu);
    ~tools_specialdebug();

private:
    Ui::tools_specialdebug *ui;
};

#endif // TOOLS_SPECIALDEBUG_H
