QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17
QT += multimedia
# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    bot.cpp \
    bullet.cpp \
    controller.cpp \
    entity.cpp \
    gamingobject.cpp \
    gun.cpp \
    hero.cpp \
    imageloader.cpp \
    level.cpp \
    levelcreatorcontroller.cpp \
    levelplaycontroller.cpp \
    main.cpp \
    mainwindow.cpp \
    menu.cpp \
    menuitem.cpp \
    pixmap.cpp \
    scene.cpp \
    view.cpp

HEADERS += \
    bot.h \
    bullet.h \
    controller.h \
    entity.h \
    gamingobject.h \
    gun.h \
    hero.h \
    imageloader.h \
    level.h \
    levelcreatorcontroller.h \
    levelplaycontroller.h \
    mainwindow.h \
    menu.h \
    menuitem.h \
    pixmap.h \
    scene.h \
    view.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resources.qrc
