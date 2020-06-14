#pragma once
#ifndef _SQL_H_
#define _SQL_H_

#include <mysql.h>
#pragma comment(lib, "libmysql.lib")
#include "config.h"

#define SQL_SIZE 1024   // sql 语句内存

bool mysqlConnect(MYSQL &mysql);
void show(MYSQL_RES *res);
MYSQL_RES *mysqlSelect(MYSQL mysql, char *sql);
int isExist(MYSQL mysql, char *sql);
bool mysqlInsert(MYSQL mysql, char *sql);
bool mysqlDelete(MYSQL mysql, char *sql);
bool mysqlUpdate(MYSQL mysql, char *sql);

#endif