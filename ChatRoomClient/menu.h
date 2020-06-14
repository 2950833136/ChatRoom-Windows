#pragma once
#ifndef _MENU_H_
#define _MENU_H_

#include "sql.h"

#define BUF_SIZE 1024

void menuWelcome();
void menuUser();
void menuDisplay(MYSQL_RES *res);
void menuOperate();

#endif