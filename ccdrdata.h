#ifndef CCDRDATA_H
#define CCDRDATA_H
#include <QAbstractTableModel>
#include <ccdrdefine.h>

//����һ��VCAttributes���͵Ķ�ά����(ʵ�ʾ���һ��Attribute���͵���ά����)
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
    //���������¼�޷�����,��������,�����¼��������;
    QString sErrorDesc;
    CCdrDefine* pcCdrDefine;

public:
    int initCdrData(CCdrDefine*, QString);
    QString getErrorDesc(){return sErrorDesc;}

    //parent��������tableû���κ�����
    int rowCount(const QModelIndex &/*parent*/) const;
    int columnCount(const QModelIndex &/*parent*/) const;
    
    //data�������ݲ�ͬ��������ز�ͬ��
    QVariant data(const QModelIndex &index, int role) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    
    CCdrDefine* getCurCdrDefine();
};

#endif // CCDRDATA_H
