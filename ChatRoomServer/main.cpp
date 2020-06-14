#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS

#include <winsock2.h>
#pragma comment(lib, "ws2_32.lib")
#include "sql.h"
#include "manage_socket.h"

DWORD TotalSockets = 0;						  // 记录正在使用的套接字总数量
LPSOCKET_INFORMATION SocketArray[FD_SETSIZE]; // 保存Socket信息对象的数组，FD_SETSIZE表示SELECT模型中允许的最大套接字数量

int main(int argc, char *argv[])
{
	char IP[] = "127.0.0.1"; // 服务器地址
	int PORT = 9999;		 // 服务器端口

	SOCKET ListenSocket;	  // 监听套接字
	SOCKET AcceptSocket;	  // 与客户端进行通信的套接字
	SOCKET toSocket;		  //目的 socket
	SOCKADDR_IN InternetAddr; // 服务器的地址
	INT InternetAddrLen;	  // 服务器的地址长度
	WSAData wsaData;		  // 用于初始化套接字环境
	FD_SET WriteSet;		  // 获取可写性的套接字集合
	FD_SET ReadSet;			  // 获取可读性的套接字集合
	DWORD Total;			  // 处于就绪状态的套接字数量
	INT Ret;				  // WinSock API的返回值
	DATA_PACKET Data;		  //数据包
	MYSQL mysql;			  // 一个数据库结构体
	MYSQL_RES *res;			  // 二维数据
	MYSQL_ROW row;			  // 一行数据
	CHAR sql[SQL_SIZE];		  // sql 语句

	mysql_init(&mysql);
	mysqlConnect(mysql);

	// 初始化WinSock环境
	if ((Ret = WSAStartup(MAKEWORD(2, 2), &wsaData)) != 0)
	{
		printf("WSAStartup() failed with error %d\n", Ret);
		WSACleanup();
		return -1;
	}
	// 创建用于监听的套接字
	if ((ListenSocket = WSASocket(AF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED)) == INVALID_SOCKET)
	{
		printf("WSASocket() failed with error %d\n", WSAGetLastError());
		return -1;
	}
	// 设置监听地址和端口号
	InternetAddr.sin_family = AF_INET;
	InternetAddr.sin_addr.s_addr = inet_addr(IP);
	InternetAddr.sin_port = htons(PORT);
	InternetAddrLen = sizeof(InternetAddr);
	// 绑定监听套接字到本地地址和端口
	if (bind(ListenSocket, (PSOCKADDR)&InternetAddr, InternetAddrLen) == SOCKET_ERROR)
	{
		printf("bind() failed with error %d\n", WSAGetLastError());
		return -1;
	}
	// 开始监听
	if (listen(ListenSocket, 100))
	{
		printf("listen() failed with error %d\n", WSAGetLastError());
		return -1;
	}
	printf("Listening...\n");
	// 设置为非阻塞模式
	ULONG NonBlock = 1;
	if (ioctlsocket(ListenSocket, FIONBIO, &NonBlock) == SOCKET_ERROR)
	{
		printf("ioctlsocket() failed with error %d\n", WSAGetLastError());
		return -1;
	}
	// 为ListenSocket套接字创建对应的SOCKET_INFORMATION
	// 这样就可以把ListenSocket添加到SocketArray数组中
	CreateSocketInformation(ListenSocket);
	while (TRUE)
	{
		// 准备用于网络I/O通知的读/写套接字集合
		FD_ZERO(&ReadSet);
		FD_ZERO(&WriteSet);
		// 向ReadSet集合中添加监听套接字ListenSocket
		FD_SET(ListenSocket, &ReadSet);
		// 将SocketArray数组中的所有套接字添加到WriteSet和ReadSet集合中
		// SocketArray数组中保存着监听套接字和所有与客户端进行通信的套接字
		// 这样就可以使用select()判断哪个套接字有接入数据或者读取/写入数据
		for (DWORD i = 0; i < TotalSockets; i++)
		{
			LPSOCKET_INFORMATION SocketInfo = SocketArray[i];
			FD_SET(SocketInfo->Socket, &WriteSet);
			FD_SET(SocketInfo->Socket, &ReadSet);
		}
		// 判断读/写套接字集合中就绪的套接字
		if ((Total = select(0, &ReadSet, &WriteSet, NULL, NULL)) == SOCKET_ERROR)
		{
			printf("select() returned with error %d\n", WSAGetLastError());
			return -1;
		}
		// 依次处理所有套接字。本服务器是一个回应服务器，即将从客户端收到的字符串再发回到客户端。
		for (DWORD i = 0; i < TotalSockets; i++)
		{
			LPSOCKET_INFORMATION SocketInfo = SocketArray[i]; // SocketInfo为当前要处理的套接字信息
			// 判断当前套接字的可读性，即是否有接入的连接请求或者可以接收数据
			if (FD_ISSET(SocketInfo->Socket, &ReadSet))
			{
				// 对于监听套接字来说，可读表示有新的连接请求
				if (SocketInfo->Socket == ListenSocket)
				{
					Total--; // 就绪的套接字减1
					// 接受连接请求，得到与客户端进行通信的套接字AcceptSocket
					if ((AcceptSocket = accept(ListenSocket, NULL, NULL)) != INVALID_SOCKET)
					{
						// 设置套接字AcceptSocket为非阻塞模式
						NonBlock = 1;
						if (ioctlsocket(AcceptSocket, FIONBIO, &NonBlock) == SOCKET_ERROR)
						{
							printf("ioctlsocket() failed with error %d\n", WSAGetLastError());
							return -1;
						}
						// 创建套接字信息，初始化LPSOCKET_INFORMATION结构体数据，将AcceptSocket添加到SocketArray数组中
						if (CreateSocketInformation(AcceptSocket) == FALSE)
						{
							return -1;
						}
					}
					else
					{
						if (WSAGetLastError() != WSAEWOULDBLOCK)
						{
							printf("accept() failed with error %d\n", WSAGetLastError());
							return -1;
						}
					}
				}
				// 接收数据
				else
				{
					// 如果当前套接字在ReadSet集合中，则表明该套接字上有可以读取的数据
					if (FD_ISSET(SocketInfo->Socket, &ReadSet))
					{
						Total--; // 减少一个处于就绪状态的套接字
						// 接收数据
						memset(&Data, 0, sizeof(Data));
						Ret = recv(SocketInfo->Socket, (char *)&Data, sizeof(Data), 0);
						if (Ret == SOCKET_ERROR)
						{
							// 错误编码等于WSAEWOULDBLOCK表示暂没有数据，否则表示出现异常
							if (WSAGetLastError() != WSAEWOULDBLOCK)
							{
								memset(sql, 0, SQL_SIZE);
								sprintf(sql, "UPDATE t_user a SET a.state = 0 WHERE a.user_id = %d;", SocketInfo->Uid);
								mysqlUpdate(mysql, sql);
								printf("WSARecv() failed with error %d\n", WSAGetLastError());
								FreeSocketInformation(i); // 释放套接字信息
							}
							continue;
						}
						else
						{
							// 如果接收到0个字节，则表示对方关闭连接
							if (Ret == 0)
							{
								memset(sql, 0, SQL_SIZE);
								sprintf(sql, "UPDATE t_user a SET a.state = 0 WHERE a.user_id = %d;", SocketInfo->Data.u_id);
								mysqlUpdate(mysql, sql);
								FreeSocketInformation(i);
								continue;
							}
							// 成功接收数据
							else
							{
								switch (Data.type)
								{
								case TYPE_INFO:
									SocketInfo->Uid = Data.u_id;
									printf("%d %d\n", Data.u_id, SocketInfo->Socket);
									break;
								case TYPE_UPLOAD:
									recvFile(SocketInfo->Socket, Data);
									break;
								case TYPE_DOWNLOAD:
									sendFile(SocketInfo->Socket, Data);
									break;
								default:
									SocketInfo->Data = Data;
									printf("%s\n", Data.buf);
									break;
								}
							}
						}
					}
				}
			}
			else
			{
				// 如果当前套接字在WriteSet集合中，则表明该套接字的内部数据缓冲区中有数据可以发送
				if (FD_ISSET(SocketInfo->Socket, &WriteSet))
				{
					Total--; // 减少一个处于就绪状态的套接字
					// 如果有需要发送的数据，则发送数据
					if (SocketInfo->Data.len > 0)
					{
						switch (SocketInfo->Data.type)
						{
						case TYPE_FRIEND:
							toSocket = findSocket(SocketInfo->Data.to_id);
							printf("%d to %d\n", SocketInfo->Socket, toSocket);
							transmit(toSocket, SocketInfo->Data);
							break;
						case TYPE_GROUP:
							memset(sql, 0, SQL_SIZE);
							sprintf(sql, "SELECT b.user_id FROM t_user_group a LEFT JOIN t_user b ON a.user_id = b.user_id WHERE a.group_id = %d AND b.state = 1 AND b.user_id != %d;", SocketInfo->Data.to_id, SocketInfo->Uid);
							res = mysqlSelect(mysql, sql);
							while (row = mysql_fetch_row(res))
							{
								SocketInfo->Data.to_id = atoi(row[0]);
								toSocket = findSocket(SocketInfo->Data.to_id);
								printf("%d to %d\n", SocketInfo->Socket, toSocket);
								transmit(toSocket, SocketInfo->Data);
							}
							break;
						}
						// 错误编码等于WSAEWOULDBLOCK表示暂没有数据，否则表示出现异常
						// if (WSAGetLastError() != WSAEWOULDBLOCK)
						// {
						//	 FreeSocketInformation(i); // 释放套接字信息
						// }
						memset(&SocketInfo->Data, 0, sizeof(SocketInfo->Data));
					}
				}
			}
		}
	}

	mysql_close(&mysql);
	return 0;
}