#-------------------------------------------------
#
# Project created by QtCreator 2017-06-18T11:33:14
#
#-------------------------------------------------

QT       += core gui
QT += sql
QT += widgets


greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Notepad
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
        main.cpp \
    mainwindow.cpp \
    read_xy.cpp \
    custom_graphics.cpp \
    curve_lines.cpp \
    draggable_ellipse.cpp \
    efficiency_lines.cpp \
    info_point.cpp \
    mergesort.cpp

HEADERS += \
    mainwindow.h \
    read_xy.h \
    custom_graphics.h \
    curve_lines.h \
    draggable_ellipse.h \
    efficiency_lines.h \
    info_point.h \
    mergesort.h

FORMS += \
    mainwindow.ui \
    read_xy.ui \
    curve_lines.ui \
    efficiency_lines.ui

RESOURCES += \
    test.qrc
