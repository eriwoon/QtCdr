#include "ccdrbaseinfo.h"

CCdrBaseInfo::CCdrBaseInfo()
{
    #ifdef DEBUG
    qDebug()<<"init CCdrBaseInfo ";
    #endif
    //*
    addTypeAttrPair("*","model");

    //name
    addTypeAttrPair(fileName(),"prefix");
    addTypeAttrPair(fileName(),"postfix");

    //field
    addTypeAttrPair(element(),"cpidnumber");
    addTypeAttrPair(element(),"cpid");
    addTypeAttrPair(element(),"color");
    addTypeAttrPair(element(),"elementname");
    addTypeAttrPair(element(),"desc");

    //cdr
    addTypeAttrPair(cdr(),"name");
    addTypeAttrPair(cdr(),"desc");

}

void CCdrBaseInfo::addTypeAttrPair(QString Type, QString Attr)
{
    this->MXmlTypeAttr.push_back(Type.toLower() + ":" + Attr.toLower());
}

bool CCdrBaseInfo::checkTypeAttr(QString Type, QString Attr)
{
    #ifdef DEBUG
    qDebug()<<"\t\t\tcheckTypeAttr:"<<Type<<":"<<Attr;
    #endif
    QString tmp(Type.toLower() + ":" + Attr.toLower());
    for(QVector<QString>::iterator iter = MXmlTypeAttr.begin(); iter != MXmlTypeAttr.end(); iter ++)
    {
        if(!iter->compare(tmp))
        {
            #ifdef DEBUG
            qDebug()<<"\t\t\tCheck result:Success";
            #endif
            return true;
        }
    }
    #ifdef DEBUG
    qDebug()<<"\t\tCheck result:Fail";
    #endif
    return false;
}

QString CCdrBaseInfo::fileName() const
{
    QString re("filename");
    return re;
}

QString CCdrBaseInfo::element() const
{
    QString re("element");
    return re;
}

QString CCdrBaseInfo::cdr() const
{
    QString re("cdr");
    return re;
}
