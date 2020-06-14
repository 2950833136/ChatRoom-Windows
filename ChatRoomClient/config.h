#pragma once
#ifndef _CONFIG_H_
#define _CONFIG_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NUM_BUF   8     // 最大配置数
#define COL_BUF   64    // 每行最大内存
#define KEY_BUF   16    // 键值最大内存
#define VALUE_BUF 48    // 配置信息最大内存

static const char *config_path = "D:\\VScode\\Github\\ChatRoom\\config.txt";    // 绝对路径

typedef struct config_information
{
    char key[KEY_BUF];
    char value[VALUE_BUF];
} CI;

void deleteSpace(char *buf);
int getEqual(char *buf);
void initCI(CI configInformation[], int n);
void getConfig(const char *configFilePath, CI configInformation[]);
char *getValue(const char *key, CI configInformation[]);

#endif