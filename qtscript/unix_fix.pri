##########################################################
#	file: unix_fix.pri
#	
#	author : CHENQ(chennqqi@qq.com)
#	date: 2015-03-03	
#	description: linux -lrt(time) -lpthread(thread) in unix
#
##########################################################

unix {
	LIBS += -lrt -lpthread
}