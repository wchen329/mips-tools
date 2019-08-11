#include "debug_view.h"
#include "debug_view_simple.h"
#include "tools_specialdebug.h"
#include "ui_tools_specialdebug.h"

tools_specialdebug::tools_specialdebug(QWidget *parent) :
    QDialog(parent),
    foregroundWidget(nullptr),
    ui(new Ui::tools_specialdebug)
{
    ui->setupUi(this);
    this->setLayout(ui->verticalLayoutSpecialDebug);
}

void tools_specialdebug::setCPU(mips_tools::diag_cpu& dcpu)
{
    if(dcpu.get_DebugViews().size() != 0)
    {
        delete ui->label_Default;
        ui->verticalSpacer_LaD->changeSize(0,0);
        ui->verticalSpacer_LaU->changeSize(0,0);

        // Add all the views.

        QWidget * addable = nullptr;

        std::vector<mips_tools::DebugView*>& dbg = dcpu.get_DebugViews();

        for(size_t w = 0; w < dbg.size(); w++)
        {
            switch(dbg[w]->getDBGType())
            {
                case mips_tools::DebugView::SIMPLE_TREE_LIST:
                {
                    QStringList * qsl = new QStringList;
                    mips_tools::DebugTree_Simple_List * dbgsl = dynamic_cast<mips_tools::DebugTree_Simple_List*>(dbg[w]);
                    std::string viewName = dbgsl->getName();
                    qsl->append(viewName.c_str());
                    QTreeWidget * qtw = new QTreeWidget();
                    qtw->setHeaderLabels(*qsl);
                    addable = qtw;

                    for(size_t ind = 0; ind < dbgsl->get_DebugTrees().size(); ind++)
                    {
                        QTreeWidgetItem * qtwi = new QTreeWidgetItem();
                        qtwi->setText(0, dbgsl->get_DebugTrees()[ind]->rootNode().getName().c_str());
                        qtw->addTopLevelItem(qtwi);

                        std::list<mips_tools::DebugTreeNode_Simple*>& rc = dbgsl->get_DebugTrees()[ind]->rootNode().getAllChildren();
                        if(!rc.empty())
                        {
                            for(std::list<mips_tools::DebugTreeNode_Simple*>::iterator ca = rc.begin(); ca != rc.end(); ca++)
                            {
                                mips_tools::DebugTreeNode_Simple* t = *ca;
                                QTreeWidgetItem * qtwi_c = new QTreeWidgetItem();
                                qtwi_c->setText(0, QString(t->getName().c_str()) + QString(": ") + QString(t->getValue().c_str()));
                                qtwi->addChild(qtwi_c);
                            }
                        }
                    }

                    ui->comboBox_Views->addItem(viewName.c_str());
                    wList.push_back(qtw);
                    break;
                }

                case mips_tools::DebugView::TABLE:
                {
                    QTableWidget * qtw = new QTableWidget;
                    mips_tools::DebugTableStringValue* dbs = dynamic_cast<mips_tools::DebugTableStringValue*>(dbg[w]);
                    ui->comboBox_Views->addItem("Pipeline Diagram");
                    wList.push_back(qtw);
                    qtw->setRowCount(dbs->getMaxDefY());\
                    qtw->setColumnCount(dbs->getMaxDefX());
                    const std::vector<mips_tools::TablePointStringV>& tbp = dbs->getDefinedPtList();
                    for(size_t tc = 0; tc < tbp.size(); tc++)
                    {
                        mips_tools::TablePointStringV pt = tbp[tc];
                        int xcoord = static_cast<int>(pt.getX());
                        int ycoord = static_cast<int>(pt.getY());

                        // Make a QPoint out of this point to index into the actual table
                        QTableWidgetItem * itm = new QTableWidgetItem();
                        itm->setText(pt.getData().c_str());

                        // Then place it there.
                        qtw->setItem(ycoord, xcoord, itm);

                    }


                    break;
                }
            }
        }

        this->setWidgetInForeground(addable);
    }
}

tools_specialdebug::~tools_specialdebug()
{
    delete ui;

    for(size_t w = 0; w < this->wList.size(); w++)
    {
        delete wList[w];
    }
}

void tools_specialdebug::setWidgetInForeground(QWidget * fwidget)
{
    // First background other view if possible
    if(foregroundWidget != nullptr)
    {
        ui->verticalLayoutSpecialDebug->removeWidget(foregroundWidget);
        foregroundWidget->setVisible(false);
    }

    ui->verticalLayoutSpecialDebug->addWidget(fwidget);
    fwidget->setVisible(true);
    foregroundWidget = fwidget;
}

void tools_specialdebug::on_comboBox_Views_currentIndexChanged(int index)
{
    if(!wList.empty())
        this->setWidgetInForeground(wList[index]);
}
