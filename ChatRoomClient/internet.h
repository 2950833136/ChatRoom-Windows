#pragma once
#ifndef _INTERNET_H_
#define _INTERNET_H_

#include <winsock2.h>
#include <stdio.h>
#include <process.h>

#pragma comment(lib, "ws2_32.lib")

#define TYPE_INFO 0     // 自身信息类型
#define TYPE_UPLOAD 1   // 上传文件类型
#define TYPE_DOWNLOAD 2 // 下载文件类型
#define TYPE_FRIEND 3   // 好友信息类型
#define TYPE_GROUP 4    // 分组信息类型
#define BUF_SIZE 1024   // 缓冲内存
#define NAME_SIZE 32    // 文件名字内存
#define MAX_SIZE 1076   // 数据包内存

// 数据包
typedef struct _DATA_PACKET  
{
    int type;
    int u_id;
    int to_id;
    int len;
    char buf[BUF_SIZE];
    int fin;
    char file_name[NAME_SIZE];
} DATA_PACKET;

void sendInfo(SOCKET s, int u_id);
void sendMessage(SOCKET s, DATA_PACKET data);
void sendFile(SOCKET s, DATA_PACKET Data);
void recvFile(SOCKET s, DATA_PACKET Data);
void recvThread(void *p);

#endif
