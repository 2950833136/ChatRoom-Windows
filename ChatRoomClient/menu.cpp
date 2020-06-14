#define _CRT_SECURE_NO_WARNINGS

#include "menu.h"

// 开始菜单
void menuWelcome()
{
    system("cls");
    printf("********************************************************************\n");
    printf("*                          1----Login                              *\n");
    printf("*                          2----Regist                             *\n");
    printf("*                          ?----Flush                              *\n");
    printf("********************************************************************\n");
}

// 用户界面
void menuUser()
{
    system("cls");
    printf("********************************************************************\n");
    printf("*                          1----Friend                             *\n");
    printf("*                          2----Group                              *\n");
    printf("*                          0----Return                             *\n");
    printf("*                          ?----Flush                              *\n");
    printf("********************************************************************\n");
}

// 列出朋友或分组
void menuDisplay(MYSQL_RES *res)
{
    system("cls");
    char *buf = (char *)malloc(sizeof(char) * BUF_SIZE);
    MYSQL_ROW row;

    printf("********************************************************************\n");
    while (row = mysql_fetch_row(res))
    {
        memset(buf, 0, BUF_SIZE);
        sprintf(buf, "*                      %5d----%-10s                         *\n", atoi(row[0]), row[1]);
        printf(buf);
    }
    printf("*                          0----Return                             *\n");
    printf("*                          ?----Flush                              *\n");
    printf("********************************************************************\n");
}

// 操作界面
void menuOperate()
{
    system("cls");
    printf("********************************************************************\n");
    printf("*                          1----SendMessage                        *\n");
    printf("*                          2----UploadFile                         *\n");
    printf("*                          3----DownloadFile                       *\n");
    printf("*                          0----Return                             *\n");
    printf("*                          ?----Flush                              *\n");
    printf("********************************************************************\n");
}