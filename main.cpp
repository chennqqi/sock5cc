#include "stdafx.h"
#include <sys/time.h>
#include <signal.h>
#include <unistd.h>
#include <getopt.h>

#include <ostream>
#include <iostream>
#include <string>
#include <fstream>

#if 0
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <socket.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <errno.h>
#include <fcntl.h>
#ifdef WIN32
#include <winsock2.h>
#else
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#endif

#ifndef INADDR_NONE
#define INADDR_NONE 0xffffffff
#endif
#define MAX_STRING_LEN 1024
#define BUFSIZE 2048

#ifndef WIN32
#define SOCKET int
#else
#define errno WSAGetLastError()
#define close(a) closesocket(a)
#define write(a, b, c) send(a, b, c, 0)
#define read(a, b, c) recv(a, b, c, 0)
#endif

char buf[BUFSIZE];

static char i_host[MAX_STRING_LEN]; /* site name */
static char i_port[MAX_STRING_LEN]; /* port number */

void err_doit(int errnoflag, const char *fmt, va_list ap);
void err_quit(const char *fmt, ...);
int tcp_connect(const char *host, const unsigned short port);
void print_usage();

//xnet_select x defines
#define READ_STATUS 0
#define WRITE_STATUS 1
#define EXCPT_STATUS 2

/*
s - SOCKET
sec - timeout seconds
usec - timeout microseconds
x - select status
*/
SOCKET xnet_select(SOCKET s, int sec, int usec, short x)
{
    int st = errno;
    struct timeval to;
    fd_set fs;
    to.tv_sec = sec;
    to.tv_usec = usec;
    FD_ZERO(&fs);
    FD_SET(s, &fs);
    switch(x)
    {
    case READ_STATUS:
        st = select(s+1, &fs, 0, 0, &to);
        break;
    case WRITE_STATUS:
        st = select(s+1, 0, &fs, 0, &to);
        break;
    case EXCPT_STATUS:
        st = select(s+1, 0, 0, &fs, &to);
        break;
    }
    return(st);
}

int tcp_connect(UINT nIPv4, UINT16 u16DstPort, ULONG nTimoutMs)
{
    unsigned long non_blocking = 1;
    unsigned long blocking = 0;
    int ret = 0;
    struct sockaddr_in sin = {0}; /* an Internet endpoint address */
    int error;

    sin.sin_family = AF_INET;
    sin.sin_port = htons(u16DstPort);
    sin.sin_addr.s_addr = htonl(nIPv4);

    /* Allocate a socket */
    SOCKET s = socket(PF_INET, SOCK_STREAM, ppe->p_proto);
    if (s < 0)
        err_quit("can't create socket: %s/n", strerror(errno));

    /* Connect the socket with timeout */
#ifdef WIN32
    ioctlsocket(s, FIONBIO, &non_blocking);
#else
    ioctl(s, FIONBIO, &non_blocking);
#endif

    if (connect(s, (struct sockaddr *)&sin, sizeof(sin)) == -1)
    {
        struct timeval tv = {0};
        // 设置连接超时时间
        tv.tv_sec = nTimoutMs/1000; // 秒数
        tv.tv_usec = nTimoutMs%1000;

        fd_set writefds;
        FD_ZERO(&writefds);
        FD_SET(s, &writefds);

        if (select(s+1, NULL, &writefds, NULL, &tv) != 0)
        {
            if (FD_ISSET(s,&writefds))
            {
                int len = sizeof(error);
                //下面的一句一定要，主要针对防火墙
                if (getsockopt(s, SOL_SOCKET, SO_ERROR, (char *)&error, &len) < 0)
                    goto error_ret;
                if (error != 0)
                    goto error_ret;
            }
            else
                goto error_ret; //timeout or error happen
        }
        else goto error_ret; ;

#ifdef WIN32
        ioctlsocket(s,FIONBIO,&blocking);
#else
        ioctl(s,FIONBIO,&blocking);
#endif

    }
    else
    {
error_ret:
        close(s);
    }
    return s;
}


int main(int argc, char *argv[])
{
    SOCKET fd;
    int n;

    /* parse command line etc ... */
    if (init(argc, argv) < 0)
    {
        print_usage();
        exit(1);
    }

    buf[0] = '{post.content}';

    /* pack the info into the buffer */

    strcpy(buf, "HelloWorld");

    /* make connection to the server */
    fd = tcp_connect(i_host, (unsigned short)atoi(i_port));

    if(xnet_select(fd, 0, 500, WRITE_STATUS)>0)
    {
        /* send off the message */
        write(fd, buf, strlen(buf));
    }
    else
    {
        err_quit("Socket I/O Write Timeout %s:%s/n", i_host, i_port);
    }

    if(xnet_select(fd, 3, 0, READ_STATUS)>0)
    {
        /* display the server response */
        printf("Server response:/n");
        n = read(fd, buf, BUFSIZE);
        buf[n] = '{post.content}';
        printf("%s/n", buf);
    }
    else
    {
        err_quit("Socket I/O Read Timeout %s:%s/n", i_host, i_port);
    }
    close(fd);

#ifdef WIN32
    WSACleanup();
#endif

    return 0;
}

