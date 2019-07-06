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
    void setSelectedCPUID(int);
    void setMemBitWidth(int);

private slots:
    void on_horizontalSliderMemBits_valueChanged(int value);

private:
    Ui::simoptionsdialog *ui;
};

#endif // SIMOPTIONSDIALOG_H
