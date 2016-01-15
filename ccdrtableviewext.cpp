#include "ccdrtableviewext.h"
#include <QHeaderView>
CCDRTableViewExt::CCDRTableViewExt()
{
    ccdrdata = NULL;
    connect(this,SIGNAL(clicked(QModelIndex)),this,SLOT(onHearingClicked(QModelIndex)));
    //this->verticalHeader()->setDefaultSectionSize(20);
    this->horizontalHeader()->setDefaultSectionSize(100);
}

void CCDRTableViewExt::onHearingClicked(QModelIndex qModInd)
{
    if(qModInd.row() < this->ccdrdata->getCurCdrDefine()->vcFields.size())
    {
        QString sDesc = this->ccdrdata->getCurCdrDefine()->vcFields[qModInd.row()].getValue("desc");
        emit clicked(sDesc);
    }
}

void CCDRTableViewExt::setModel(CCdrData * ccdrdata)
{
    this->ccdrdata = ccdrdata;
    QTableView::setModel(ccdrdata);
    //this->resizeColumnToContents(1);
    //this->horizontalHeader()->setMinimumWidth(100);
    this->resize(1,2);
    this->resizeRowsToContents();
    this->setColumnWidth(0,200);
}
