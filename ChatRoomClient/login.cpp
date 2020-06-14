#define _CRT_SECURE_NO_WARNINGS

#include "login.h"

// 登录，返回 U_ID
int login(MYSQL mysql, char *sql, char *username, char *password)
{
    sprintf(sql, "SELECT user_id FROM t_user WHERE username = \"%s\" AND password = \"%s\" LIMIT 1;", username, password);
    //printf("%s\n", sql);
    int id = 0;
    if (id = isExist(mysql, sql))
    {
        printf("Login success\n");
        return id;
    }
    else
    {
        printf("Login fail\n");
        return -1;
    }
}

// 注册
bool regist(MYSQL mysql, char *sql, char *username, char *password)
{
    sprintf(sql, "SELECT username FROM t_user WHERE username = \"%s\" LIMIT 1;", username);
    if (isExist(mysql, sql))
    {
        printf("The username is exist\n");
        return false;
    }
    sprintf(sql, "INSERT INTO t_user(username, password) VALUES(\"%s\", \"%s\");", username, password);
    if (mysqlInsert(mysql, sql))
    {
        printf("Regist success\n");
        return true;
    }
    else
    {
        printf("Regist fail\n");
        return false;
    }
}