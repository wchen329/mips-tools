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
    std::queue<int>& getRemoveReplay_arch() { return this->ModelreplayQueue_arch;}
    std::queue<int>& getRemoveReplay_prog() { return this->ModelreplayQueue_prog; }
    ~listbreak();

private slots:
    void on_PBP_removeButton_clicked();
    void on_ABP_removeButton_clicked();

private:
    Ui::listbreak *ui;
    std::queue<int> ModelreplayQueue_arch;
    std::queue<int> ModelreplayQueue_prog;

};

#endif // LISTBREAK_H
