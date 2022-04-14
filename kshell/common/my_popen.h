/*************************************************************************
	> File Name: my_popen.h
	> Author: Qiyue
	> Mail: xiliiu@163.com
	> Memo: somebody die, and somebody relife!
	> Created Time: Mon 28 Feb 2022 03:34:17 PM CST
 ************************************************************************/
#ifndef _MY_POPEN_H
#define _MY_POPEN_H
FILE *my_popen(const  char *cmd, const char *type);
int my_pclose(FILE *fp);
#endif

