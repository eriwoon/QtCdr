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

Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);   

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
    qComboBox = new QComboBox;
    qComboBox->setSizeAdjustPolicy(QComboBox::AdjustToContents);
    //增加默认选项
    lsComboValues.push_back("-");
    connect(qComboBox,SIGNAL(currentIndexChanged(int)),this,SLOT(onComboBoxChange(int)));
    //TextEdit控件
    qTextEdit = new QTextEdit;
    qTextEdit->setLineWrapMode(QTextEdit::WidgetWidth);
    qTextEdit->setMaximumHeight(55);
    connect(qTextEdit,SIGNAL(textChanged()),this,SLOT(onTextEditChange()));

    //设置开启drag and drop功能
    this->setAcceptDrops(true);

    //设置图形展示
    setDialogLayout();

    //加载xml文件中的话单配置
    if(CCdrXmlProcess::init(&vcCdrDefines) == FAIL)
    {
        #ifdef DEBUG
        qDebug()<<"ExecuteMessageBox";
        #endif
        QMessageBox qMsgBox;
        qMsgBox.setText("Cannot find cdrconfig.xml file!!");
        qMsgBox.exec();
        //this->close();
    }
    //加载成功后,增加下拉框设置
    else
    {
        for(VCCdrDefines::iterator iter = vcCdrDefines.begin();
            iter != vcCdrDefines.end();
            iter ++)
        {
            QString s(iter->vcCdrAttr.getValue("name"));
            #ifdef DEBUG
            qDebug()<<"qComboBox->addItem value="<<s;
            #endif

            if(s.size()>0)
            {
                lsComboValues.push_back(iter->vcCdrAttr.getValue("name"));
            }
        }
        qComboBox->addItems(lsComboValues);

    }

}

Dialog::~Dialog()
{
    delete ui;
    //delete qButton;
    delete qLabel;
}


void Dialog::setDialogLayout()
{
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
    QLabel *qLable_SelectCDRType = new QLabel("Select CDR Type:");
    H2->addWidget(qLable_SelectCDRType);
    H2->addWidget(qComboBox);
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
    qTab->removeTab(index);
}

void Dialog::onClickOpenFile()
{
    #ifdef DEBUG
    qDebug()<<"Dialog::onClickOpenFile()";
    #endif

    sFileName = QFileDialog::getOpenFileName(this,"Open File",sFileName,"Open Files (*.*)");

    openWithFile(sFileName);
}

void Dialog::openWithFile(QString sFileName)
{
    #ifdef DEBUG
    qDebug()<<"Dialog::openWithFile("<<sFileName<<")";
    #endif
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

        //设置窗口标题
        this->setWindowTitle(sFileName + " -- CDR Reader");
        //设置FileName的Label
        QString sPrompt("FileName:");
        qLabel->setText(sPrompt+sFileName);
        qLabel->setEnabled(true);
        qLabel->setWordWrap(true);
        qLabel->setTextInteractionFlags(Qt::TextSelectableByMouse);

        //清除已有的相关tab信息
        while(qTab->count()!=0)
        {
            qDebug()<<"Number:"<<qTab->count();
            qTab->removeTab(0);
        }

        //相关信息梳理,为循环准备
        QByteArray qByteArray;
        QString qStr;
        int iRecordNo = 0;
        //create MODEL
        QFileInfo qFileinfo(sFileName);
        CCdrDefine* pcCdrDefine = this->vcCdrDefines.findMatchedCDR(qFileinfo.baseName());

        if(pcCdrDefine == 0)
            return;

        //此时,还要与右上角的控件进行互动
        //清空当前的textEdit控件的文本
        qTextEdit->clear();
        //设置当前的comboBox显示正确的选项.
        int iIndexOfComboBox = qComboBox->findText(pcCdrDefine->vcCdrAttr.getValue("name"));
        if(iIndexOfComboBox == -1) iIndexOfComboBox = 0;
        qComboBox->setCurrentIndex(iIndexOfComboBox);

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
            setTabWidget(qStr,pcCdrDefine,sTabTitle);
        }

        qFile.close();

    }
}

void Dialog::setTabWidget(QString sLine,CCdrDefine* pcCdrDefine, QString sTabTitle)
{
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

    int iIndex = qComboBox->currentIndex() - 1;
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
    while(qTab->count()!=0)
    {
        qDebug()<<"Number:"<<qTab->count();
        qTab->removeTab(0);
    }

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


