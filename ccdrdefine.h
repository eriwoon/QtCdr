#ifndef CCDRDEFINE_H
#define CCDRDEFINE_H
#include <QString>
#include <QVector>
#include <QVariant>
#include "ccdrbaseinfo.h"


//declare the Attribute Class
class CAttribute
{
//public:
    //define new type
    //enum{INT,STRING};
    //data
public:
    QString sAttributeName;
    //int enumAttributeType;
    //int iAttributeValue;
    //QString sAttributeValue;
    QString sAttributeValue;

public:
    //初始化函数
    void init(QString,int);
    void init(QString,QString);
    void init(QString,QVariant);

public:
    //不带参数,赋值默认值
    CAttribute();
    //传入name和value,vlaue为int型
    //CAttribute(QString,int);
    //传入name和value,value为string型
    CAttribute(QString, QString);
//private:
    //CAttribute(QString,QVariant);
};

//Attributes类
class VCAttributes :public QVector<CAttribute>
{
public:
    //增加一个节点
    //CAttribute* addOne(QString,int);
    CAttribute* addOne(QString,QString);
    //CAttribute* addOne(QString,QVariant);

public:
    //获取一个节点的属性值
    QString getValue(QString);

};

class CCdrDefine
{
public:
    CCdrDefine(){}

public:
    //话单相关属性
    VCAttributes vcCdrAttr;

    //包含这个话单的前缀列表
    class VCCdrNameDef: public QVector<VCAttributes>
    {
    public:
        bool isFilePrefixMatch(QString);
    };
    VCCdrNameDef vcCdrNameDef;

    //第二个成员,包含这个话单各个字段的定义
    class VCFields: public QVector<VCAttributes>
    {
    private:
        int getFieldNumber();
        int getColumnNumber(QString);
        void getNextValuePosition(int&,int&);
    public:
        VCFields(){}
    };
    VCFields vcFields;

public:
    void addNameDef(VCAttributes);
    void addField(VCAttributes);

public:
    bool ifMatchedCDR(QString);
};

class VCCdrDefines : public QVector<CCdrDefine>
{
public:
    //增加一张话单
    CCdrDefine* addOne();
    //找到与对应的文件名对应的话单文件.
    CCdrDefine* findMatchedCDR(QString);
};

#endif // CCDRDEFINE_H
