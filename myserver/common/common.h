/*************************************************************************
	> File Name: common.h
	> Author: Qiyue
	> Mail: xiliiu@163.com
	> Memo: somebody die, and somebody relife!
	> Created Time: Thu 24 Mar 2022 02:22:51 PM CST
 ************************************************************************/
#ifndef _COMMON_H
#define _COMMON_H

int make_nonblock(int fd);
int make_block(int fd);
int socket_create(int port);
int socket_connect(const char *ip, int port);
int socket_connect_timeout(const char *ip, int port, long timeout);

#endif

