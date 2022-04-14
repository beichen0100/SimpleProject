/*************************************************************************
	> File Name: head.h
	> Author: Qiyue
	> Mail: xiliiu@163.com
	> Memo: somebody die, and somebody relife!
	> Created Time: Fri 25 Feb 2022 07:39:28 PM CST
 ************************************************************************/
#ifndef _HEAD_H
#define _HEAD_H
#include <stdio.h>
#include <time.h>
#include <pwd.h>
#include <grp.h>
#include <ctype.h>
#include <stdlib.h>
#include <unistd.h>
#include <error.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <dirent.h>
#include <sys/ioctl.h>
#include <math.h>
#include <errno.h>
#include <pthread.h>
#include <time.h>
#include <sys/sem.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/mman.h>

#include "color.h"
#include "my_popen.h"
#include "common.h"

char ans[512];
#ifdef _D
#define DBG(fmt, arg...) \
    printf(L_BLUE"[%s : %d] -  ", __func__, __LINE__);\
    printf(fmt, ##arg)
#else
#define DBG(fmt, arg...)
#endif

#endif

