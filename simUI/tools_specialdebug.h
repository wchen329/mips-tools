#ifndef TOOLS_SPECIALDEBUG_H
#define TOOLS_SPECIALDEBUG_H

#include <QDialog>
#include <QTreeView>
#include <QTableView>
#include <QTreeWidget>
#include <vector>
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


private slots:
    void on_comboBox_Views_currentIndexChanged(int index);

private:
    Ui::tools_specialdebug *ui;
    std::vector<QWidget*> views;
    QWidget* foregroundWidget;
    std::vector<QWidget*> wList;
    void setWidgetInForeground(QWidget *);
};

#endif // TOOLS_SPECIALDEBUG_H
