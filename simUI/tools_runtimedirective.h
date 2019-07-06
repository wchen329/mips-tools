#ifndef TOOLS_RUNTIMEDIRECTIVE_H
#define TOOLS_RUNTIMEDIRECTIVE_H

#include <QDialog>

namespace Ui {
class tools_runtimeDirective;
}

class tools_runtimeDirective : public QDialog
{
    Q_OBJECT

public:
    explicit tools_runtimeDirective(QWidget *parent = 0);
    QString getTextValue();
    ~tools_runtimeDirective();

private slots:
    void on_Run_button_clicked();

private:
    Ui::tools_runtimeDirective *ui;
};

#endif // RUNTIMEDIRECTIVE_H
