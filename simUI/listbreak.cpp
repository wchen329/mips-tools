#include "listbreak.h"
#include "ui_listbreak.h"
#include "streams.h"

listbreak::listbreak(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::listbreak)
{
    ui->setupUi(this);
}

listbreak::listbreak(std::vector<unsigned long>& abps, std::vector<unsigned long>& pbps, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::listbreak)
{
    ui->setupUi(this);

    for(size_t abp_i = 0; abp_i < abps.size(); abp_i++)
    {
        this->ui->ABP_List->addItem(QString("[Arch. Breakpoint ") + QString(priscas_io::StrTypes::SizeToStr(abp_i).c_str()) + QString("] - ")
                                    + QString("Cycle ") + QString(priscas_io::StrTypes::UInt32ToStr(abps[abp_i]).c_str()));
    }

    for(size_t pbp_i = 0; pbp_i < pbps.size(); pbp_i++)
    {
        this->ui->PBP_List->addItem(QString("[Program Breakpoint ") + QString(priscas_io::StrTypes::SizeToStr(pbp_i).c_str()) + QString("] - ")
                                    + QString("Line ") + QString(priscas_io::StrTypes::UInt32ToStr(pbps[pbp_i]).c_str()));
    }
}

listbreak::~listbreak()
{
    delete ui;
}

void listbreak::on_PBP_removeButton_clicked()
{
    QList<QListWidgetItem*> w = this->ui->PBP_List->selectedItems();
    if(w.empty())
        return;
    else
    {
        for(int ws = 0; ws < w.size(); ws++)
        {

            delete w[ws];
        }
    }
}
