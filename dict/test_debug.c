/*************************************************************************
	> File Name: test_debug.c
	> Author: Qiyue
	> Mail: xiliiu@163.com
	> Memo: somebody die, and somebody relife!
	> Created Time: Sun 27 Feb 2022 11:41:24 AM CST
 ************************************************************************/

#include<stdio.h>

int main () {

    int n = 10;
    while(n--) {
        printf("hello world\n");
        #ifdef _Debug
        printf("kdkdkdk");
        #endif
    }

    return 0;
}
