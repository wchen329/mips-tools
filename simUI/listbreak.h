#ifndef LISTBREAK_H
#define LISTBREAK_H

#include <QDialog>
#include <queue>

namespace Ui {
class listbreak;
}

class listbreak : public QDialog
{
    Q_OBJECT

public:
    explicit listbreak(QWidget *parent = 0);
    listbreak(std::vector<unsigned long>& abps, std::vector<unsigned long>& pbps, QWidget* parent = 0);
    ~listbreak();

private slots:
    void on_PBP_removeButton_clicked();

private:
    Ui::listbreak *ui;

};

#endif // LISTBREAK_H
