#include "ccdrdata.h"
#include <QBrush>
#include <QSize>
int CCdrData::initCdrData(CCdrDefine* pcCdrDefine, QString sReadLine)
{
    #ifdef DEBUG
    qDebug()<<"CCdrData::initCdrData start";
    #endif
    this->pcCdrDefine = pcCdrDefine;

    //如果没有字段的话,就直接返回
    if(pcCdrDefine->vcFields.size() <= 0)
    {
        sErrorDesc = "CDR define is empty!";
        qDebug()<<sErrorDesc;
        return FAIL;
    }


    int currentPos = 0;
    int nextPos = 0;

    //所要用到的变量的定义
    QString sFieldWord;

    //接收当前的VCBranck
    VCBranch* pvcBranch = 0;

    int iCPIDNumber = 0;  //表示CPID一共有多少组
    int iCPIDPos    = -1; //表示上一个CPID开始的位置(因为数组的长度在增长,所以这里不能用指针记录,只能用相对位置来记录)

    //用来获取话单属性的值
    QString sValue;

    bool bOK = true;//标志从string转换成int是否成功.

    #ifdef DEBUG
    qDebug()<<"CCdrData::initCdrData : while cycle: sReadLine = "<<sReadLine;
    #endif
    //一直往前找下一个"|"所处的位置
    while((nextPos = sReadLine.indexOf("|",currentPos) )>0)
    {
        //取出当前的值
        sFieldWord = sReadLine.mid(currentPos,nextPos - currentPos);
        currentPos = nextPos + 1;
        #ifdef DEBUG
        qDebug()<<"\t***EnterDataPocess, current Node = "<<sFieldWord;
        #endif

        //首先区分两种情况,当前应该是CPID的还是不应该是CPID的位置,也就是,当前的CPID的number的值是0还是非零
        //如果当前的CPID的数量还没有用完,则需要继续往CPID的组里记录
        if(iCPIDNumber > 0)
        {
            //保证[]里面是有效的值,如果超出有效,说明话单字段个数和unl文件不一致
            if(pvcBranch - this->vcTrunk.begin() + 1>= pcCdrDefine->vcFields.size())
            {
                //这种情况是当前已经是话单的结尾,但是CPID依旧有值,说明话单最后以CPID结尾
                sValue.clear();
            }
            else
            {
                sValue = pcCdrDefine->vcFields[pvcBranch - this->vcTrunk.begin() + 1].getValue("cpid");
            }

            //如果这个还是CPID的节点的话,继续往下延续
            if(!sValue.compare("member"))
            {
                //先确定下一个是否存在,如果已经存在
                if(pvcBranch +1 != this->vcTrunk.end())
                {
                    pvcBranch++;
                    *(pvcBranch->addOne())=sFieldWord;

                    #ifdef DEBUG
                    qDebug()<<"\tADDDATA TYPE=1: line "
                           <<pvcBranch - this->vcTrunk.begin()
                           <<" no "
                           <<pvcBranch->size();
                    #endif
                }
                //如果不存在
                else
                {
                    //将当前的字段加到话单上去
                    pvcBranch = this->vcTrunk.addOne();
                    *(pvcBranch->addOne()) = sFieldWord;
                    #ifdef DEBUG
                    qDebug()<<"\tADDDATA TYPE=2: line "
                           <<pvcBranch - this->vcTrunk.begin()
                           <<" no "
                           <<pvcBranch->size();
                    #endif
                }
            }
            //如果下一个字段已经不是member了,那就要重新回到开始的地方重新写
            else
            {
                iCPIDNumber --;

                //如果当前的位置就是之前的pos的位置,说明没有CPID字段
                if(this->vcTrunk.begin() + iCPIDPos == pvcBranch)
                {
                    sErrorDesc = "xml file is not OK, CPID number without member";
                    qDebug()<<sErrorDesc;
                }

                if(iCPIDNumber > 0)
                {
                    pvcBranch = this->vcTrunk.begin() + iCPIDPos + 2;
                    *(pvcBranch->addOne()) = sFieldWord;
                    #ifdef DEBUG
                    qDebug()<<"\tADDDATA TYPE=3: line "
                           <<pvcBranch - this->vcTrunk.begin()
                           <<" no "
                           <<pvcBranch->size();
                    #endif
                }
            }
        }

        //如果不是CPID,则应该继续向下走
        //这里不用else是因为上面有一种情况,会做iCPIDNumber的--
        if(iCPIDNumber == 0)
        {
            //获取下一个节点的属性
            if(pvcBranch == 0)
            {
                sValue = pcCdrDefine->vcFields[0].getValue("cpid");
            }
            else
            {
                //保证[]里面是有效的值,如果超出有效,说明话单字段个数和unl文件不一致
                if(pvcBranch - this->vcTrunk.begin() + 1 >= pcCdrDefine->vcFields.size())
                {
                    sErrorDesc = "Exceed The Max number!!!";
                    qDebug()<<sErrorDesc;
                    return FAIL;
                }
                sValue = pcCdrDefine->vcFields[pvcBranch - this->vcTrunk.begin() + 1].getValue("cpid");
            }

            //如果直接是member,说明刚刚的cpid字段的值是0,则继续寻找下一个不是member的位置
            while(!sValue.compare("member"))
            {
                pvcBranch=this->vcTrunk.addOne();
                //同时,继续获得下一个节点的位置
                //获取下一个节点的属性
                //当然,依旧要保证[]里面是有效的值,如果超出有效,说明话单字段个数和unl文件不一致
                if(pvcBranch - this->vcTrunk.begin() + 1 >= pcCdrDefine->vcFields.size())
                {
                    sErrorDesc = "Exceed The Max number!!!";
                    qDebug()<<sErrorDesc;
                    return FAIL;
                }
                sValue = pcCdrDefine->vcFields[pvcBranch - this->vcTrunk.begin() + 1].getValue("cpid");
            }


            //如果当前是CPID的字段的话,记录这个CPID的值
            if(!sValue.compare("number"))
            {
                iCPIDNumber = sFieldWord.toInt(&bOK);
                if(!bOK){
                    sErrorDesc = "CdrIsNotGood, CpidIsNotANumber!!!!";
                    qDebug()<<sErrorDesc;
                    return FAIL;
                }
                //记录下来CPID开始的那个字段,因为VCTrunk是一个vector类型的变量,所以一直在增长,不能使用指针,只能记录相对位置
                //用来给一个CPID的组结束后,回到开始的地方
                iCPIDPos = pvcBranch - this->vcTrunk.begin();
            }

            //将当前的字段加到话单上去
            pvcBranch = this->vcTrunk.addOne();
            *(pvcBranch->addOne()) = sFieldWord;
            #ifdef DEBUG
            qDebug()<<"\tADDDATA TYPE=4: line "
                   <<pvcBranch - this->vcTrunk.begin()
                   <<" no "
                   <<pvcBranch->size();
            #endif
        }
    #ifdef DEBUG
    qDebug()<<"CCdrData::initCdrData stop!";
    #endif
    }
    return SUCCESS;
}

