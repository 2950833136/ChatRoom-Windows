#define _CRT_SECURE_NO_WARNINGS

#include <winsock2.h>
#include <stdio.h>
#include <process.h>
#pragma comment(lib, "ws2_32.lib")
#include <time.h>
#include "operate.h"

int main()
{
    MYSQL mysql;              // mysql 定义
    char sql[SQL_SIZE];       // sql 语句
    memset(sql, 0, SQL_SIZE); // sql 初始化
    mysql_init(&mysql);       // mysql 初始化
    mysqlConnect(mysql);      // mysql 连接 
    welcome(mysql, sql);      // 开始
    mysql_close(&mysql);      // mysql 关闭

    return 0;
}