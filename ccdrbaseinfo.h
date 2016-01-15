#ifndef CCDRBASEINFO_H
#define CCDRBASEINFO_H

#include <QVector>
#include <QString>
#include <QDebug>
#define QT_NO_ICU
#define QT_NO_QLOCALE
#define FAIL    -1
#define SUCCESS 0
#define COPYRIGHTINFO "CDR Reader v0.5\n2016.1.14\n\nCopyright (C) 2016 Xiao Zhen\nThe MIT License (MIT)\nhttp://mit-license.org/"
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
class CLog
{
public:
    CLog(QString sFuncName):sFuncName(sFuncName){qDebug() << "Enter Func:" << sFuncName;}
    ~CLog(){qDebug() << "Exit Func:" << sFuncName;}
private:
    QString sFuncName;
};
#define LOG_FUNC(FuncName) CLog _log(#FuncName)

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
