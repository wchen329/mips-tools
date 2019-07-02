#ifndef SIMUI_H
#define SIMUI_H

#include <QMainWindow>

namespace Ui {
class simUI;
}

class simUI : public QMainWindow
{
    Q_OBJECT

public:
    explicit simUI(QWidget *parent = 0);
    ~simUI();

private slots:
    void on_actionExit_triggered();

    void on_actionAbout_simUI_triggered();

private:
    Ui::simUI *ui;
};

#endif // SIMUI_H
