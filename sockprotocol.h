/*! sockprotocol.h
********************************************************************************
                          #    #  ####  #####  #####  
                           #  #  #    # #    # #    # 
                            ##   #      #    # #    # 
                            ##   #      #####  #####  
                           #  #  #    # #      #      
                          #    #  ####  #      #      
--------------------------------------------------------------------------------
description			:  A simple cross platform tcp socket class
related files		:  sockprotocol.h
create date			:  2015-08-18
author				:  CHENQ
version				:  v0.1
--------------------------------------------------------------------------------
If you find any bugs, please e-mail me chennqqi@qq.com
--------------------------------------------------------------------------------

history :
2015-08-18			CHENQ	created
2015-08-20			CHENQ 	version 0.1 release
*******************************************************************************/


#ifndef __SOCK_PROTOCOL_H__
#define __SOCK_PROTOCOL_H__

/** @detail
	CSockClient sock;
	char Tx[] = "122345666";
	char Rx[] = "1111111111";
	for (ip in iploops, port)
	{
		if (!sock.VerifyConnect(ip,port))
		{
			//failed
			Reset();
			continue;
		}
		sock.BuildTxData(Tx, strlen(Tx));
		if (!sock.Wait(timeout))
		{
			//timeout;
			Reset();
			continue;
		}		
	}


*******************************************************************************/


#define SOCKS_DEFAULT_TIMEOUTMS 3000U

class CSockClient
{
public:
	enum SockVersion_t
	{
		Sock4 = 0,
		Sock4A,
		Sock5,
	};
	CSockClient(SockVersion_t ver) : m_ver(ver), 
		m_nTimeOutMs(SOCKS_DEFAULT_TIMEOUTMS), m_fd(INVALID_SOCKET)
	{
	}
	virtual ~CSockClient()
	{}
	
	
	virtual ssize_t BuildTxData(void* pData, size_t dataLen) = 0;
	
	virtual BOOL VerifyData(const void* pData, size_t rxLen) = 0;
	
	virtual BOOL VerifyProc(void* pData, size_t bufLen)
	{
		ssize_t txLen = BuildTxData(pData, bufLen);
		if (txLen <= 0)
		{
			return FALSE;
		}
		if (txLen != Tx(pData, txLen))
		{
			return FALSE;
		}
		
		ssize_t rxLen = Rx(pData, bufLen);
		if (rxLen <= 0)
		{
			return FALSE;
		}
		return VerifyData(pData, rxLen);
	}
	virtual BOOL Verify(UINT nIPv4, UINT16 u16DstPort, ULONG uTimeOutMs)
	{
		char szBuffer[64] = {0};
		if (!CreateWithConnect(nIPv4, u16DstPort, uTimeOutMs))
		{
			return FALSE;
		}
		BOOL bRet = (VerifyProc(szBuffer, sizeof(szBuffer)));
		Reset();
		return bRet;
	}


	void Reset();
	
protected:
	void Close() const;
	BOOL CreateWithConnect(UINT nIPv4, UINT16 u16DstPort, ULONG uTimeOutMs);
	ssize_t Tx(const void* pData, size_t txLen);
	ssize_t Rx(void* pData, size_t bufLen);
	
private:
	const SockVersion_t m_ver;
	UINT m_nTimeOutMs;
	SOCKET m_fd;
};

class CSock5Client : public CSockClient
{
public:
	CSock5Client() : CSockClient(Sock5)
	{
	}
	
	virtual ssize_t BuildTxData(void* pData, size_t dataLen);
	
	virtual BOOL VerifyData(const void* pData, size_t rxLen);
		
};

#endif /*__SOCK_PROTOCOL_H__*/