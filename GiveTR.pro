QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11
QT += sql
QT += printsupport

win32:RC_ICONS += givertr.ico

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    main.cpp \
    mainwindow.cpp \
    mydataobject.cpp \
    settingsdialog.cpp \
    simplemessagebox.cpp

HEADERS += \
    mainwindow.h \
    mydataobject.h \
    myinclude.h \
    settingsdialog.h \
    myflag.h \
    data_support.h \
    myinclude.h \
    simplemessagebox.h

FORMS +=

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
