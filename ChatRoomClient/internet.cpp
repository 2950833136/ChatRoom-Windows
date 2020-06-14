#define _CRT_SECURE_NO_WARNINGS

#include "internet.h"

// 发送自己 U_ID
void sendInfo(SOCKET s, int u_id)
{
    DATA_PACKET Data;
    memset(&Data, 0, sizeof(Data));
    Data.u_id = u_id;
    Data.type = TYPE_INFO;
    send(s, (char *)&Data, sizeof(Data), 0);
}

// 发送消息
void sendMessage(SOCKET s, DATA_PACKET data)
{
    DATA_PACKET Data;
    int ret;

    while (true)
    {
        memset(&Data, 0, sizeof(Data));
        printf("[Send]: ");
        fgets(Data.buf, BUF_SIZE, stdin);
        Data.len = strlen(Data.buf);
        Data.buf[Data.len - 1] = '\0';
        Data.u_id = data.u_id;
        Data.to_id = data.to_id;
        Data.type = data.type;
        ret = send(s, (char *)&Data, sizeof(Data), 0);
        if (ret == SOCKET_ERROR)
        {
            printf("Send failed !");
            continue;
        }
        if (strcmp("quit", Data.buf) == 0)
        {
            break;
        }
        Sleep(300);
    }
}

// 线程接收消息
void recvThread(void *p)
{
    SOCKET recv_socket = *(SOCKET *)p;
    DATA_PACKET Data;
    int ret;

    while (true)
    {
        memset(&Data, 0, sizeof(Data));
        ret = recv(recv_socket, (char *)&Data, sizeof(Data), 0);
        if (ret == SOCKET_ERROR)
        {
            continue;
        }
        if (Data.type == TYPE_FRIEND || Data.type == TYPE_GROUP)
        {
            printf("[Recv]: %s\n", Data.buf);
        }
        if (Data.type == TYPE_DOWNLOAD)
        {
            recvFile(recv_socket, Data);
        }
    }
    closesocket(recv_socket);
    _endthread();
}

// 接收文件
void recvFile(SOCKET s, DATA_PACKET Data)
{
    FILE *fp;
    char new_name[BUF_SIZE];
    int ret;
    if (!(fp = fopen("D:\\VScode\\Github\\ChatRoom\\FileClient\\tmp", "wb")))
    {
        printf("Open error");
    }
    fwrite(Data.buf, Data.len, 1, fp);
    while (true)
    {
        memset(&Data, 0, sizeof(Data));
        ret = recv(s, (char *)&Data, sizeof(Data), 0);
        if (ret == SOCKET_ERROR)
        {
            //printf("Please repeat %d\n", GetLastError());
            //ret = recv(s, (char*)&Data, sizeof(Data), 0);
            continue;
        }
        fwrite(Data.buf, Data.len, 1, fp);
        if (Data.fin == -1)
        {
            break;
        }
    }
    fclose(fp);
    memset(new_name, 0, BUF_SIZE);
    sprintf(new_name, "D:\\VScode\\Github\\ChatRoom\\FileClient\\%s", Data.file_name);
    rename("D:\\VScode\\Github\\ChatRoom\\FileClient\\tmp", new_name);
    printf("\nDownload finish\n");
}

// 发送文件
void sendFile(SOCKET s, DATA_PACKET Data)
{
    FILE *fp;
    int ret;
    int end;
    int count;
    char *p;
    char filename[BUF_SIZE] = {0};

    fp = fopen(Data.file_name, "rb");
    strcpy(filename, (p = strrchr(Data.file_name, '\\')) ? p + 1 : Data.file_name);
    fseek(fp, 0, SEEK_END);
    end = ftell(fp);
    fseek(fp, 0, 0);
    count = (end % BUF_SIZE) == 0 ? (end / BUF_SIZE) : (end / BUF_SIZE) + 1;

    while (count--)
    {
        memset(&Data, 0, sizeof(Data));
        fread(Data.buf, BUF_SIZE, 1, fp);
        if (end >= BUF_SIZE) // 还有很多信息
        {
            Data.fin = count;
            Data.len = BUF_SIZE;
        }
        else // 最后一点信息
        {
            Data.fin = count - 1;
            Data.len = end;
        }
        //printf("%d\n", Data.fin);
        strcpy(Data.file_name, filename);
        Data.type = TYPE_UPLOAD;
        ret = send(s, (char *)&Data, sizeof(Data), 0);
        while (ret == SOCKET_ERROR)
        {
            ret = send(s, (char *)&Data, sizeof(Data), 0);
            //printf("%d repeat\n", Data.fin);
        }
        end -= BUF_SIZE;
    }
    fclose(fp);
    printf("Upload finish\n");
}
