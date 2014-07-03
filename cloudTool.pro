QT       += core
QT       -= gui

QMAKE_CXXFLAGS += -std=gnu++11

TARGET = cloudTool
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app

SOURCES += main.cpp

INCLUDEPATH += ../../SAT/posture-vision/src /usr/include/pcl-1.7 /usr/include/eigen3

LIBS += -lboost_filesystem -lboost_system
LIBS += -L/usr/lib -lpcl_io -lpcl_common

OBJECTS_DIR = build
