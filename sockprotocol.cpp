#include "stdafx.h"
#include "sock5.h"
#include "sockprotocol.h"

#define NOBLOCK_MODE
#define TX_NOBLOCK_MODE 

BOOL CSockClient::CreateWithConnect(UINT nIPv4, UINT16 u16DstPort, ULONG uTimeOutMs)
{
	int flags = 0;
	int result = 0;
	sockaddr_in remoteAddr = {0};

	SOCKET fd = socket(AF_INET, SOCK_STREAM, 0);
    if (fd < 0) 
    {
    	goto __ERROR_QUIT;
    }
	remoteAddr.sin_family = AF_INET;
	remoteAddr.sin_port = htons(u16DstPort);
	remoteAddr.sin_addr.s_addr = (nIPv4);

#ifdef NOBLOCK_MODE
    /* make socket non-blocking */
    flags = fcntl(fd, F_GETFL, 0);
    fcntl(fd, F_SETFL, flags | O_NONBLOCK);

    /* attempt to connect */
    result = connect(fd, (const sockaddr*)&remoteAddr, sizeof(remoteAddr));

    /* immediately successful connect */
    if (result == 0) 
    {
    	m_nTimeOutMs = uTimeOutMs;
    	m_fd = fd;
    	return TRUE;	
    }
    else if (result < 0 && errno != EINPROGRESS)     /* connection error */
    {
    	//DoNothing;
    	goto __CONNECT_ERROR_QUIT;
    }
    else     /* connection in progress - wait for it... */
    {
        do 
        {
       	    fd_set rfds;
			fd_set wfds;

            /* set connection timeout */
            struct timeval tv = {0};
            tv.tv_sec = uTimeOutMs/1000;
            tv.tv_usec = uTimeOutMs%1000;

            FD_ZERO(&wfds);
            FD_SET(fd, &wfds);
            rfds = wfds;

            /* wait for readiness */
            int sel = select(fd + 1, &rfds, &wfds, NULL, &tv);
            /* timeout */
            if (sel == 0) 
            {
                /*printf("TIMEOUT\n");*/
                break;
            }

            /* an error occurred */
            if (sel < 0 && errno != EINTR) 
            {
                break;
            }
            /* got something - check it */
            else //sel > 0 
            {
				int optval = 0;

                /* get socket options to check for errors */
                socklen_t optlen = sizeof(int);
                if (getsockopt(fd, SOL_SOCKET, SO_ERROR, (void *)(&optval), &optlen) < 0) 
                {
                    break;
                }

                /* an error occurred in the connection */
                if (optval != 0) 
                {
                    break;
                }

                /* the connection was good! */
                /*
                   printf("CONNECT SELECT: ERRNO=%s\n",strerror(errno));
                   printf("CONNECT SELECT: OPTVAL=%s\n",strerror(optval));
                   */
               	m_nTimeOutMs = uTimeOutMs;
                m_fd = fd;
                return TRUE;
                break;
            }
        }
        while (TRUE);
    }
#else
	if (connect(fd, (const sockaddr*)&remoteAddr, sizeof(remoteAddr)) == 0)
	{
		m_fd = fd;
		return TRUE;
	}    
	printf("connect remote faild,errno =%d\n", errno);
#endif
    
__CONNECT_ERROR_QUIT:
	closesocket(fd);
	
__ERROR_QUIT:
	return FALSE;
}

void CSockClient::Close() const
{
	if (m_fd != INVALID_SOCKET)
	{
		closesocket(m_fd);
	}
}

void CSockClient::Reset()
{
	Close();
	m_fd = INVALID_SOCKET;
}

ssize_t CSockClient::Tx(const void* pData, size_t txLen)
{
	SOCKET fd = m_fd;

	fd_set set;
	FD_ZERO(&set);
	FD_SET(fd, &set);

	struct timeval tp = { 0 };
	tp.tv_sec = m_nTimeOutMs / 1000;
	tp.tv_usec = (m_nTimeOutMs % 1000) * 1000;
	struct timeval* pTp = &tp;
	
	if (m_nTimeOutMs == WAIT_FOREVER)
	{
		pTp = NULL;
	}
	const char* pSrc = (const char*)pData;
	size_t offset = 0;
	for (; offset<txLen;)
	{
		char szTxBuffer[2048] = { 0 };

		size_t copyLen = (txLen-offset)>sizeof(szTxBuffer) ? sizeof(szTxBuffer) : (txLen-offset);
		memcpy(szTxBuffer, &pSrc[offset], copyLen);
		int sel = 0;
		sel = select(fd + 1, NULL, &set, NULL, pTp);
		
#ifdef TX_NOBLOCK_MODE
		if (FD_ISSET(fd, &set))
		{
		    int err = -1;
		    socklen_t len = sizeof(int);
			if (getsockopt(fd,  SOL_SOCKET, SO_ERROR ,&err, &len) < 0 )
			{
			    printf("errno:%d %s\n", errno, strerror(errno));
			    return FALSE;
			}
			
			if (err)
			{
			    errno = err;
			    return FALSE;
			}
		}
#else
		if (sel <= 0)
		{
			printf("select faile, errno=%d\n", errno);
			_ASSERT(FALSE);
			return offset;
		}
#endif
		
		if ((ssize_t)copyLen != send(fd, (const char*)szTxBuffer, copyLen, 0))
		{
			//printf("Send failed, errno=%d\n", CXSocket::GetLastSocketError());
			return offset;
		}
		offset += copyLen;
	}
	return offset;
}

ssize_t CSockClient::Rx(void* pBuffer, size_t bufLen)
{
	SOCKET fd = m_fd;
	fd_set set;
	FD_ZERO(&set);
	FD_SET(fd, &set);

	struct timeval tp = { 0 };
	tp.tv_sec = m_nTimeOutMs / 1000;
	tp.tv_usec = (m_nTimeOutMs % 1000) * 1000;
	struct timeval* pTp = &tp;
	
	if (m_nTimeOutMs == WAIT_FOREVER)
	{
		pTp = NULL;
	}

	int sel = select(fd + 1, &set, NULL, NULL, pTp);
	if (sel == 1)
	{
		ssize_t ret = recv(fd, (char*)pBuffer, bufLen, 0);
		if (ret <= 0)
		{
			return -1;//远程连接断开
		}
		return ret;
	}
	else if (sel == 0)
	{
		return 0;
	}

	return -1;
}
	
ssize_t CSock5Client::BuildTxData(void* pData, size_t dataLen)
{
	//TODO:dataLen
	
	UINT8* pTxPtr = (UINT8*)pData;
	UINT nIdx = 0;
	pTxPtr[nIdx++] = 0x05;
	pTxPtr[nIdx++] = 1;	//number of Methods
	pTxPtr[nIdx++] = SOCK5_METHOD_NOAUTH;
	
	return (ssize_t)nIdx;
}
	
BOOL CSock5Client::VerifyData(const void* pData, size_t rxLen)
{
	const UINT8* pRxPtr = (const UINT8*)pData;
	if (rxLen < 2)
	{
		return FALSE;
	}
	
	return pRxPtr[0]==0x05 && pRxPtr[1]==0x00;
}
