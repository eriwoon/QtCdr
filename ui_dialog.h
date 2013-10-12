/********************************************************************************
** Form generated from reading UI file 'dialog.ui'
**
** Created by: Qt User Interface Compiler version 5.0.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DIALOG_H
#define UI_DIALOG_H

#include <QtCore/QVariant>

#define V4_8

#ifndef V4_8
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHeaderView>
#endif
#ifdef V4_8
#include <QAction>
#include <QApplication>
#include <QButtonGroup>
#include <QDialog>
#include <QHeaderView>
#endif

QT_BEGIN_NAMESPACE

class Ui_Dialog
{
public:

    void setupUi(QDialog *Dialog)
    {
        if (Dialog->objectName().isEmpty())
        {
            #ifndef V4_8
            Dialog->setObjectName(QStringLiteral("Dialog"));
            #endif
            #ifdef V4_8
            Dialog->setObjectName("Dialog");
            #endif
        }
        Dialog->resize(400, 300);

        retranslateUi(Dialog);

        QMetaObject::connectSlotsByName(Dialog);
    } // setupUi

    void retranslateUi(QDialog *Dialog)
    {
        Dialog->setWindowTitle(QApplication::translate("Dialog", "Dialog", 0));
    } // retranslateUi

};

namespace Ui {
    class Dialog: public Ui_Dialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DIALOG_H
