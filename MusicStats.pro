QT += core gui widgets charts

CONFIG += c++20
CONFIG += lrelease
CONFIG += embed_translations

QMAKE_CXXFLAGS += -Wall -Wextra -pedantic-errors

DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000 # disables all the APIs deprecated before Qt 6.0.0

TARGET = MusicStats
DESTDIR = $$PWD/bin
RC_FILE = $$PWD/res/MusicStats.rc

# Default rules for deployment
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

# Automatic coping Qt libs into bin folder
win32: CONFIG(release, debug|release) {
QMAKE_POST_LINK += $$quote($$[QT_INSTALL_BINS]/windeployqt.exe --no-compiler-runtime $$DESTDIR)
}

INCLUDEPATH += \
	src

SOURCES += \
	src/main.cpp \
	src/MusicStats.cpp \
	src/gui/Gui.cpp \
	src/gui/LibraryTable.cpp \
	src/gui/LibraryTableItem.cpp \
	src/gui/MainWindow.cpp \
	src/gui/StatisticsChart.cpp \
	src/input/Input.cpp \
	src/input/XmlParser.cpp

HEADERS += \
	src/MusicStats.h \
	src/common/Album.h \
	src/common/Artist.h \
	src/common/Global.h \
	src/common/Helper.h \
	src/common/Library.h \
	src/common/Track.h \
	src/gui/Gui.h \
	src/gui/LibraryTable.h \
	src/gui/LibraryTableItem.h \
	src/gui/MainWindow.h \
	src/gui/StatisticsChart.h \
	src/input/Input.h \
	src/input/XmlParser.h

TRANSLATIONS += \
	lng/MusicStats_en_US.ts
