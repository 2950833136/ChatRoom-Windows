#define _CRT_SECURE_NO_WARNINGS

#include "manage_socket.h"

extern DWORD TotalSockets;							 // 记录正在使用的套接字总数量
extern LPSOCKET_INFORMATION SocketArray[FD_SETSIZE]; // 保存Socket信息对象的数组，FD_SETSIZE表示SELECT模型中允许的最大套接字数量

// 根据 id 查询对应 socket
SOCKET findSocket(int to_id)
{
	for (DWORD i = 0; i < TotalSockets; i++)
	{
		LPSOCKET_INFORMATION SocketInfo = SocketArray[i];
		if (SocketInfo->Uid == to_id)
		{
			return SocketArray[i]->Socket;
		}
	}
	return NULL;
}

// 创建SOCKET信息
BOOL CreateSocketInformation(SOCKET s)
{
	LPSOCKET_INFORMATION SI; // 用于保存套接字的信息
	printf("Accepted socket id %d, Now TotalSockets is %d\n", s, TotalSockets);
	// 为SI分配内存空间
	if ((SI = (LPSOCKET_INFORMATION)GlobalAlloc(GPTR, sizeof(SOCKET_INFORMATION))) == NULL)
	{
		printf("GlobalAlloc() failed with error %d\n", GetLastError());
		return FALSE;
	}
	// 初始化SI的值
	SI->Socket = s;
	memset(&SI->Data, 0, sizeof(SI->Data));
	// 在SocketArray数组中增加一个新元素，用于保存SI对象
	SocketArray[TotalSockets] = SI;
	TotalSockets++; // 增加套接字数量
	return TRUE;
}

// 从数组SocketArray中删除指定的LPSOCKET_INFORMATION对象
void FreeSocketInformation(DWORD Index)
{
	LPSOCKET_INFORMATION SI = SocketArray[Index]; // 获取指定索引对应的LPSOCKET_INFORMATION对象
	DWORD i;
	// 关闭套接字
	printf("Closing socket id %d, Now TotalSockets is %d\n", SI->Socket, TotalSockets - 2);
	closesocket(SI->Socket);
	// 释放指定LPSOCKET_INFORMATION对象资源
	GlobalFree(SI);
	// 将数组中index索引后面的元素前移
	for (i = Index; i < TotalSockets; i++)
	{
		SocketArray[i] = SocketArray[i + 1];
	}
	TotalSockets--; // 套接字总数减1
}