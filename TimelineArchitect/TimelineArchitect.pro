QT+= core gui

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
    centralframe_View.cpp \
    event.cpp \
    eventframe_View.cpp \
    main.cpp \
    mainwindow.cpp \
    newevent_View.cpp \
    newtimeline_View.cpp \
    showevent_View.cpp \
    steptype.cpp \
    streamReader.cpp \
    tags.cpp \
    textformatcontrol.cpp \
    timeline.cpp \
    timemaster.cpp

HEADERS += \
    centralframe_View.h \
    event.h \
    eventframe_View.h \
    mainwindow.h \
    newevent_View.h \
    newtimeline_View.h \
    showevent_View.h \
    steptype.h \
    streamReader.h \
    tags.h \
    textformatcontrol.h \
    timeline.h \
    timemaster.h

FORMS += \
    centralframe_View.ui \
    eventframe_View.ui \
    mainwindow.ui \
    newevent_View.ui \
    newtimeline_View.ui \
    showevent_View.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
