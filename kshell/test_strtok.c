/*************************************************************************
	> File Name: test_strtok.c
	> Author: Qiyue
	> Mail: xiliiu@163.com
	> Memo: somebody die, and somebody relife!
	> Created Time: Tue 01 Mar 2022 11:39:14 AM CST
 ************************************************************************/

#include "head.h"


int main () {
    char buff[50];
    char *token = NULL;
    int cnt = 0;
    scanf("%[^\n]s", buff);
    
    token = strtok(buff, " ");
    putchar(10);
    while(token) {
        printf(YELLOW"<D> %d - token = %s\n", cnt++, token);
        token = strtok(NULL, " ");
    }
    putchar(10);

    printf(YELLOW"finally buff = %s\n", buff);  // buff中，最后只留下了 第一个非空字符串
    return 0;
}
