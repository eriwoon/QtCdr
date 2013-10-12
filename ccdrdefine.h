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
    //��ʼ������
    void init(QString,int);
    void init(QString,QString);
    void init(QString,QVariant);

public:
    //��������,��ֵĬ��ֵ
    CAttribute();
    //����name��value,vlaueΪint��
    //CAttribute(QString,int);
    //����name��value,valueΪstring��
    CAttribute(QString, QString);
//private:
    //CAttribute(QString,QVariant);
};

//Attributes��
class VCAttributes :public QVector<CAttribute>
{
public:
    //����һ���ڵ�
    //CAttribute* addOne(QString,int);
    CAttribute* addOne(QString,QString);
    //CAttribute* addOne(QString,QVariant);

public:
    //��ȡһ���ڵ������ֵ
    QString getValue(QString);

};

class CCdrDefine
{
public:
    CCdrDefine(){}

public:
    //�����������
    VCAttributes vcCdrAttr;

    //�������������ǰ׺�б�
    class VCCdrNameDef: public QVector<VCAttributes>
    {
    public:
        bool isFilePrefixMatch(QString);
    };
    VCCdrNameDef vcCdrNameDef;

    //�ڶ�����Ա,����������������ֶεĶ���
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
    //����һ�Ż���
    CCdrDefine* addOne();
    //�ҵ����Ӧ���ļ�����Ӧ�Ļ����ļ�.
    CCdrDefine* findMatchedCDR(QString);
};

#endif // CCDRDEFINE_H