BOOL TryConnect(UINT nIPv4, UINT16 u16DstPort)
{
    SOCKET fd = socket(res->ai_family, SOCK_STREAM, res->ai_protocol);
    if (fd < 0)
    {
        return -1;
    }

    /* make socket non-blocking */
    int flags = fcntl(fd, F_GETFL, 0);
    fcntl(fd, F_SETFL, flags | O_NONBLOCK);

    sockaddr_in remoteAddr = {0};
    remoteAddr.sin_family = AF_INET;
    remoteAddr.sin_port = htons(u16DstPort);
    remoteAddr.sin_addr.saddr = htonl(nIPv4);

    /* attempt to connect */
    int result = connect(fd, (const sockaddr*)&remoteAddr, sizeof(remoteAddr));

    /* immediately successful connect */
    if (result == 0)
    {
        result = 1;
        return TRUE;
    }
    else if (result < 0 && errno != EINPROGRESS)     /* connection error */
    {
        return FALSE;
    }
    else     /* connection in progress - wait for it... */
    {
        do
        {
            fd_set rfds;
            fd_set wfds;

            /* set connection timeout */
            struct timeval tv = {0};
            tv.tv_sec = timeout;
            tv.tv_usec = 0;

            FD_ZERO(&wfds);
            FD_SET(*sd, &wfds);
            rfds = wfds;

            /* wait for readiness */
            int sel = select(fd + 1, &rfds, &wfds, NULL, &tv);
            /* timeout */
            if (sel == 0)
            {
                /*printf("TIMEOUT\n");*/
                result = -1;
                break;
            }

            /* an error occurred */
            if (sel < 0 && errno != EINTR)
            {
                result = -1;
                break;
            }

            /* got something - check it */
            else if (result > 0)
            {
                int optval = 0;

                /* get socket options to check for errors */
                socklen_t optlen = sizeof(int);
                if (getsockopt(*sd, SOL_SOCKET, SO_ERROR, (void *)(&optval), &optlen) < 0)
                {
                    result = -1;
                    break;
                }

                /* an error occurred in the connection */
                if (optval != 0)
                {
                    result = -1;
                    break;
                }

                /* the connection was good! */
                /*
                   printf("CONNECT SELECT: ERRNO=%s\n",strerror(errno));
                   printf("CONNECT SELECT: OPTVAL=%s\n",strerror(optval));
                   */
                result = 1;
                break;
            }

            /* some other error occurred */
            else
            {
                result = -1;
                break;
            }

        }
        while (TRUE);
    }
    return result;
}
#endif

#include "sockprotocol.h"

static void showUseage()
{
}

static UINT64 GetTickNs()
{
    struct timeval tv = {0};
    gettimeofday(&tv, NULL);
    return (UINT64)tv.tv_sec*1000000000+(UINT64)tv.tv_usec*1000;
}

int main(int argc, char *argv[])
{
    struct option sock5ccOptions[] =
    {
        {
            "ipfiles",//long option name
            required_argument,//no_argument,required_argument,optional_argument
            NULL,//flags,
            'i',//index,
        },

        {
            "output",//long option name
            required_argument,//required_argument,optional_argument
            NULL,//flags,
            'o',//index,
        },
        {
            "help",
            no_argument,
            NULL,
            'h',
        },
        {
            "host",
            required_argument,
            NULL,
            't',
        },

        {
            NULL,
            0,
            0,
            0,
        },
    };

    int opt = 0;
    int index = 0;
    std::string inputfileName;
    std::string outputfileName;
    std::string host;

    while (( opt = getopt_long_only(argc, argv, "hi:o:t:", sock5ccOptions, &index) ) != -1)
    {
        switch (opt)
        {
        case 'i':
            printf("input file is %s\n", optarg);
            inputfileName = optarg;
            break;

        case 'o':
            printf("output file is %s\n", optarg);
            outputfileName = optarg;
            break;

        case 0:
            printf("debug is %s\n", optarg);
            break;

        case '?':
            printf("invalid option %s", optarg);
            break;

        case 't':
            host = optarg;
            break;

        case 'h':
            showUseage();
            return 0;
            break;
        }
    }


//	signal(SIGPIPE, SIG_IGN);
    {
        sigset_t set;
        sigemptyset(&set);
        sigaddset(&set, SIGPIPE);
        sigprocmask(SIG_BLOCK, &set, NULL);
    }

	std::ifstream fi(inputfileName.c_str());
    std::ofstream fo(outputfileName.c_str());  
	std::streambuf* pOutBuf = NULL;
    
    if (fo.is_open())
	{
   		pOutBuf = std::cout.rdbuf(fo.rdbuf()); 
	}

	std::string line;
	CSock5Client client;

	if (fi.is_open())
	{
   		while (getline(fi, line))
        {
            UINT nIP = inet_addr(line.c_str());
            if (nIP == INADDR_NONE)
            {
                printf("%s is not a valid IP\n", line.c_str());
                continue;
            }

            UINT64 tStartNs = GetTickNs();
            if (!client.Verify(nIP, 1080, 3000))
            {
                UINT64 tEndNs = GetTickNs();
                printf("%s failed connect, cost %llu nanoseconds\n", line.c_str(), tEndNs-tStartNs);
            }
            else
            {
                UINT64 tEndNs = GetTickNs();
                printf("%s connect OK, cost %llu seconds\n", line.c_str(), tEndNs-tStartNs);
                std::cout << line << std::endl;
            }
        }
        if (fo.is_open())
		{
			std::cout.rdbuf(pOutBuf);
	    	fo.close();
	    }
    }
    else if (host.size() > 0)
	{
   		UINT nIP = inet_addr(host.c_str());
   		if (nIP == INADDR_NONE)
        {
            printf("%s is not a valid IP\n", host.c_str());
            return -1;
        }
        UINT64 tStartNs = GetTickNs();
        if (!client.Verify(nIP, 1080, 3000))
        {
            UINT64 tEndNs = GetTickNs();
            printf("failed connect, cost %llu nanoseconds\n", tEndNs-tStartNs);
        }
        else
        {
            UINT64 tEndNs = GetTickNs();
            printf("%s connect OK, cost %llu seconds\n", line.c_str(), tEndNs-tStartNs);
        }
    }

    return 0;
}
