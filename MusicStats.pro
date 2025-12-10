QT += core gui widgets

CONFIG += c++17
CONFIG += lrelease
CONFIG += embed_translations

DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

TARGET = MusicStats
DESTDIR = $$PWD/bin

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

INCLUDEPATH += \
	src

SOURCES += \
	src/main.cpp \
	src/MusicStats.cpp \
	src/gui/Gui.cpp \
	src/gui/MainWindow.cpp \
	src/input/Input.cpp \
	src/logic/Logic.cpp

HEADERS += \
	src/MusicStats.h \
	src/gui/Gui.h \
	src/gui/MainWindow.h \
	src/input/Input.h \
	src/logic/Logic.h

TRANSLATIONS += \
	lng/MusicStats_en_US.ts
