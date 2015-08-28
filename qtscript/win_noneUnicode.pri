#	author : CHENQ(chennqqi@qq.com)
#	date: 2015-03-03	
#
#	qmake generate msvc *.vxproj without UNICODE mode
#
#
##########################################################

# it looks like qmake(qt) don't use unicode in unix
DEFINES -= UNICODE _UNICODE

win32 {
		CharacterSet = 0
		#CharacterSet = 1 #unicode
		#CharacterSet = 2 #MBCS
}