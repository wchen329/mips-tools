#include <memory>
#include "tools_meminspector.h"
#include "ui_tools_meminspector.h"
#include "mt_exception.h"
#include "range.h"
#include "streams.h"

tools_meminspector::tools_meminspector(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::tools_meminspector)
{
    ui->setupUi(this);
}

void tools_meminspector::addMemoryChannel(mips_tools::mem* m)
{
    this->memoryChannels.push_back(m);
}

tools_meminspector::~tools_meminspector()
{
    delete ui;
    this->resetTree();
}

void tools_meminspector::resetTree()
{
    /*
    for(std::vector<QTreeWidgetItem*>::iterator itr = mement_alloc.begin(); itr != mement_alloc.end(); itr++)
    {
        QTreeWidgetItem* d = *itr;
        delete d;
        mement_alloc.erase(itr);
    }

    for(std::vector<QTreeWidgetItem*>::iterator itr = channel_alloc.begin(); itr != channel_alloc.end(); itr++)
    {
        QTreeWidgetItem* d = *itr;
        delete d;
        channel_alloc.erase(itr);
    }*/
}

void tools_meminspector::on_pushButton_Display_clicked()
{
    this->ui->treeWidget_MemInspect->clear();

    for(size_t mcc = 0; mcc < this->memoryChannels.size(); mcc++)
    {
        QTreeWidgetItem * qtwi = new QTreeWidgetItem();
        qtwi->setText(0, "Channel 0");

        this->ui->treeWidget_MemInspect->addTopLevelItem(qtwi);
        this->channel_alloc.push_back(qtwi);

        std::string range_spec;
        std::unique_ptr<mips_tools::range> rs;

        try
        {
             range_spec = this->ui->lineEdit_BeginIndex->text().toStdString() + ":" + this->ui->lineEdit_EndingIndex->text().toStdString();
            rs = std::unique_ptr<mips_tools::range>(new mips_tools::range(range_spec));
        }
        catch(mips_tools::mt_exception& br)
        {
            return;
        }

        for(mips_tools::range_iterator itr = rs->begin(); itr != rs->end(); itr++)
        {
            if(*itr > memoryChannels[mcc]->get_size())
            {
                return;
            }

            QTreeWidgetItem* qtwic = new QTreeWidgetItem();
            qtwic->setText(0,   QString("mem[") +
                                QString(priscas_io::StrTypes::SizeToStr(*itr).c_str()) +
                                QString("]: ") +
                                QString(priscas_io::StrTypes::IntToStr((*memoryChannels[mcc])[*itr]).c_str())
                    );
            qtwi->addChild(qtwic);
            this->mement_alloc.push_back(qtwic);
        }
    }
}
