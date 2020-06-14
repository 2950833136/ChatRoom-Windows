#define _CRT_SECURE_NO_WARNINGS

#include "config.h"

// 根据 key 得到 value
char* getValue(const char* key, CI configInformation[])
{
    for (int i = 0; i < NUM_BUF; i++)
    {
        if (0 == strcmp(configInformation[i].key, key))
        {
            return configInformation[i].value;
        }
    }
    return NULL;
}

// 得到对应关系
void getConfig(const char* configFilePath, CI configInformation[])
{
    int i = 0;
    FILE* fp = NULL;
    fp = fopen(configFilePath, "r");

    char* buf = (char*)malloc(sizeof(char) * COL_BUF);
    memset(buf, 0, COL_BUF);

    while (fgets(buf, COL_BUF, fp) != NULL)
    {
        if (buf[0] == '#' || buf[0] == '\n' || buf[0] == '\r')
        {
            continue;
        }
        else
        {
            unsigned int m = strlen(buf) - 1;
            buf[m] = '\0';
            deleteSpace(buf);
            unsigned int n = getEqual(buf);
            strncpy(configInformation[i].key, buf, n);
            strncpy(configInformation[i].value, buf + n + 1, m - n);
            // printf("%s\n", configInformation[i].key);
            // printf("%s\n", configInformation[i].value);
            i++;
        }
        memset(buf, 0, COL_BUF);
    }
    fclose(fp);
}

// 初始化
void initCI(CI configInformation[], int n)
{
    for (int i = 0; i < n; i++)
    {
        memset(configInformation[i].key, 0, KEY_BUF);
        memset(configInformation[i].value, 0, VALUE_BUF);
    }
}

// 找到等号
int getEqual(char* buf)
{
    int n = 0;
    while (*buf != '\0')
    {
        if (*buf == '=')
        {
            return n;
        }
        n++;
        buf++;
    }
    return -1;
}

// 去除字符空格
void deleteSpace(char* buf)
{
    int i = 0, j = 0;
    int len = strlen(buf);
    if (0 == len)
    {
        return;
    }
    for (i = 0; i < len; i++)
    {
        if (buf[i] != ' ')
        {
            buf[j++] = buf[i];
        }
    }
    buf[j] = '\0';
}