#ifndef CCDRDATA_H
#define CCDRDATA_H
#include <QAbstractTableModel>
#include <ccdrdefine.h>

//构造一个VCAttributes类型的二维数组(实际就是一个Attribute类型的三维数组)
class VCBranch : public QVector<QString>
{
public:
    QString* addOne();
};
class VCTrunk :public QVector<VCBranch>
{
public:
    VCBranch* addOne();
};

class CCdrData : public QAbstractTableModel
{
public:
    CCdrData(QObject *parent = 0)
        :QAbstractTableModel(parent){}
    
    CCdrData(CCdrDefine* pcCdrDefine, QString sReadLine, QObject *parent = 0)
        :QAbstractTableModel(parent)
    {
        initCdrData(pcCdrDefine, sReadLine);
    //    this->re
    }
    
private:
    VCTrunk vcTrunk;
    //如果话单记录无法解码,解析错误,这里记录错误描述;
    QString sErrorDesc;
    CCdrDefine* pcCdrDefine;

public:
    int initCdrData(CCdrDefine*, QString);
    QString getErrorDesc(){return sErrorDesc;}

    //parent参数对于table没有任何意义
    int rowCount(const QModelIndex &/*parent*/) const;
    int columnCount(const QModelIndex &/*parent*/) const;
    
    //data函数根据不同的情况返回不同的
    QVariant data(const QModelIndex &index, int role) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    
    CCdrDefine* getCurCdrDefine();
};

#endif // CCDRDATA_H
