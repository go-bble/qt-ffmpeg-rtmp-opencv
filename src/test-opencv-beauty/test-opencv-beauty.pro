QT       += core gui concurrent multimedia

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    TFilter.cpp \
    TData.cpp \
    TDataThread.cpp \
    TVideoCapture.cpp \
    main.cpp \
    mainwindow.cpp

HEADERS += \
    TData.h \
    TFilter.h \
    TDataThread.h \
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