QString* VCBranch::addOne()
{
    this->resize(this->size()+1);
    return &(this->last());
}

VCBranch* VCTrunk::addOne()
{
    this->resize(this->size()+1);
    return &(this->last());
}

//parent参数对于table没有任何意义
int CCdrData::rowCount(const QModelIndex &/*parent*/) const
{
    return this->pcCdrDefine->vcFields.size();
}
int CCdrData::columnCount(const QModelIndex &/*parent*/) const
{
    int re = 0,tmp = 0;
    for(VCTrunk::iterator iter = const_cast<VCBranch*>(this->vcTrunk.begin());
        iter != this->vcTrunk.end();
        iter ++)
    {
        tmp = iter->size();
        re = tmp > re ? tmp : re;
    }
    return re;
}

//data函数根据不同的情况返回不同的
QVariant CCdrData::data(const QModelIndex &index, int role) const
{
    QVariant re;
    switch(role)
    {
    case Qt::DisplayRole:
        if(index.row() < this->vcTrunk.size() && index.column() < this->vcTrunk[index.row()].size())
            re = this->vcTrunk[index.row()][index.column()];
        break;
    case Qt::BackgroundRole:
        if(index.row() < this->vcTrunk.size() && index.column() < this->vcTrunk[index.row()].size())
        {
            if(this->pcCdrDefine->vcFields[index.row()].getValue("cpid") == "number")
            {
                QBrush cyan(Qt::cyan);
                re = cyan;
            }
            else if(this->pcCdrDefine->vcFields[index.row()].getValue("cpid") == "member")
            {
                QBrush yellow(Qt::yellow);
                re = yellow;
            }

            //对于自定义的颜色,根据自定义处理
            QString sColor = this->pcCdrDefine->vcFields[index.row()].getValue("color");
            if(sColor.isEmpty() != true)
            {
                qDebug()<<sColor;
                if(sColor == "red")
                {
                    QBrush red(Qt::red);
                    re = red;
                }
                else if(sColor == "green")
                {
                    QBrush green(Qt::green);
                    re = green;
                }
                else if(sColor == "yellow")
                {
                    QBrush yellow(Qt::yellow);
                    re = yellow;
                }
                else if(sColor == "gray")
                {
                    QBrush gray(Qt::gray);
                    re = gray;
                }
                else if(sColor == "cyan")
                {
                    QBrush cyan(Qt::cyan);
                    re = cyan;
                }
                else if(sColor == "blue")
                {
                    QBrush blue(Qt::blue);
                    re = blue;
                }
            }

        }
        break;

    case Qt::SizeHintRole:
        QSize size(20,3);
        re = size;
        break;
    }

    return re;
}
QVariant CCdrData::headerData(int section, Qt::Orientation orientation, int role) const
{
    QVariant re;
    switch (role)
    {
    case Qt::DisplayRole :
        if(orientation == Qt::Horizontal)
        {
            re = section + 1;
        }
        else
        {
            QString s = QString::number(section + 1);
            re = s + '.' + this->pcCdrDefine->vcFields[section].getValue("field");
        }
        break;
//    case Qt::SizeHintRole:
//        QSize size(3,20);
//        re = size;
//        break;
    }

    return re;
}

CCdrDefine* CCdrData::getCurCdrDefine()
{
    return this->pcCdrDefine;
}
