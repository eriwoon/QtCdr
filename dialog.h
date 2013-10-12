#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include <QPushButton>
#include <QString>
#include <QLabel>
#include <QTableView>
#include <QStandardItemModel>
#include <QTableWidget>
#include "ccdrxmlprocess.h"
#include "ccdrbaseinfo.h"
#include "ccdrdefine.h"
#include "ccdrdata.h"
#include <QComboBox>
#include <QTextEdit>

namespace Ui {
class Dialog;
}

class Dialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit Dialog(QWidget *parent = 0);
    ~Dialog();
    
private:
    Ui::Dialog *ui;

//界面按钮和控件
private:
    //QPushButton *qButton;
    QPushButton *qButtonOpenFromFTP;
    QPushButton *qButtonOpenFile;
    QString      sFileName;
    QLabel      *qLabel;
    QLabel      *qLabelDesc;
    QTableView  *qTable;
    QTabWidget  *qTab;
    QComboBox   *qComboBox;
    QTextEdit   *qTextEdit;
    QList<QString> lsComboValues;

//消息响应处理函数
private slots:
    void onClickOpenFile();
    //void onFileOpen();
    void onClickCloseTab(int);
    void onClickTableView(QString);
    void onComboBoxChange(int);
    void onTextEditChange();

private:
    void setDialogLayout();
    void setTabWidget(QString,CCdrDefine*,QString);
    void openWithFile(QString);
    void dropEvent(QDropEvent *);
    void dragEnterEvent(QDragEnterEvent *);

//话单配置文件处理
    VCCdrDefines vcCdrDefines;
};

#endif // DIALOG_H
