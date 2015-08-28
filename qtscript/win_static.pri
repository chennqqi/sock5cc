#	file: win_static.pri
#	
#	author : CHENQ(chennqqi@qq.com)
#	date: 2015-03-03	
#	description: qmake generate msvc *.vxproj 
#		with static config(/MT)
#
##########################################################

win32 {
	QMAKE_CFLAGS_RELEASE -= -MD
	QMAKE_CFLAGS_RELEASE += -MT
	QMAKE_CXXFLAGS_RELEASE -= -MD
	QMAKE_CXXFLAGS_RELEASE += -MT
	
	QMAKE_CFLAGS_DEBUG -= -MDd
	QMAKE_CFLAGS_DEBUG += -MTd
	QMAKE_CXXFLAGS_DEBUG -= -MDd
	QMAKE_CXXFLAGS_DEBUG += -MTd
}