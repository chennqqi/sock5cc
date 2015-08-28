################################################################################
#	file: noneQt.pri
#	
#	author : CHENQ(chennqqi@qq.com)
#	date: 2015-03-03	
#	description: configure project(MSVC) or generate Makefile without qt 
#		attribute
#
################################################################################
QT -= gui core
CONFIG -= QT

win32 {
	QMAKE_LIBS_QT_ENTRY -= -lqtmain
} else {
	LIBS += -Wl,-rpath=./
}