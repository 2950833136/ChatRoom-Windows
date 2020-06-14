#define _CRT_SECURE_NO_WARNINGS

#include "internet.h"

// 转发消息
void transmit(SOCKET s, DATA_PACKET Data)
{
    int ret;
    ret = send(s, (char *)&Data, sizeof(Data), 0);
}

// 发送文件
void sendFile(SOCKET s, DATA_PACKET Data)
{
    FILE *fp;
    int ret;
    int end;
    int count;
    char filename[NAME_SIZE] = {0};
    char filepath[BUF_SIZE] = {0};

    sprintf(filepath, "D:\\VScode\\Github\\ChatRoom\\FileServer\\%s", Data.file_name);
    strcpy(filename, Data.file_name);
    fp = fopen(filepath, "rb");
    fseek(fp, 0, SEEK_END);
    end = ftell(fp);
    fseek(fp, 0, 0);
    count = (end % BUF_SIZE) == 0 ? (end / BUF_SIZE) : (end / BUF_SIZE) + 1;

    while (count--)
    {
        memset(&Data, 0, sizeof(Data));
        fread(Data.buf, BUF_SIZE, 1, fp);
        if (end >= BUF_SIZE) //剩余很多信息
        {
            Data.fin = count;
            Data.len = BUF_SIZE;
        }
        else
        {
            Data.fin = count - 1;
            Data.len = end;
        }
        //printf("%d\n", Data.fin);
        strcpy(Data.file_name, filename);
        Data.type = TYPE_DOWNLOAD;
        ret = send(s, (char *)&Data, sizeof(Data), 0);
        while (ret == SOCKET_ERROR)
        {
            ret = send(s, (char *)&Data, sizeof(Data), 0);
            printf("%d repeat %d\n", Data.fin, GetLastError());
        }
        end -= BUF_SIZE;
    }
    fclose(fp);
}

// 接收文件
void recvFile(SOCKET s, DATA_PACKET Data)
{
    FILE *fp;
    char new_name[BUF_SIZE];
    int ret;
    if (!(fp = fopen("D:\\VScode\\Github\\ChatRoom\\FileServer\\tmp", "wb")))
    {
        printf("Open error");
    }
    //printf("%d\n", Data.fin);
    fwrite(Data.buf, Data.len, 1, fp);
    while (true)
    {
        memset(&Data, 0, sizeof(Data));
        ret = recv(s, (char *)&Data, sizeof(Data), 0);
        //printf("%d\n", Data.fin);
        if (ret == SOCKET_ERROR)
        {
            //printf("recv() failed!\n");
            continue;
        }
        fwrite(Data.buf, Data.len, 1, fp);
        if (Data.fin == -1)
        {
            break;
        }
    }
    fclose(fp);
    printf("Recv finish!\n");
    memset(new_name, 0, BUF_SIZE);
    sprintf(new_name, "D:\\VScode\\Github\\ChatRoom\\FileServer\\%s", Data.file_name);
    rename("D:\\VScode\\Github\\ChatRoom\\FileServer\\tmp", new_name);
}