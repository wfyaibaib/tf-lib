#-------------------------------------------------
#
# Project created by QtCreator 2013-08-03T11:41:55
#
#-------------------------------------------------

QT       -= core gui

TARGET = tf
TEMPLATE = lib

DEFINES += TF_LIBRARY

SOURCES += testlib.cpp

HEADERS += testlib.h\
        tf_global.h \
    ../node.hpp

unix:!symbian {
    maemo5 {
        target.path = /opt/usr/lib
    } else {
        target.path = /usr/lib
    }
    INSTALLS += target
}
