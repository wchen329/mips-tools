#include <memory>
#include "tools_meminspector.h"
#include "ui_tools_meminspector.h"
#include "mt_exception.h"
#include "primitives.h"
#include "range.h"
#include "streams.h"

tools_meminspector::tools_meminspector(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::tools_meminspector)
{
    ui->setupUi(this);
    QStringList * qsl = new QStringList();
    this->setLayout(this->ui->verticalLayoutMemInspect);
    qsl->append("Name");
    qsl->append("Signed Integer");
    qsl->append("Unsigned Integer");
    qsl->append("Floating Point");
    qsl->append("Hex");
    this->ui->treeWidget_MemInspect->setHeaderLabels(*qsl);
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

        bool READ_ONE = this->ui->radioButton_1B->isChecked();
        bool READ_TWO = this->ui->radioButton_2B->isChecked();
        bool READ_FOUR = this->ui->radioButton_4B->isChecked();;

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

            if(READ_ONE)
            {
                qtwic->setText(0,   QString("mem[") +
                                    QString(priscas_io::StrTypes::SizeToStr(*itr).c_str()) +
                                    QString("] ")
                        );
                qtwic->setText(1,   priscas_io::StrTypes::IntToStr((*memoryChannels[mcc])[*itr]).c_str());
                qtwic->setText(2,   priscas_io::StrTypes::UIntToStr((*memoryChannels[mcc])[*itr]).c_str());
                qtwic->setText(3,   "N/A");
                qtwic->setText(4,   mips_tools::genericHexBuilder<int, 8>((*memoryChannels[mcc])[*itr]).c_str());
            }

            else if(READ_TWO)
            {
                size_t itrval = *itr;
                if(itrval % 2 != 0 || (itrval + 1) > memoryChannels[mcc]->get_size()) continue;
                qtwic->setText(0,   QString("mem[") +
                                    QString(priscas_io::StrTypes::SizeToStr(itrval + 1).c_str()) +
                                    QString(":") +
                                    QString(priscas_io::StrTypes::SizeToStr(itrval).c_str()) +
                                    QString("] ")
                        );

                mips_tools::BW_16 value = mips_tools::BW_16((*memoryChannels[mcc])[*itr], (*memoryChannels[mcc])[(*itr)+1]);
                qtwic->setText(1, priscas_io::StrTypes::IntToStr(value.AsInt16()).c_str());
                qtwic->setText(2, priscas_io::StrTypes::UIntToStr(value.AsUInt16()).c_str());
                qtwic->setText(3, "N/A");
                qtwic->setText(4, value.toHexString().c_str());
            }

            else if(READ_FOUR)
            {
                size_t itrval = *itr;
                if(itrval % 4 != 0 || (itrval + 3) > memoryChannels[mcc]->get_size()) continue;
                qtwic->setText(0,   QString("mem[") +
                                    QString(priscas_io::StrTypes::SizeToStr(itrval + 3).c_str()) +
                                    QString(":") +
                                    QString(priscas_io::StrTypes::SizeToStr(itrval).c_str()) +
                                    QString("] ")
                        );

                mips_tools::BW_32 value = mips_tools::BW_32((*memoryChannels[mcc])[*itr], (*memoryChannels[mcc])[(*itr)+1],
                        (*memoryChannels[mcc])[*itr + 2], (*memoryChannels[mcc])[*itr + 3]);
                qtwic->setText(1, priscas_io::StrTypes::Int32ToStr(value.AsInt32()).c_str());
                qtwic->setText(2, priscas_io::StrTypes::UInt32ToStr(value.AsUInt32()).c_str());
                qtwic->setText(3, priscas_io::StrTypes::FloatToStr(value.AsSPFloat()).c_str());
                qtwic->setText(4, value.toHexString().c_str());
            }

            qtwi->addChild(qtwic);
            this->mement_alloc.push_back(qtwic);
        }
    }
}
