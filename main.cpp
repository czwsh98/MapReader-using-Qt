/*
* main.cpp
* Calls and initializes the mainwindow
*
* Written by: Ziwei Chen
*         on: 8-10-2017
*         Jiaotong University All Rights Reserved
*/

#include <QApplication>
#include "mainwindow.h"
#include "curve_lines.h"
#include "efficiency_lines.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow main;
    main.show();
    return a.exec();
}
