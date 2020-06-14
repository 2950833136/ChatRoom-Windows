#define _CRT_SECURE_NO_WARNINGS

#include <winsock2.h>
#include <stdio.h>
#include <process.h>
#pragma comment(lib, "ws2_32.lib")
#include "operate.h"
#include "get_socket.h"

int U_ID = 0;   // 自己id

// 开始
void welcome(MYSQL mysql, char *sql)
{
    DATA_PACKET data;
    memset(&data, 0, sizeof(data));
    menuWelcome();
    int key = 0;
    printf("Please input key: ");
    scanf("%d", &key);
    char *username = (char *)malloc(sizeof(char) * STR_SIZE);
    char *password = (char *)malloc(sizeof(char) * STR_SIZE);
    memset(username, 0, STR_SIZE);
    memset(password, 0, STR_SIZE);
    switch (key)
    {
    case 1:
        printf("Please input username: ");
        scanf("%s", username);
        printf("Please input password: ");
        scanf("%s", password);
        if (-1 != (U_ID = login(mysql, sql, username, password)))
        {
            //printf("%d\n", U_ID);
            sprintf(sql, "UPDATE t_user a SET a.state = 1 WHERE a.user_id = %d;", U_ID);
            mysqlUpdate(mysql, sql);
            data.u_id = U_ID;
            client_socket = get_socket();
            sendInfo(client_socket, U_ID);
            _beginthread(recvThread, 0, &client_socket);
            userInterface(mysql, sql, data);
        }
        else
        {
            Sleep(3000);
            welcome(mysql, sql);
        }
        break;
    case 2:
        printf("Please input username: ");
        scanf("%s", username);
        printf("Please input password: ");
        scanf("%s", password);
        regist(mysql, sql, username, password);
        Sleep(3000);
        welcome(mysql, sql);
        break;
    default:
        welcome(mysql, sql);
        break;
    }
}

// 用户
void userInterface(MYSQL mysql, char *sql, DATA_PACKET data)
{
    menuUser();
    int key = 0;
    printf("Please input key: ");
    scanf("%d", &key);
    getchar();
    switch (key)
    {
    case 1:
        userFriend(mysql, sql, data);
        break;
    case 2:
        userGroup(mysql, sql, data);
        break;
    case 0:
        welcome(mysql, sql);
        break;
    default:
        userInterface(mysql, sql, data);
        break;
    }
}

// 用户朋友
void userFriend(MYSQL mysql, char *sql, DATA_PACKET data)
{
    MYSQL_RES *res;
    sprintf(sql, "SELECT a.user_id, a.username FROM t_user a WHERE a.user_id IN( SELECT b.friend_id FROM t_user a INNER JOIN t_friend b ON a.user_id = b.user_id WHERE a.user_id = %d) AND a.state = 1;", U_ID);
    res = mysqlSelect(mysql, sql);
    menuDisplay(res);
    int id = 0;
    printf("Please input id: ");
    scanf("%d", &id);
    getchar();
    if (id > 0)
    {
        data.type = TYPE_FRIEND;
        data.to_id = id;
        userOperate(mysql, sql, data);
    }
    else if (id == 0)
    {
        userInterface(mysql, sql, data);
    }
    else
    {
        userFriend(mysql, sql, data);
    }
}

// 用户分组
void userGroup(MYSQL mysql, char *sql, DATA_PACKET data)
{
    MYSQL_RES *res;
    sprintf(sql, "SELECT c.group_id, c.group_name FROM t_group c WHERE c.group_id IN( SELECT b.group_id FROM t_user a INNER JOIN t_user_group b ON a.user_id = b.user_id WHERE a.user_id = %d);", U_ID);
    res = mysqlSelect(mysql, sql);
    menuDisplay(res);
    int id = 0;
    printf("Please input id: ");
    scanf("%d", &id);
    getchar();
    if (id > 0)
    {
        data.type = TYPE_GROUP;
        data.to_id = id;
        groupOperate(mysql, sql, data);
    }
    else if (id == 0)
    {
        userInterface(mysql, sql, data);
    }
    else
    {
        userGroup(mysql, sql, data);
    }
}

// 用户对朋友操作
void userOperate(MYSQL mysql, char *sql, DATA_PACKET data)
{
    DATA_PACKET Data;
    menuOperate();
    int key = 0;
    while (true)
    {
        printf("Please input key: ");
        scanf("%d", &key);
        getchar();
        switch (key)
        {
        case 1:
            sendMessage(client_socket, data);
            break;
        case 2:
            printf("Please input filepath: ");
            fgets(data.file_name, NAME_SIZE, stdin);
            data.file_name[strlen(data.file_name) - 1] = '\0';
            sendFile(client_socket, data);
            break;
        case 3:
            memset(&Data, 0, sizeof(Data));
            printf("Please input filename: ");
            fgets(Data.file_name, NAME_SIZE, stdin);
            Data.file_name[strlen(Data.file_name) - 1] = '\0';
            Data.type = TYPE_DOWNLOAD;
            send(client_socket, (char *)&Data, sizeof(Data), 0);
            break;
        case 0:
            userFriend(mysql, sql, data);
            break;
        default:
            userOperate(mysql, sql, data);
            break;
        }
    }
}

// 用户对分组操作
void groupOperate(MYSQL mysql, char *sql, DATA_PACKET data)
{
    DATA_PACKET Data;
    menuOperate();
    int key = 0;
    while (true)
    {
        printf("Please input key: ");
        scanf("%d", &key);
        getchar();
        switch (key)
        {
        case 1:
            sendMessage(client_socket, data);
            break;
        case 2:
            printf("Please input filepath: ");
            fgets(data.file_name, NAME_SIZE, stdin);
            data.file_name[strlen(data.file_name) - 1] = '\0';
            sendFile(client_socket, data);
            break;
        case 3:
            memset(&Data, 0, sizeof(Data));
            printf("Please input filename: ");
            fgets(Data.file_name, NAME_SIZE, stdin);
            Data.file_name[strlen(Data.file_name) - 1] = '\0';
            Data.type = TYPE_DOWNLOAD;
            send(client_socket, (char *)&Data, sizeof(Data), 0);
            break;
        case 0:
            userGroup(mysql, sql, data);
            break;
        default:
            groupOperate(mysql, sql, data);
            break;
        }
    }
}