﻿#include "dialog.h"
#include <QApplication>
#include <QDebug>
int main(int argc, char *argv[])
{
    LOG_FUNC(QApplication);
    QApplication a(argc, argv);

    Dialog w;
    w.resize(800,600);
    w.show();
    
    return a.exec();
}
