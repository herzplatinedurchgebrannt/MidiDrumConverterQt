QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    Binasc.cpp \
    MidiEvent.cpp \
    MidiEventList.cpp \
    MidiFile.cpp \
    MidiMessage.cpp \
    Options.cpp \
    about.cpp \
    main.cpp \
    mainwindow.cpp \
    tinyxml2.cpp

HEADERS += \
    Binasc.h \
    MidiEvent.h \
    MidiEventList.h \
    MidiFile.h \
    MidiMessage.h \
    Options.h \
    about.h \
    json.hpp \
    mainwindow.h \
    tinyxml2.h

FORMS += \
    about.ui \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    ../Coding/Icons/Icons.qrc

ICON = MidiConverter.icns
