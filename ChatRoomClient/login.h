#pragma once
#ifndef _LOGIN_H_
#define _LOGIN_H_

#include "sql.h"

int login(MYSQL mysql, char *sql, char *username, char *password);
bool regist(MYSQL mysql, char *sql, char *username, char *password);

#endif