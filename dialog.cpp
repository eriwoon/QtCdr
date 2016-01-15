#include "dialog.h"
#include "ui_dialog.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFileDialog>
#include <QFileInfo>
#include <QDebug>
#include <QMessageBox>
#include "ccdrbaseinfo.h"
#include "ccdrdata.h"
#include "ccdrdefine.h"
#include "ccdrxmlprocess.h"
#include <QDropEvent>
#include <QMimeData>
#include <QDragEnterEvent>
#include <QUrl>
#include "ccdrtableviewext.h"
#include <QGridLayout>
#include <Qdir>

#define XML_LOCATION "c:/cdrconfig.xml"

Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);   

    LOG_FUNC(Dialog::Dialog);
    this->setWindowTitle("CDR Reader");
    this->setWindowFlags(Qt::Dialog
                         | Qt::WindowMaximizeButtonHint
                         | Qt::WindowMinimizeButtonHint
                         | Qt::WindowCloseButtonHint);

    qButtonOpenFromFTP = new QPushButton("Open FTP");
    qButtonOpenFromFTP->hide();

    qButtonOpenFile = new QPushButton("&Open File");
    connect(qButtonOpenFile,SIGNAL(clicked()),this,SLOT(onClickOpenFile()));

    //new label to show the file path
    qLabel = new QLabel;
    qLabel->setText("No File Selected.");
    qLabel->setDisabled(true);
    qLabel->setWordWrap(true);

    qLabelDesc = new QLabel;
    qLabelDesc->setWordWrap(true);
    qLabelDesc->setTextInteractionFlags(Qt::TextSelectableByMouse);
    qLabelDesc->setText(COPYRIGHTINFO);

    //new tab view
    qTab = new QTabWidget();
    //enable close subtab function
    qTab->setTabsClosable(true);
    qTab->setMovable(true);
    connect(qTab,SIGNAL(tabCloseRequested(int)),this,SLOT(onClickCloseTab(int)));
    //使能够根据用户选中的单元格不同,得到不同的用户的信息

    //右上角的控件处理
    //选取site
    qComboBox_selectSite    = new QComboBox;
    qComboBox_selectSite->setSizeAdjustPolicy(QComboBox::AdjustToContents);
    lsComboValuesSite.push_back("-");
    this->initCdrConf();
    qComboBox_selectSite->addItems(this->lsComboValuesSite);
    connect(qComboBox_selectSite,SIGNAL(currentIndexChanged(int)),this,SLOT(onComboBoxChangeSite(int)));

    //选取CDRType
    qComboBox_selectCDRType = new QComboBox;
    qComboBox_selectCDRType->setSizeAdjustPolicy(QComboBox::AdjustToContents);
    lsComboValues.push_back("<-Please Choose Site first");
    qComboBox_selectCDRType->addItems(lsComboValues);
    connect(qComboBox_selectCDRType,SIGNAL(currentIndexChanged(int)),this,SLOT(onComboBoxChange(int)));

    //TextEdit控件
    qTextEdit = new QTextEdit;
    qTextEdit->setLineWrapMode(QTextEdit::WidgetWidth);
    qTextEdit->setMaximumHeight(55);
    connect(qTextEdit,SIGNAL(textChanged()),this,SLOT(onTextEditChange()));

    //设置开启drag and drop功能
    this->setAcceptDrops(true);

    //设置图形展示
    this->setDialogLayout();
}

Dialog::~Dialog()
{
    delete ui;
    //delete qButton;
    delete qLabel;
}


