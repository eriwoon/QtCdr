#include "ccdrxmlprocess.h"
#include <QFile>
#include <QDebug>
//#include <QMessageBox>
//#include <Qdir>

#define XML_LOCATION "c:/cdrconfig.xml"

int CCdrXmlProcess::init(VCCdrDefines* pvcCdrDefines)
{
    LOG_FUNC(CCdrXmlProcess::init);

    CCdrBaseInfo BaseInfo;

    //打开xml配置文件
    QFile file;
    //QMessageBox mb;
    //mb.setText(QDir::currentPath() + "/cdrcomfig.xml");
    //mb.exec();
    file.setFileName("./cdrconfig.xml");
    if(!file.open(QIODevice::ReadOnly))
    {
        #ifdef DEBUG
        qDebug()<<"cannot find:cdrcomfig.xml at current dir"<<file.fileName();
        #endif
        file.setFileName(XML_LOCATION);
        if(!file.open(QIODevice::ReadOnly))
        {
            #ifdef DEBUG
            qDebug()<<"cannot find:"<<XML_LOCATION<<" at current dir, return FAIL"<<file.fileName();
            #endif
            return FAIL;
        }
    }
    #ifdef DEBUG
    qDebug()<<"filename:"<<file.fileName();
    #endif

    //创建一个新的节点用来处理
    QXmlStreamReader xmlReader(&file);

    CCdrDefine* cCdeDefine = 0;
    VCAttributes vcAttributes;

    for(QXmlStreamReader::TokenType type  = xmlReader.readNext();!xmlReader.atEnd(); type = xmlReader.readNext())
    {
        switch(type)
        {
        //开始节点
        case QXmlStreamReader::StartElement:
            #ifdef DEBUG
            qDebug()<<"\t\t\ttype = QXmlStreamReader::StartElement; name = "<<xmlReader.name();
            #endif
            //cdr表示一张话单的开始
            if(!xmlReader.name().compare("cdr"))
                //todo 这里处理一个话单开始的工作,比如创建一个新的对象
            {
                #ifdef DEBUG
                qDebug()<<"new cdr"<<xmlReader.name();
                #endif
                cCdeDefine = new CCdrDefine;

                QXmlStreamAttributes qStreamAttr = xmlReader.attributes();

                //遍历所有的属性,插入可以使用的属性
                for(QXmlStreamAttributes::iterator iter = qStreamAttr.begin();iter != qStreamAttr.end();iter++)
                {
                    if(BaseInfo.checkTypeAttr(BaseInfo.cdr(),iter->name().toString()))
                    {
                        cCdeDefine->vcCdrAttr.addOne(iter->name().toString(),iter->value().toString());
                    }
                }
            }
            else if(!xmlReader.name().compare("filename"))
                //如果这里是filename,就应该创建一个节点,用来存储相关的信息;
                ;
            else if(!xmlReader.name().compare("element"))
                //如果是element,表示是字段了.
                ;
            else if(!xmlReader.name().compare("name") && cCdeDefine != 0)
            {
                //清空上次使用的attributes
                vcAttributes.clear();

                QXmlStreamAttributes qStreamAttr = xmlReader.attributes();

                //在第一个位置添加这个节点的值
                vcAttributes.addOne(xmlReader.name().toString(),xmlReader.readElementText());

                //遍历所有的属性,插入可以使用的属性
                for(QXmlStreamAttributes::iterator iter = qStreamAttr.begin();iter != qStreamAttr.end();iter++)
                {
                    if(BaseInfo.checkTypeAttr(BaseInfo.fileName(),iter->name().toString()))
                    {
                        vcAttributes.addOne(iter->name().toString(),iter->value().toString());
                    }
                }
                #ifdef DEBUG
                qDebug()<<"\tinsert into NameDef, name = "<<xmlReader.name();
                #endif
                //插入到链表中去
                cCdeDefine->addNameDef(vcAttributes);


            }
            else if(!xmlReader.name().compare("field"))
            {
                //清空上次使用的attributes
                vcAttributes.clear();

                QXmlStreamAttributes qStreamAttr = xmlReader.attributes();
                #ifdef DEBUG
                qDebug()<<"\txmlReader.attributes()="<<qStreamAttr.size();
                #endif

                //在第一个位置添加这个节点的值
                vcAttributes.addOne(xmlReader.name().toString(),xmlReader.readElementText());

                //遍历所有的属性,插入可以使用的属性
                for(QXmlStreamAttributes::iterator iter = qStreamAttr.begin();iter != qStreamAttr.end();iter++)
                {
                    if(BaseInfo.checkTypeAttr(BaseInfo.element(),iter->name().toString()))
                    {
                        vcAttributes.addOne(iter->name().toString(),iter->value().toString());
                    }
                }

                #ifdef DEBUG
                qDebug()<<"\tinsert into field, name = "<<xmlReader.name();
                #endif
                //插入到链表中去
                cCdeDefine->addField(vcAttributes);
            }
            break;

        //结束节点
        case QXmlStreamReader::EndElement:
            if(!xmlReader.name().compare("cdr"))
                //todo 这里处理一个话单开始的工作,比如创建一个新的对象
            {
                if(cCdeDefine != 0)
                {
                    #ifdef DEBUG
                    qDebug()<<"push_back cdr"<<xmlReader.name();
                    #endif
                    pvcCdrDefines->push_back(*cCdeDefine);
                }
                delete cCdeDefine;
                cCdeDefine = 0;
            }

            else if(!xmlReader.name().compare("filename"))
                //如果这里是filename,就应该创建一个节点,用来存储相关的信息;
                ;
            else if(!xmlReader.name().compare("element"))
                //如果是element,表示是字段了.
                ;
            else if(!xmlReader.name().compare("name"))
                ;
            else if(!xmlReader.name().compare("field"))
                ;
            break;

        //对于其他类型的节点不用处理
        default:
            ;
        };
    }
    if(xmlReader.hasError())
    {
        qDebug()<<xmlReader.error()<<":"<<xmlReader.errorString();
    }

    return SUCCESS;
}
