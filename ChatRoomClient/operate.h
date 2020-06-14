#pragma once
#ifndef _OPERATE_H_
#define _OPERATE_H_

#include "sql.h"
#include "menu.h"
#include "login.h"
#include "internet.h"

#define STR_SIZE 128    // 用户名、密码内存

void welcome(MYSQL mysql, char *sql);
void userInterface(MYSQL mysql, char *sql, DATA_PACKET data);
void userFriend(MYSQL mysql, char *sql, DATA_PACKET data);
void userGroup(MYSQL mysql, char *sql, DATA_PACKET data);
void userOperate(MYSQL mysql, char *sql, DATA_PACKET data);
void groupOperate(MYSQL mysql, char *sql, DATA_PACKET data);

#endif
