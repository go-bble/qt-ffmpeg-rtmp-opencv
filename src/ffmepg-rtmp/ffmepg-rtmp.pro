QT       += core gui  concurrent multimedia #openglextensions opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
#    XVideoWidget.cpp \
    TAudioRecord.cpp \
    TBilateralFilter.cpp \
    TController.cpp \
    TData.cpp \
    TDataThread.cpp \
#    TEncode.cpp \
    TFilter.cpp \
    TMediaEncode.cpp \
    TRtmp.cpp \
    TVideoCapture.cpp \
    main.cpp \
    mainwindow.cpp

HEADERS += \
#    XVideoWidget.h \
    TAudioRecord.h \
    TBilateralFilter.h \
    TController.h \
    TData.h \
    TDataThread.h \
#    TEncode.h \
    TFilter.h \
    TMediaEncode.h \
    TRtmp.h \
    TVideoCapture.h \
    mainwindow.h

FORMS += \
    mainwindow.ui
CONFIG(debug, debug|release){
    DESTDIR = $$PWD/../../bin
}else{
    DESTDIR = $$PWD/../../bin
}

# Default rules for deployment.
unix:!macx|win32: LIBS += -L$$PWD/../../lib/ -lavcodec
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target



INCLUDEPATH += $$PWD/../../include
DEPENDPATH += $$PWD/../../include

unix|win32: LIBS += -L$$PWD/../../lib/ -lavformat -lavutil -lswscale -lswresample
#win32:!win32-g++: PRE_TARGETDEPS += $$PWD/../../lib/avformat.lib


# opencv
#INCLUDEPATH += $$PWD/../../include/opencv2/
LIBS += $$PWD/../../lib/libopencv_*.a







