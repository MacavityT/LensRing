#-------------------------------------------------
#
# Project created by QtCreator 2018-03-09T10:07:48
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Lens_Ring
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
    cmd.cpp \
    ic_capture.cpp \
    pmd.cpp \
    running.cpp \
    sensorthread.cpp

HEADERS += \
        mainwindow.h \
    cmd.h \
    ic_capture.h \
    Dmc1000.h \
    pmd.h \
    running.h \
    sensorthread.h

FORMS += \
        mainwindow.ui \
    cmd.ui \
    pmd.ui


#头文件路径包含

INCLUDEPATH += G:/HALCON/include

INCLUDEPATH += G:/HALCON/include/halconcpp

#lib文件路径包含

LIBS += G:/HALCON/lib/x64-win64/halcon.lib

LIBS += G:/HALCON/lib/x64-win64/halconcpp.lib


win32: LIBS += -L$$PWD/./ -lDmc1000

INCLUDEPATH += $$PWD/.
DEPENDPATH += $$PWD/.

win32:!win32-g++: PRE_TARGETDEPS += $$PWD/./Dmc1000.lib
else:win32-g++: PRE_TARGETDEPS += $$PWD/./libDmc1000.a

QT += serialport \
      sql
