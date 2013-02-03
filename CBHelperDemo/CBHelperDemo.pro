APP_NAME = CBHelperDemo

CONFIG += qt warn_on cascades10

INCLUDEPATH += ../../CBHelper/src ../../CBHelper/src/yajl ../../CBHelper/src/yajl/api

INCLUDEPATH += ${QNX_TARGET}/usr/include/qt4/QtCore ${QNX_TARGET}/usr/include/qt4/QtConnectivity ${QNX_TARGET}/usr/include/bb/device

INCLUDEPATH += ${QNX_TARGET}/usr/include/bb/cascades/pickers ${QNX_TARGET}/usr/include/qt4/QtGui

DEPENDPATH += ../../CBHelper/src ../../CBHelper/src/yajl ../../CBHelper/src/yajl/api

DEPENDPATH += ../src ${QNX_TARGET}/usr/include/qt4/QtCore ${QNX_TARGET}/usr/include/qt4/QtConnectivity ${QNX_TARGET}/usr/include/bb/device

DEPENDPATH += ${QNX_TARGET}/usr/include/bb/cascades/pickers ${QNX_TARGET}/usr/include/qt4/QtGui

QT += gui

LIBS += -L../../CBHelper/Device-Debug
LIBS += -L../../CBHelper/Device-Release
LIBS += -L../../CBHelper/Simulator-Debug

PRE_TARGETDEPS += ../../CBHelper/Simulator-Debug/libCBHelper.so

LIBS += -lCBHelper
LIBS += -lbbdevice
LIBS += -lQtLocationSubset
LIBS +=  -lbbcascadespickers

CONFIG += no_keywords

include(config.pri)