/*! XAfx.h
********************************************************************************
                          #    #  ####  #####  #####  
                           #  #  #    # #    # #    # 
                            ##   #      #    # #    # 
                            ##   #      #####  #####  
                           #  #  #    # #      #      
                          #    #  ####  #      #      
--------------------------------------------------------------------------------
create date			:  2014-09-10
author				:  CHENQ
version				:	---
--------------------------------------------------------------------------------
If you find any bugs, please e-mail me chennqqi@qq.com
--------------------------------------------------------------------------------
NOTE:
 You should include this file before you use any code of XCPP project.
 Generally, this file is included in "stdafx.h"
 
 在使用任何xcpp项目中的代码，你首先要包含这个文件。
 通常来说你只要在你的预处理文件stdafx.h中包含就ok了

*******************************************************************************/
#ifndef __STDAFX_H__
#define __STDAFX_H__



#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#if (defined(WINVER) || defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__))
 #define __LITTLE_ENDIAN 1234
 #define __BIG_ENDIAN    4321
 #define __BYTE_ORDER __LITTLE_ENDIAN

 #ifndef OS_WIN
  #define OS_WIN 1
 #endif

#if _MSC_VER<1400
#error "For some reason, we need MS >= VS2005"
#endif

#elif defined(__linux__) || defined(__linux)
 #ifndef OS_LINUX
  #define OS_LINUX 1
  #define OS_UNIX OS_LINUX
 #endif
#if __GNUC__<4
#error "For some reason, we need gcc version>=gcc4.0"
#endif

#else
#error "NOT support platform, please e-mail me chennqqi@qq.com"
#endif /*(defined(WINVER) || defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__))*/


#ifdef OS_WIN
#include <tchar.h>
#include <WinSock2.h>
#include <Windows.h>
#include <WinBase.h>
#include <Ws2tcpip.h>
#include <crtdbg.h>
#include <direct.h>
#include <stdint.h>
#include <time.h>
#include <io.h>
typedef void* caddr_t;

#define _ASSERT_X _ASSERT_EXPR

//windows llp64
#ifdef _WIN64
typedef __int64 ssize_t;
#define OS_X64
#else
#define OS_X32
typedef int ssize_t;
#endif

#else //!!!OS_WIN
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <assert.h>
#include <endian.h>
#include <errno.h>
#include <limits.h>
#include <unistd.h>
#include <wchar.h>
#include <time.h>
#include <fcntl.h>


#ifdef NDEBUG
#define _ASSERT(x)
#define _ASSERT_X(x,msg) 
#else
#define _ASSERT assert
#define _ASSERT_X(x,msg) 	do 	{ _ASSERT(x); if (!(x)) printf("%s\n", (msg));	}while(false)
#endif

#endif /*Endof OS_WIN*/


#ifdef OS_LINUX
#define _stricmp strcasecmp
#define _taccess access
#define _tremove remove
#define lstrlen strlen


#ifdef UNICODE
#define __T(x)      L ## x
#else
#define __T(x)      x
#endif

#define _TEXT(x)    __T(x)
#define _T(x)       __T(x)

#define _alloca alloca

#include <inttypes.h>
#define INVALID_HANDLE_VALUE ((int)(~0))

#if !defined(_INC_WINDOWS) && !defined(_WINDOWS_)
typedef int            BOOL;     /* f */
typedef unsigned char  BYTE;     /* b */
typedef unsigned int   UINT;     /* ui */
typedef unsigned short USHORT;   /* us */
typedef unsigned long  ULONG;    /* ul */
typedef uint64_t	   UINT64;
typedef unsigned long long ULONG64;
typedef int64_t		   INT64;
typedef uint32_t	  UINT32;
typedef int32_t		INT32;
typedef uint16_t	UINT16;
typedef int16_t		INT16;
typedef unsigned char UINT8;
typedef char		INT8;
typedef int         INT;
typedef int			SOCKET;
#define INVALID_SOCKET -1
#define closesocket close
#endif   // _INC_WINDOWS

#define _stricmp strcasecmp
#define _strnicmp strncasecmp

#ifndef _MAX_PATH
#define _MAX_PATH PATH_MAX
#endif

#ifdef __x86_64__
#define OS_X64
#else
#define OS_X32
#endif

#ifdef UNICODE
typedef wchar_t		TCHAR;
#else
typedef char		TCHAR;
#endif

#ifndef TRUE
#define TRUE    1
#endif

#ifndef FALSE
#define FALSE   0
#endif

#endif /*OS_LINUX*/

#ifndef EOS
#define EOS _T('\0')
#endif


#if defined(OS_MACOSX) || defined(OS_LINUX) || defined(OS_FREEBSD) ||     \
	defined(OS_OPENBSD) || defined(OS_SOLARIS) || defined(OS_ANDROID) ||  \
	defined(OS_NACL)
#define OS_POSIX 1
#endif

#ifndef IN
#define IN
#endif

#ifndef OUT
#define OUT
#endif

#ifndef _ERRNO_T_DEFINED
#define _ERRNO_T_DEFINED
typedef int errno_t;
#endif

#ifndef UNUSED
#define UNUSED(x) (void)(x)
#endif


#ifndef X_UNINIT
#define X_UNINIT(x) (void)(x)
#endif

//#include "XEndian.h"

#ifdef OS_WIN
#define WAIT_FOREVER INFINITE
#else
#define WAIT_FOREVER (UINT)(~0)
#endif

#ifndef _countof
#define _countof(_a) (sizeof(_a)/sizeof( (_a)[0] )  )
#endif

#ifndef DISALLOW_COPY_AND_ASSIGN
#define DISALLOW_COPY_AND_ASSIGN(TypeName) \
	TypeName(const TypeName&);               \
	void operator=(const TypeName&)
#endif


// TODO:  在此处引用程序需要的其他头文件
#endif /*__STDAFX_H__*/
