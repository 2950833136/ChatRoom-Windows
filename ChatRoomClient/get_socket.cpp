#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS

#include "get_socket.h"

SOCKET get_socket()
{
    CHAR SERVER_IP[] = "127.0.0.1"; // 服务端地址
    INT SERVER_PORT = 9999;         // 服务端端口

    INT Ret;                 // 调用各种Socket函数的返回值
    WSADATA wsadata;         // 用于初始化Windows Socket
    SOCKET cs;               // 与服务器进行通信的套接字
    SOCKADDR_IN server_addr; // 服务器地址
    INT server_addr_len;     // 服务器地址长度

    // 初始化Windows Socket
    if (WSAStartup(MAKEWORD(2, 2), &wsadata) != 0)
    {
        printf("WSAStartup failed!\n");
        return -1;
    }
    // 创建套接字
    cs = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (INVALID_SOCKET == cs)
    {
        printf("Socket failed!\n");
        WSACleanup();
        return -1;
    }
    // 连接服务器
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.S_un.S_addr = inet_addr(SERVER_IP); // 用户需要根据实际情况修改
    server_addr.sin_port = htons(SERVER_PORT);               // 在实际应用中，建议将服务器的IP地址和端口号保存在配置文件中
    server_addr_len = sizeof(server_addr);                   // 计算地址的长度
    Ret = connect(cs, (LPSOCKADDR)&server_addr, server_addr_len);
    if (Ret == SOCKET_ERROR)
    {
        printf("Connect failed !\n");
        WSACleanup();
        return -1;
    }
    // 设置套接字为非阻塞模式
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