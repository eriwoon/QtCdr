#include "ccdrdefine.h"
/*
void CAttribute::init(QString sAttributeName, QVariant varAttributeValue)
{
    this->sAttributeName = sAttributeName;
    this->varAttributeValue = varAttributeValue;
}
*/
/*
void CAttribute::init(QString sAttributeName,int iAttributeValue)
{
    QVariant re = iAttributeValue;
    init(sAttributeName,re);
}
*/
void CAttribute::init(QString sAttributeName,QString sAttributeValue)
{
    //QVariant re = sAttributeValue;
    //init(sAttributeName,re);
    this->sAttributeName = sAttributeName;
    this->sAttributeValue = sAttributeValue;
}

CAttribute::CAttribute()
{}
/*
CAttribute::CAttribute(QString sAttributeName,int iAttributeValue)
{
    init(sAttributeName,iAttributeValue);
}
*/
CAttribute::CAttribute(QString sAttributeName, QString sAttributeValue)
{
    init(sAttributeName,sAttributeValue);
}
/*
CAttribute* VCAttributes::addOne(QString sAttributeName, int iAttributeValue)
{
    QVariant var = iAttributeValue;
    return addOne(sAttributeName,var);
}
*/
CAttribute* VCAttributes::addOne(QString sAttributeName, QString sAttributeValue)
{
    #ifdef DEBUG
    qDebug()<<"VCAttributes::addOne sAttributeName="<<sAttributeName<<"sAttributeValue="<<sAttributeValue;
    #endif

    this->resize(this->size()+1);
    this->last().init(sAttributeName,sAttributeValue);

    #ifdef DEBUG
    qDebug()<<"\taddOne result:sAttributeName="
           << this->last().sAttributeName
           <<"sAttributeValue="
          << this->last().sAttributeValue;
    #endif

    return &(this->last());
}
/*
CAttribute* VCAttributes::addOne(QString sAttributeName,QVariant varAttributeValue)
{
    this->resize(this->size()+1);
    this->last().init(sAttributeName,varAttributeValue);
    return &(this->last());
}
*/
QString VCAttributes::getValue(QString sAttributeName)
{
    #ifdef DEBUG
    qDebug()<<"VCAttributes::getValue: target = "<< sAttributeName;
    #endif

    for(VCAttributes::iterator iter = this->begin();iter != this->end(); iter++)
    {
        #ifdef DEBUG
        qDebug()<<"\tcurrent Node:iter->sAttributeName="
                  <<iter->sAttributeName;
        #endif

        if(iter->sAttributeName == sAttributeName)
        {
            #ifdef DEBUG
            qDebug()<<"\tresult:success";
            #endif
            return iter->sAttributeValue;
        }
    #ifdef DEBUG
    qDebug()<<"\tresult:fail";
    #endif
    }
    #ifdef DEBUG
    qDebug()<<"\tVCAttributes::getValue:not find";
    #endif
    QString re;
    return re;
}

CCdrDefine* VCCdrDefines::addOne()
{
    this->resize(this->size()+1);
    return &(this->last());
}

void CCdrDefine::addNameDef(VCAttributes vcAttributes)
{
    #ifdef DEBUG
    qDebug()<<"\tAttributesDetails:"<<vcAttributes.size();
    #endif
    vcCdrNameDef.append(vcAttributes);
}
void CCdrDefine::addField(VCAttributes vcAttributes)
{
    #ifdef DEBUG
    qDebug()<<"\tAttributesDetails:"<<vcAttributes.size();
    #endif
    vcFields.append(vcAttributes);
}

CCdrDefine* VCCdrDefines::findMatchedCDR(QString sFileName)
{
    qDebug()<<"I get Here:3. sFileName="<<sFileName<<" __ sizeof(this)="<<this->size();

    CCdrDefine* iter = this->begin();
    for(;iter != this->end();iter++)
    {
        if(iter->ifMatchedCDR(sFileName))
            return iter;
    }

    return 0;
}

bool CCdrDefine::ifMatchedCDR(QString sFileName)
{
    qDebug()<<"I get Here:4 ";
    for(VCCdrNameDef::iterator iter = vcCdrNameDef.begin()
        ;iter != vcCdrNameDef.end()
        ;iter++)
    {
        QString sName = iter->getValue("name");
        QString sMode = iter->getValue("mode");
        #ifdef DEBUG
        qDebug()<<"CCdrDefine::ifMatchedCDR name="<<sName<<"mode="<<sMode;
        #endif
        if(!sMode.compare("postfix"))
        {
            qDebug()<<"we don't support postfix currently!";
        }
        //其他都是后缀
        else
        {
            #ifdef DEBUG
            qDebug()<<"\tCCdrDefine::ifMatchedCDR sFileName.left(sName.size())="
                    <<sFileName.left(sName.size())
                    <<"sName="<<sName;
            #endif
            if(sFileName.left(sName.size()) == sName)
            {
                #ifdef DEBUG
                qDebug()<<"\t\tresult:success";
                #endif
                return true;
            }
            #ifdef DEBUG
            qDebug()<<"\t\tresult:fail";
            #endif
        }
    }

    return false;
}