void Dialog::setDialogLayout()
{
    LOG_FUNC(Dialog::setDialogLayout);
    //左上-上
    QHBoxLayout *H1 = new QHBoxLayout;
    H1->addWidget(qButtonOpenFile);
    H1->addWidget(qButtonOpenFromFTP);
    H1->addStretch();

    //左上
    QVBoxLayout *V1 = new QVBoxLayout;
    V1->addLayout(H1);
    V1->addWidget(qLabel);

    //右上-上
    QHBoxLayout *H2 = new QHBoxLayout;
    QLabel *qLable_selectSite    = new QLabel("select Site:");
    H2->addWidget(qLable_selectSite);
    H2->addWidget(this->qComboBox_selectSite);
    QLabel *qLable_SelectCDRType = new QLabel("Select CDR Type:");
    H2->addWidget(qLable_SelectCDRType);
    H2->addWidget(qComboBox_selectCDRType);

    H2->addStretch();

    //右上
    QVBoxLayout *V2 = new QVBoxLayout;
    V2->addLayout(H2);
    V2->addWidget(qTextEdit);

    //上
    QGridLayout *H3 = new QGridLayout;
    H3->addLayout(V1,1,1);
    H3->addLayout(V2,1,2);
    H3->setColumnStretch(1,1);
    H3->setColumnStretch(2,2);


    //下
    QGridLayout *V3 = new QGridLayout;
    V3->addWidget(qTab,1,1);
    V3->addWidget(qLabelDesc,1,2);
    V3->setColumnStretch(1,2);
    V3->setColumnStretch(2,1);
    //V3->set
    //整体
    QVBoxLayout *qVLayout = new QVBoxLayout;
    qVLayout->addLayout(H3);
    qVLayout->addLayout(V3);
    //qVLayout->addStretch();
    setLayout(qVLayout);
}
/*
//set the Tab view
void Dialog::setTabWidget()
{
    QFile qFile;
    qFile.setFileName(sFileName);
    if(!qFile.open(QIODevice::ReadOnly))
    {
        qDebug()<<"Open file failed:"<<sFileName;
    }

    QByteArray qByteArray;
    QString qStr;
    int iRecordNo = 0;

    for(qByteArray = qFile.readLine();qByteArray.count()>1;qByteArray = qFile.readLine())
    {
        qStr = qByteArray;

        //create MODEL
        QStandardItemModel *qModel = new QStandardItemModel;
        //create VIEW
        QTableView* qTable = new QTableView;

        CModuleProcess::init(qModel,sFileName,qStr);
        //qTable->
        qTable->setModel(qModel);
        qTable->setAlternatingRowColors(true);

        QWidget* qW1 = new QWidget;
        QHBoxLayout *qL1 = new QHBoxLayout;
        qL1->addWidget(qTable);
        qW1->setLayout(qL1);

        QString tabname("Record ");
        tabname.append(iRecordNo + '1');
        qTab->addTab(qW1,tabname);
        //qTab->addTab(qW1,tabname);
        iRecordNo++;

        //Max record is 9.
        if(iRecordNo >= 9)
            break;
    }
    qFile.close();
}
*/
void Dialog::onClickCloseTab(int index)
{
    LOG_FUNC(Dialog::onClickCloseTab);
    qTab->removeTab(index);
}

void Dialog::onClickOpenFile()
{
    LOG_FUNC(Dialog::onClickOpenFile);

    sFileName = QFileDialog::getOpenFileName(this,"Open File",sFileName,"Open Files (*.*)");

    this->openWithFile(sFileName);
}

