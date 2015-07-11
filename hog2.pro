#-------------------------------------------------
#
# Project created by QtCreator 2015-06-03T17:20:52
#
#-------------------------------------------------

QT       += core

QT       -= gui

TARGET = hog2
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app

INCLUDEPATH += C:\\opencv249-mingw\\install\\include
#LIBS += -LC:/opencv-mingw/install/x64/mingw/bin/*.dll”
LIBS += -LC:\\opencv249-mingw\\install\\x64\\mingw\\bin #\
  #  libopencv_core300 \
  #  libopencv_highgui300 \
  #  libopencv_imgproc300 \
  #  libopencv_features2d300 \
  #  libopencv_photo300 \
  #  libopencv_calib3d300
LIBS += -lopencv_core249 -lopencv_highgui249 -lopencv_imgproc249 -lopencv_features2d249 -lopencv_photo249 -lopencv_ml249 -lopencv_objdetect249


SOURCES += main.cpp
