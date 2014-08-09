#-------------------------------------------------
#
# Project created by QtCreator 2014-08-06T18:10:49
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = VT-Playback
TEMPLATE = app


SOURCES += main.cpp\
        vtcontrolwindow.cpp \
    mediaitemwidget.cpp

HEADERS  += vtcontrolwindow.h \
    mediaitemwidget.h

FORMS    += vtcontrolwindow.ui \
    mediaitemwidget.ui

INCLUDEPATH +=  /usr/local/include/mlt
LIBS += -L/usr/local/lib -lmlt++ -lmlt