void Dialog::openWithFile(QString sFileName)
{
    const bool debug = true;
    LOG_FUNC(Dialog::openWithFile);
    //当传入的文件名不为空的时候调用下面的组件
    if(!sFileName.isEmpty())
    {
        QFile qFile;
        qFile.setFileName(sFileName);
        if(!qFile.open(QIODevice::ReadOnly))
        {
            QMessageBox qMsgBox;
            qMsgBox.setText("Cannot find this unl file!!");
            qMsgBox.exec();
            qDebug()<<"Open file failed:"<<sFileName;
            return;
        }
        if(debug)qDebug()<<"set window title.";
        //设置窗口标题
        this->setWindowTitle(sFileName + " -- CDR Reader");
        //设置FileName的Label
        QString sPrompt("FileName:");
        qLabel->setText(sPrompt+sFileName);
        qLabel->setEnabled(true);
        qLabel->setWordWrap(true);
        qLabel->setTextInteractionFlags(Qt::TextSelectableByMouse);

        if(debug)qDebug()<<"clear related tab info.";
        //清除已有的相关tab信息
        this->qTab->clear();

        if(debug)qDebug()<<"preparing for loop.";
        //相关信息梳理,为循环准备
        QByteArray qByteArray;
        QString qStr;
        int iRecordNo = 0;
        //create MODEL
        QFileInfo qFileinfo(sFileName);
        CCdrDefine* pcCdrDefine = this->vcCdrDefines.findMatchedCDR(qFileinfo.baseName());

        if(pcCdrDefine == 0)
            return;

        if(debug)qDebug()<<"interact with widget in top-right cornor.";
        //此时,还要与右上角的控件进行互动
        //清空当前的textEdit控件的文本
        qTextEdit->clear();
        //设置当前的comboBox显示正确的选项.
        int iIndexOfComboBox = qComboBox_selectCDRType->findText(pcCdrDefine->vcCdrAttr.getValue("name"));
        if(iIndexOfComboBox == -1) iIndexOfComboBox = 0;
        qComboBox_selectCDRType->setCurrentIndex(iIndexOfComboBox);

        for(qByteArray = qFile.readLine();qByteArray.count()>1;qByteArray = qFile.readLine())
        {
            qStr = qByteArray;
            //设置当前标签页的名字
            QString sTabTitle = pcCdrDefine->vcCdrAttr.getValue("name");
            if(sTabTitle.size() == 0)
            {
                sTabTitle = "Record";
            }
            sTabTitle += " ";
            sTabTitle.append(iRecordNo + '1');
            iRecordNo++;

            //设置该标签页
            this->setTabWidget(qStr,pcCdrDefine,sTabTitle);
        }

        qFile.close();

    }
}

void Dialog::setTabWidget(QString sLine,CCdrDefine* pcCdrDefine, QString sTabTitle)
{
    LOG_FUNC(Dialog::setTabWidget);
    #ifdef DEBUG
    qDebug()<<"Dialog::setTabWidget start";
    qDebug()<<"\tline info:"<<sLine;
    qDebug()<<"\tsTabTitle:"<<sTabTitle;
    #endif

    CCdrData* pcCdrData = 0;
    if(pcCdrDefine != 0)
    {
        pcCdrData = new CCdrData(pcCdrDefine,sLine);
    }
    else
    {
        QMessageBox qMsgBox;
        qMsgBox.setText("Cannot find related unl define!!");
        qMsgBox.exec();
        return;
    }

    //QStandardItemModel *qModel = new QStandardItemModel;
    //create VIEW
    CCDRTableViewExt* qTable = new CCDRTableViewExt;
    //CModuleProcess::init(qModel,sFileName,qStr);
    //qTable->
    qTable->setModel(pcCdrData);
    qTable->setAlternatingRowColors(true);
    connect(qTable,SIGNAL(clicked(QString)),this,SLOT(onClickTableView(QString)));

    QWidget* qW1 = new QWidget;
    QHBoxLayout *qL1 = new QHBoxLayout;
    qL1->addWidget(qTable);
    qW1->setLayout(qL1);


    qTab->addTab(qW1,sTabTitle);
}

void Dialog::dropEvent(QDropEvent* event)
{
    QList<QUrl> lqUrl;
    if(event->mimeData()->hasUrls())
        lqUrl = event->mimeData()->urls();

    if(lqUrl.size() > 0)
    {
        this->setWindowTitle(lqUrl.first().toLocalFile());
        openWithFile(lqUrl.first().toLocalFile());
    }
}

void Dialog::dragEnterEvent(QDragEnterEvent *event)
{
    if(event->mimeData()->hasUrls())
        event->acceptProposedAction();
}

void Dialog::onClickTableView(QString sDesc)
{
    for(QString::iterator iter = sDesc.begin(); iter != sDesc.end(); iter++)
    {
        if(*iter == '$')
            *iter = '\n';
    }

    if(sDesc.size() == 0)
        this->qLabelDesc->hide();
    else
    {
        this->qLabelDesc->show();
        this->qLabelDesc->setText(sDesc);
    }
}

void Dialog::onComboBoxChange(int)
{
    qTextEdit->clear();
}

