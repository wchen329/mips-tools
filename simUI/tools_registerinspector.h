#ifndef TOOLS_REGISTERINSPECTOR_H
#define TOOLS_REGISTERINSPECTOR_H

#include <QDialog>
#include <QHeaderView>
#include <QTreeWidgetItem>
#include <vector>
#include "diag_cpu.h"
#include "streams.h"

namespace Ui {
class tools_registerInspector;
}

class tools_registerInspector : public QDialog
{
    Q_OBJECT

public:
    explicit tools_registerInspector(QWidget *parent = 0);
    ~tools_registerInspector();
    void addCPU(mips_tools::diag_cpu& dcpu);
private:
    Ui::tools_registerInspector *ui;
    std::vector<QTreeWidgetItem*> regNode_allocList;
    std::vector<QTreeWidgetItem*> cpuNode_allocList;
    int cpu_count;
    bool accepted;
};

#endif // TOOLS_REGISTERINSPECTOR_H
