#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS

#include <winsock2.h>
#pragma comment(lib, "ws2_32.lib")
#include "sql.h"
#include "manage_socket.h"

DWORD TotalSockets = 0;						  // ��¼����ʹ�õ��׽���������
LPSOCKET_INFORMATION SocketArray[FD_SETSIZE]; // ����Socket��Ϣ��������飬FD_SETSIZE��ʾSELECTģ�������������׽�������

int main(int argc, char *argv[])
{
	char IP[] = "127.0.0.1"; // ��������ַ
	int PORT = 9999;		 // �������˿�

	SOCKET ListenSocket;	  // �����׽���
	SOCKET AcceptSocket;	  // ��ͻ��˽���ͨ�ŵ��׽���
	SOCKET toSocket;		  //Ŀ�� socket
	SOCKADDR_IN InternetAddr; // �������ĵ�ַ
	INT InternetAddrLen;	  // �������ĵ�ַ����
	WSAData wsaData;		  // ���ڳ�ʼ���׽��ֻ���
	FD_SET WriteSet;		  // ��ȡ��д�Ե��׽��ּ���
	FD_SET ReadSet;			  // ��ȡ�ɶ��Ե��׽��ּ���
	DWORD Total;			  // ���ھ���״̬���׽�������
	INT Ret;				  // WinSock API�ķ���ֵ
	DATA_PACKET Data;		  //���ݰ�
	MYSQL mysql;			  // һ�����ݿ�ṹ��
	MYSQL_RES *res;			  // ��ά����
	MYSQL_ROW row;			  // һ������
	CHAR sql[SQL_SIZE];		  // sql ���

	mysql_init(&mysql);
	mysqlConnect(mysql);

	// ��ʼ��WinSock����
	if ((Ret = WSAStartup(MAKEWORD(2, 2), &wsaData)) != 0)
	{
		printf("WSAStartup() failed with error %d\n", Ret);
		WSACleanup();
		return -1;
	}
	// �������ڼ������׽���
	if ((ListenSocket = WSASocket(AF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED)) == INVALID_SOCKET)
	{
		printf("WSASocket() failed with error %d\n", WSAGetLastError());
		return -1;
	}
	// ���ü�����ַ�Ͷ˿ں�
	InternetAddr.sin_family = AF_INET;
	InternetAddr.sin_addr.s_addr = inet_addr(IP);
	InternetAddr.sin_port = htons(PORT);
	InternetAddrLen = sizeof(InternetAddr);
	// �󶨼����׽��ֵ����ص�ַ�Ͷ˿�
	if (bind(ListenSocket, (PSOCKADDR)&InternetAddr, InternetAddrLen) == SOCKET_ERROR)
	{
		printf("bind() failed with error %d\n", WSAGetLastError());
		return -1;
	}
	// ��ʼ����
	if (listen(ListenSocket, 100))
	{
		printf("listen() failed with error %d\n", WSAGetLastError());
		return -1;
	}
	printf("Listening...\n");
	// ����Ϊ������ģʽ
	ULONG NonBlock = 1;
	if (ioctlsocket(ListenSocket, FIONBIO, &NonBlock) == SOCKET_ERROR)
	{
		printf("ioctlsocket() failed with error %d\n", WSAGetLastError());
		return -1;
	}
	// ΪListenSocket�׽��ִ�����Ӧ��SOCKET_INFORMATION
	// �����Ϳ��԰�ListenSocket��ӵ�SocketArray������
	CreateSocketInformation(ListenSocket);
	while (TRUE)
	{
		// ׼����������I/O֪ͨ�Ķ�/д�׽��ּ���
		FD_ZERO(&ReadSet);
		FD_ZERO(&WriteSet);
		// ��ReadSet��������Ӽ����׽���ListenSocket
		FD_SET(ListenSocket, &ReadSet);
		// ��SocketArray�����е������׽�����ӵ�WriteSet��ReadSet������
		// SocketArray�����б����ż����׽��ֺ�������ͻ��˽���ͨ�ŵ��׽���
		// �����Ϳ���ʹ��select()�ж��ĸ��׽����н������ݻ��߶�ȡ/д������
		for (DWORD i = 0; i < TotalSockets; i++)
		{
			LPSOCKET_INFORMATION SocketInfo = SocketArray[i];
			FD_SET(SocketInfo->Socket, &WriteSet);
			FD_SET(SocketInfo->Socket, &ReadSet);
		}
		// �ж϶�/д�׽��ּ����о������׽���
		if ((Total = select(0, &ReadSet, &WriteSet, NULL, NULL)) == SOCKET_ERROR)
		{
			printf("select() returned with error %d\n", WSAGetLastError());
			return -1;
		}
		// ���δ��������׽��֡�����������һ����Ӧ�������������ӿͻ����յ����ַ����ٷ��ص��ͻ��ˡ�
		for (DWORD i = 0; i < TotalSockets; i++)
		{
			LPSOCKET_INFORMATION SocketInfo = SocketArray[i]; // SocketInfoΪ��ǰҪ������׽�����Ϣ
			// �жϵ�ǰ�׽��ֵĿɶ��ԣ����Ƿ��н��������������߿��Խ�������
			if (FD_ISSET(SocketInfo->Socket, &ReadSet))
			{
				// ���ڼ����׽�����˵���ɶ���ʾ���µ���������
				if (SocketInfo->Socket == ListenSocket)
				{
					Total--; // �������׽��ּ�1
					// �����������󣬵õ���ͻ��˽���ͨ�ŵ��׽���AcceptSocket
					if ((AcceptSocket = accept(ListenSocket, NULL, NULL)) != INVALID_SOCKET)
					{
						// �����׽���AcceptSocketΪ������ģʽ
						NonBlock = 1;
						if (ioctlsocket(AcceptSocket, FIONBIO, &NonBlock) == SOCKET_ERROR)
						{
							printf("ioctlsocket() failed with error %d\n", WSAGetLastError());
							return -1;
						}
						// �����׽�����Ϣ����ʼ��LPSOCKET_INFORMATION�ṹ�����ݣ���AcceptSocket��ӵ�SocketArray������
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
				// ��������
				else
				{
					// �����ǰ�׽�����ReadSet�����У���������׽������п��Զ�ȡ������
					if (FD_ISSET(SocketInfo->Socket, &ReadSet))
					{
						Total--; // ����һ�����ھ���״̬���׽���
						// ��������
						memset(&Data, 0, sizeof(Data));
						Ret = recv(SocketInfo->Socket, (char *)&Data, sizeof(Data), 0);
						if (Ret == SOCKET_ERROR)
						{
							// ����������WSAEWOULDBLOCK��ʾ��û�����ݣ������ʾ�����쳣
							if (WSAGetLastError() != WSAEWOULDBLOCK)
							{
								memset(sql, 0, SQL_SIZE);
								sprintf(sql, "UPDATE t_user a SET a.state = 0 WHERE a.user_id = %d;", SocketInfo->Uid);
								mysqlUpdate(mysql, sql);
								printf("WSARecv() failed with error %d\n", WSAGetLastError());
								FreeSocketInformation(i); // �ͷ��׽�����Ϣ
							}
							continue;
						}
						else
						{
							// ������յ�0���ֽڣ����ʾ�Է��ر�����
							if (Ret == 0)
							{
								memset(sql, 0, SQL_SIZE);
								sprintf(sql, "UPDATE t_user a SET a.state = 0 WHERE a.user_id = %d;", SocketInfo->Data.u_id);
								mysqlUpdate(mysql, sql);
								FreeSocketInformation(i);
								continue;
							}
							// �ɹ���������
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
				// �����ǰ�׽�����WriteSet�����У���������׽��ֵ��ڲ����ݻ������������ݿ��Է���
				if (FD_ISSET(SocketInfo->Socket, &WriteSet))
				{
					Total--; // ����һ�����ھ���״̬���׽���
					// �������Ҫ���͵����ݣ���������
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
						// ����������WSAEWOULDBLOCK��ʾ��û�����ݣ������ʾ�����쳣
						// if (WSAGetLastError() != WSAEWOULDBLOCK)
						// {
						//	 FreeSocketInformation(i); // �ͷ��׽�����Ϣ
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