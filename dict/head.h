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
#include "color.h"
#include <math.h>

#ifdef _D
#define DBG(fmt, arg...) printf(fmt, ##arg)
#else
#define DBG(fmt, arg...)
#endif

#endif

