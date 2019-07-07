#ifndef TOOLS_MEMINSPECTOR_H
#define TOOLS_MEMINSPECTOR_H

#include <QDialog>
#include <QTreeWidgetItem>
#include "mem.h"

namespace Ui {
class tools_meminspector;
}

class tools_meminspector : public QDialog
{
    Q_OBJECT

public:
    explicit tools_meminspector(QWidget *parent = 0);
    ~tools_meminspector();
    void addMemoryChannel(mips_tools::mem* m);

private slots:
    void on_pushButton_Display_clicked();

private:
    Ui::tools_meminspector *ui;
    std::vector<mips_tools::mem*> memoryChannels;
    std::vector<QTreeWidgetItem*> channel_alloc;
    std::vector<QTreeWidgetItem*> mement_alloc;
    void resetTree();
};

#endif // TOOLS_MEMINSPECTOR_H
