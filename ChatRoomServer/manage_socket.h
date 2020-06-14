#pragma once
#ifndef _MANAGE_SOCKET_H_
#define _MANAGE_SOCKET_H_

#include "internet.h"

// socket 结构体
typedef struct _SOCKET_INFORMATION
{
	INT Uid;		  // 用户 id
	SOCKET Socket;	  // 用户连接 socket
	DATA_PACKET Data; // 用户数据包
} SOCKET_INFORMATION, *LPSOCKET_INFORMATION;

BOOL CreateSocketInformation(SOCKET s);
void FreeSocketInformation(DWORD Index);
SOCKET findSocket(int u_id);

#endif
