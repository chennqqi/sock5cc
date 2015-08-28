#ifndef __SOCK5_H__
#define __SOCK5_H__

#define SOCK_SERVER_TCP_PORT 1080U

struct SOCK4_HDR
{
	UINT8 u8Ver;
	UINT8 u8Method;
	UINT16 u16PortBE;
	UINT32 u32IPv4;
	char szId[1];/*End with eos*/
};

struct SOCK4A_HDR
{
	UINT8 u8Ver;
	UINT8 u8Method;
	UINT16 u16PortBE;
	UINT32 u32IPv4;	/*Always BigEndian(0x000000001)*/
	char szId[1]; /*End with eos*/

/*	char szDomain[1];	*/
};

#define SOCK5_METHOD_NOAUTH 0x00
#define SOCK5_METHOD_GSSAPI 0x01
#define SOCK5_MEHTOD_PASS 0x02
#define SOCK5_METHOD_NANA_BEGIN 0x03
#define SOCK5_METHOD_NANA_END 0x7F
#define SOCK5_METHOD_PRIVATE 0x80
#define SOCK5_METHOD_NOACCEPTBLE  0xFF

struct SOCK5_SYN /*C->S*/
{
	UINT8 u8Ver; /* 0x05*/
	UINT8 u8nMethod; /* 0x01* user passwd*/
	UINT8 u8xx[255];/* 1~255 */
};

struct SOCK5_ACK /*S->C*/
{
	UINT8 u8Ver;
	UINT8 u8Method;
};

struct SOCK5_AUTH_HDR
{
	UINT8 u81;/*0x01*/
	UINT8 u8LenUser;
};

struct SOCK5_AUTH_USER_TRUNK
{
	UINT8 u8Len;
	char szData[1]; /* strlen(szData)==u8Len;*/
};

#define SOCK5_TX_CMD_CONNECT 0x01
#define SOCK5_TX_CMD_BIND 0x02
#define SOCK5_TX_CMD_UDP 0x03

#define SOCK5_ADDRTYPE_IPV4 0x01
#define SOCK5_ADDRTYPE_DOMAIN 0x03
#define SOCK5_ADDRTYPE_IPV6 0x04

struct SOCK5_FORWARD_REQ
{
	UINT8 u8Ver;
	UINT8 u8Cmd;
	UINT8 u8Rsv; /*always 0x00*/
	UINT8 u8Atyp;
/*/	struct rmtAddr;*/
};

struct SOCK5_ADDR_IPV6
{
	UINT8 u8IPv6[16];
	UINT16 u16DstPort;
};

struct SOCK5_ADDR_IPV4
{
	UINT32 u32IPv4;
	UINT16 u16DstPort;
};

struct SOCK5_ADDR_DOMAIN
{
	UINT8 u8Len;
	char szDomain[1];/* none NULL len=u8Len*/
	/*UINT16 u16DstPort;*/
};

struct SOCK5_FORWARD_RESP
{
	UINT8 u8Ver;
	UINT8 u8Cmd;
	UINT8 u8Rsv;
	UINT8 u8Atyp;
/*	struct bindAddr;*/
/* UINT16 bindPort*/
};

#endif /*__SOCK5_H__*/
