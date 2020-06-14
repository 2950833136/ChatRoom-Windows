#define _CRT_SECURE_NO_WARNINGS

#include "sql.h"

// 连接 MySQL
bool mysqlConnect(MYSQL &mysql)
{
    CI configInformation[NUM_BUF];
    initCI(configInformation, NUM_BUF);
    getConfig(config_path, configInformation);
    char *host = getValue("host", configInformation);
    char *port = getValue("port", configInformation);
    char *user = getValue("user", configInformation);
    char *password = getValue("password", configInformation);
    char *database = getValue("database", configInformation);
    if (!(mysql_real_connect(&mysql, host, user, password, database, atoi(port), NULL, 0)))
    {
        printf("Fail----%s\n", mysql_error(&mysql));
        return false;
    }
    else
    {
        return true;
    }
}

// 返回查询记录
MYSQL_RES *mysqlSelect(MYSQL mysql, char *sql)
{
    MYSQL_RES *res; // 一个结果集结构体
    // 查询，成功则返回0
    if (mysql_real_query(&mysql, sql, (unsigned int)strlen(sql)))
    {
        printf("Query failed!\n");
        return NULL;
    }
    else
    {
        // mysql_store_result 将全部的查询结果读取到客户端
        res = mysql_store_result(&mysql);
        return res;
    }
}

// 显示查询记录（没用到）
void show(MYSQL_RES *res)
{
    MYSQL_ROW row; // char** 二维数组，存放一条条记录
    // mysql_fetch_row 检索结果集的下一行
    while (row = mysql_fetch_row(res))
    {
        // mysql_num_fields 返回结果集中的字段数目
        for (unsigned int n = 0; n < mysql_num_fields(res); n++)
        {
            printf("%10s", row[n]);
        }
        printf("\n");
    }
}

// 查询是否存在
int isExist(MYSQL mysql, char *sql)
{
    MYSQL_RES *res; // 一个结果集结构体
    MYSQL_ROW row;  // char** 二维数组，存放一条条记录
    if (mysql_real_query(&mysql, sql, (unsigned int)strlen(sql)))
    {
        printf("Query failed!\n");
        return false;
    }
    else
    {
        res = mysql_store_result(&mysql);
        int num = (int)mysql_num_rows(res);
        if (num == 0)
        {
            return num;
        }
        row = mysql_fetch_row(res);
        return atoi(row[0]);
    }
}

// 添加
bool mysqlInsert(MYSQL mysql, char *sql)
{
    if (mysql_real_query(&mysql, sql, (unsigned int)strlen(sql)))
    {
        printf("Insert failed!\n");
        return false;
    }
    else
    {
        return true;
    }
}

// 删除
bool mysqlDelete(MYSQL mysql, char *sql)
{
    if (mysql_real_query(&mysql, sql, (unsigned int)strlen(sql)))
    {
        printf("Delete failed!\n");
        return false;
    }
    else
    {
        return true;
    }
}

// 更新
bool mysqlUpdate(MYSQL mysql, char *sql)
{
    if (mysql_real_query(&mysql, sql, (unsigned int)strlen(sql)))
    {
        printf("Update failed!\n");
        return false;
    }
    else
    {
        return true;
    }
}