void Dialog::onTextEditChange()
{
    QString sText = qTextEdit->toPlainText();
    if(sText.isEmpty())
        return;
    sText += '\n';

    int iIndex = qComboBox_selectCDRType->currentIndex() - 1;
    CCdrDefine* pcCdrDefine = 0;

    if(iIndex >=0 && iIndex < this->vcCdrDefines.size())
        pcCdrDefine = &(this->vcCdrDefines[iIndex]);
    else
        return;

    //设置窗口标题
    this->setWindowTitle("CDR Reader");
    //设置FileName的Label
    qLabel->setText("FileName: User Input");
    qLabel->setEnabled(false);

    //清除已有的相关tab信息
    this->qTab->clear();

    QString qStr;
    int nextPos = 0, currentPos = 0;
    int iRecordNo = 0;
    while((nextPos = sText.indexOf("\n",currentPos) )>0)
    {
        qStr = sText.mid(currentPos,nextPos - currentPos);
        currentPos = nextPos + 1;

        if(qStr.size()<10)
            continue;

        //设置当前标签页的名字
        QString sTabTitle = pcCdrDefine->vcCdrAttr.getValue("name");
        if(sTabTitle.size() == 0)
        {
            sTabTitle = "Record";
        }
        sTabTitle += " ";
        sTabTitle.append(iRecordNo + '1');
        iRecordNo++;

        //设置该标签页
        setTabWidget(qStr,pcCdrDefine,sTabTitle);
    }
}

void Dialog::initCdrConf()
{
    LOG_FUNC(Dialog::initCdrConf);
    QDir dir("./");

    /*C++11 语法
    for(auto fileName : dir.entryList())
    {
        if(fileName.indexOf(".xml") != -1)
            this->lsComboValuesSite.append(fileName);
    }*/
    QStringList list = dir.entryList();
    for(QStringList::iterator iter = list.begin(); iter != list.end(); iter ++)
    {
        if(iter->indexOf(".xml") != -1)
            this->lsComboValuesSite.append(*iter);
    }
}

void Dialog::onComboBoxChangeSite(int)
{
    LOG_FUNC(Dialog::onComboBoxChangeSite);
    qTextEdit->clear();
    //加载xml文件中的话单配置
    //if(CCdrXmlProcess::init(&vcCdrDefines) == FAIL)
    if(this->initCdr(&vcCdrDefines, this->lsComboValuesSite[this->qComboBox_selectSite->currentIndex()]) == FAIL)
    {
        QMessageBox qMsgBox;
        qMsgBox.setText("Cannot find configure file!");
        qMsgBox.exec();
        //this->close();
    }
    //加载成功后,增加下拉框设置
    else
    {
        qComboBox_selectCDRType->clear();
        lsComboValues.clear();
        lsComboValues.push_back("-");
        for(VCCdrDefines::iterator iter = vcCdrDefines.begin();
            iter != vcCdrDefines.end();
            iter ++)
        {
            QString s(iter->vcCdrAttr.getValue("name"));
            #ifdef DEBUG
            qDebug()<<"qComboBox_selectCDRType->addItem value="<<s;
            #endif

            if(s.size()>0)
            {
                lsComboValues.push_back(iter->vcCdrAttr.getValue("name"));
            }
        }
        qComboBox_selectCDRType->addItems(lsComboValues);

    }
}

int Dialog::initCdr(VCCdrDefines *pvcCdrDefines, QString fileName)
{
    LOG_FUNC(Dialog::initCdr);
    //初始化返回参数
    pvcCdrDefines->clear();

    CCdrBaseInfo BaseInfo;

    //打开xml配置文件
    QFile file;
    //QMessageBox mb;
    //mb.setText(QDir::currentPath() + "/cdrcomfig.xml");
    //mb.exec();
    file.setFileName(fileName);
    if(!file.open(QIODevice::ReadOnly))
    {
        file.setFileName(XML_LOCATION);
        if(!file.open(QIODevice::ReadOnly))
        {
            return FAIL;
        }
    }

    //创建一个新的节点用来处理
    QXmlStreamReader xmlReader(&file);

    CCdrDefine* cCdeDefine = 0;
    VCAttributes vcAttributes;

    for(QXmlStreamReader::TokenType type = xmlReader.readNext();!xmlReader.atEnd(); type = xmlReader.readNext())
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
