#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS

#include "get_socket.h"

SOCKET get_socket()
{
    CHAR SERVER_IP[] = "127.0.0.1"; // ����˵�ַ
    INT SERVER_PORT = 9999;         // ����˶˿�

    INT Ret;                 // ���ø���Socket�����ķ���ֵ
    WSADATA wsadata;         // ���ڳ�ʼ��Windows Socket
    SOCKET cs;               // �����������ͨ�ŵ��׽���
    SOCKADDR_IN server_addr; // ��������ַ
    INT server_addr_len;     // ��������ַ����

    // ��ʼ��Windows Socket
    if (WSAStartup(MAKEWORD(2, 2), &wsadata) != 0)
    {
        printf("WSAStartup failed!\n");
        return -1;
    }
    // �����׽���
    cs = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (INVALID_SOCKET == cs)
    {
        printf("Socket failed!\n");
        WSACleanup();
        return -1;
    }
    // ���ӷ�����
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.S_un.S_addr = inet_addr(SERVER_IP); // �û���Ҫ����ʵ������޸�
    server_addr.sin_port = htons(SERVER_PORT);               // ��ʵ��Ӧ���У����齫��������IP��ַ�Ͷ˿ںű����������ļ���
    server_addr_len = sizeof(server_addr);                   // �����ַ�ĳ���
    Ret = connect(cs, (LPSOCKADDR)&server_addr, server_addr_len);
    if (Ret == SOCKET_ERROR)
    {
        printf("Connect failed !\n");
        WSACleanup();
        return -1;
    }
    // �����׽���Ϊ������ģʽ
    u_long iMode = 1;
    Ret = ioctlsocket(cs, FIONBIO, &iMode);
    if (Ret == SOCKET_ERROR)
    {
        printf("Ioctlsocket failed !\n");
        WSACleanup();
        return -1;
    }
    client_socket = cs;
    return cs;
}