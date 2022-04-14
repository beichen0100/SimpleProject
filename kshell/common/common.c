/*************************************************************************
	> File Name: common.c
	> Author: Qiyue
	> Mail: xiliiu@163.com
	> Memo: somebody die, and somebody relife!
	> Created Time: Sun 27 Feb 2022 08:52:54 PM CST
 ************************************************************************/

#include "head.h"

int make_nonblock(int fd) {
    int flag;
    if((flag = fcntl(fd, F_GETFL)) == -1) {
        perror("fcntl");
        return -1;
    }
    flag |= O_NONBLOCK;
    return fcntl(fd, F_SETFL, flag);
}


int make_block(int fd) {
    int flag;
    if((flag = fcntl(fd, F_GETFL)) == -1) {
        perror("fcntl");
        return -1;
    }
    flag &= ~O_NONBLOCK;
    return fcntl(fd, F_SETFL, flag);
}
