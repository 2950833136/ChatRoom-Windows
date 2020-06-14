#define _CRT_SECURE_NO_WARNINGS

#include "manage_socket.h"

extern DWORD TotalSockets;							 // ��¼����ʹ�õ��׽���������
extern LPSOCKET_INFORMATION SocketArray[FD_SETSIZE]; // ����Socket��Ϣ��������飬FD_SETSIZE��ʾSELECTģ�������������׽�������

// ���� id ��ѯ��Ӧ socket
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

// ����SOCKET��Ϣ
BOOL CreateSocketInformation(SOCKET s)
{
	LPSOCKET_INFORMATION SI; // ���ڱ����׽��ֵ���Ϣ
	printf("Accepted socket id %d, Now TotalSockets is %d\n", s, TotalSockets);
	// ΪSI�����ڴ�ռ�
	if ((SI = (LPSOCKET_INFORMATION)GlobalAlloc(GPTR, sizeof(SOCKET_INFORMATION))) == NULL)
	{
		printf("GlobalAlloc() failed with error %d\n", GetLastError());
		return FALSE;
	}
	// ��ʼ��SI��ֵ
	SI->Socket = s;
	memset(&SI->Data, 0, sizeof(SI->Data));
	// ��SocketArray����������һ����Ԫ�أ����ڱ���SI����
	SocketArray[TotalSockets] = SI;
	TotalSockets++; // �����׽�������
	return TRUE;
}

// ������SocketArray��ɾ��ָ����LPSOCKET_INFORMATION����
void FreeSocketInformation(DWORD Index)
{
	LPSOCKET_INFORMATION SI = SocketArray[Index]; // ��ȡָ��������Ӧ��LPSOCKET_INFORMATION����
	DWORD i;
	// �ر��׽���
	printf("Closing socket id %d, Now TotalSockets is %d\n", SI->Socket, TotalSockets - 2);
	closesocket(SI->Socket);
	// �ͷ�ָ��LPSOCKET_INFORMATION������Դ
	GlobalFree(SI);
	// ��������index���������Ԫ��ǰ��
	for (i = Index; i < TotalSockets; i++)
	{
		SocketArray[i] = SocketArray[i + 1];
	}
	TotalSockets--; // �׽���������1
}