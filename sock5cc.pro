######################################################################
# Automatically generated by qmake (2.01a) ?? 8? 21 07:08:05 2015
######################################################################A
include(../qtscript/noneQt.pri)
include(../qtscript/fullproject.pri)


TEMPLATE = app
TARGET = 
DEPENDPATH += .
INCLUDEPATH += .

# Input
HEADERS += sock5.h sockprotocol.h stdafx.h
SOURCES += main.cpp sockprotocol.cpp

APPNAME = sock5cc

CONFIG(debug,debug|release) {
	DESTDIR = bin/Debug
	OBJECTS_DIR = temp/$$APPNAME/Debug
} else {
	DESTDIR = bin/Release
	OBJECTS_DIR = temp/$$APPNAME/Release
}
