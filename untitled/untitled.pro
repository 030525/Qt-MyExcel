
CONFIG+= PRECOMPILED_HEADER
PRECOMPILED_HEADER =

QT       += core gui
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    finddialog.cpp \
    gotowidget.cpp \
    main.cpp \
    mainwindow.cpp \
    tableset.cpp

HEADERS += \
    finddialog.h \
    gotowidget.h \
    mainwindow.h \
    tableset.h

FORMS += \
    finddialog.ui \
    gotowidget.ui \
    mainwindow.ui \
    tableset.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target


RESOURCES += \
    img.qrc

DISTFILES +=
