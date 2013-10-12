#ifndef CCDRBASEINFO_H
#define CCDRBASEINFO_H

#include <QVector>
#include <QString>
#include <QDebug>
#define QT_NO_ICU
#define QT_NO_QLOCALE
#define FAIL    -1
#define SUCCESS 0
#define COPYRIGHTINFO "Copyright:\nXiao Zhen\nCDR Reader v0.4\n2013.06.25"
//#define DEBUG
/*
QDebug mydebug()
{
    static int i = 0;
    QDebug qdebug = qDebug();
    qdebug <<"["<< i++ <<"]:";
    return qdebug;
}
*/


class CCdrBaseInfo
{
public:
    CCdrBaseInfo();

private:
    //xml的节点类型和节点可选属性的对应关系
    QVector<QString> MXmlTypeAttr;
public:
    //检查一个节点是否可以拥有一个属性
    bool checkTypeAttr(QString,QString);
public:
    QString fileName() const;
    QString element() const;
    QString cdr() const;

private:
    //不允许动态增加关系
    void addTypeAttrPair(QString,QString);

};

#endif // CCDRBASEINFO_H
