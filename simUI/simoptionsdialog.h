#ifndef SIMOPTIONSDIALOG_H
#define SIMOPTIONSDIALOG_H

#include <QDialog>

namespace Ui {
class simoptionsdialog;
}

class simoptionsdialog : public QDialog
{
    Q_OBJECT

public:
    explicit simoptionsdialog(QWidget *parent = 0);
    ~simoptionsdialog();
    int getSelectedCPUID();
    int getMemBitWidth();

private:
    Ui::simoptionsdialog *ui;
};

#endif // SIMOPTIONSDIALOG_H
