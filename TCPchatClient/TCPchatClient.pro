QT       += core gui network multimedia

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    clientswindow.cpp \
    connectionwindow.cpp \
    emojiwindow.cpp \
    imageballoon.cpp \
    main.cpp \
    mainwindow.cpp \
    messageballoon.cpp \
    volumedialog.cpp

HEADERS += \
    clientswindow.h \
    connectionwindow.h \
    emojiwindow.h \
    imageballoon.h \
    mainwindow.h \
    messageballoon.h \
    volumedialog.h

FORMS += \
    clientswindow.ui \
    connectionwindow.ui \
    emojiwindow.ui \
    mainwindow.ui \
    volumedialog.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    sound.qrc
