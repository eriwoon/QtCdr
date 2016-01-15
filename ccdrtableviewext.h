#ifndef CCDRTABLEVIEWEXT_H
#define CCDRTABLEVIEWEXT_H
#include <QTableView>
#include "ccdrdata.h"

class CCDRTableViewExt : public QTableView
{
    Q_OBJECT

public:
    CCDRTableViewExt();

signals:
    void clicked(QString s);

private slots:
    void onHearingClicked(QModelIndex);

public:
    void setModel(CCdrData *);

private:
    CCdrData* ccdrdata;
};

#endif // CCDRTABLEVIEWEXT_H
