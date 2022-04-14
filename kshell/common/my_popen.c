/*************************************************************************
	> File Name: my_popen.c
	> Author: Qiyue
	> Mail: xiliiu@163.com
	> Memo: somebody die, and somebody relife!
	> Created Time: Mon 28 Feb 2022 03:37:45 PM CST
 ************************************************************************/

#include "head.h"

static pid_t *childpid = NULL;
static int maxsize;
// 考虑到 popen 会被多次调用，所以 childpid 设置成全局变量

FILE *my_popen(const char* cmd, const char *type) {
    int fd[2];
    FILE *fp = NULL;
    pid_t pid;

    if (type[0] != 'r' && type[0] != 'w') {
        errno = EINVAL;
        return NULL;
    } 

    if (pipe(fd) < 0) {
        perror("pipe");
        return NULL;
    }

    if (childpid == NULL) {
        maxsize = sysconf(_SC_OPEN_MAX);
        if ((childpid = (pid_t *)calloc(maxsize, sizeof(pid_t))) == NULL) {
            return NULL;
        }
    }

    if ((pid = fork()) < 0) {
        perror("fork");
        exit(1);
    }

    /*
    * r -- 子进程关闭读端，父进程关闭写端
    * w -- 子进程关闭写端， 父进程关闭读端
    *
    * */
    if (pid == 0) {
        if (type[0] == 'r') {
            // 子进程把东西写到 popen 打开的文件流中
            // 向这个流的写入，会被当做是 cmd 的标准输入
            close(fd[0]);
            if (fd[1] != STDOUT_FILENO) {
                dup2(fd[1], STDOUT_FILENO);
                close(fd[1]);
            }
        } else {
            close(fd[1]);
            if (fd[0] != STDIN_FILENO) {
                dup2(fd[0], STDIN_FILENO);
                close(fd[0]);
            }
        }
        execl("/bin/sh", "sh", "-c", cmd, NULL);
        exit(1);
    }

    if (type[0] == 'r') {
        close(fd[1]);
        if ((fp = fdopen(fd[0], type)) == NULL) {
            return NULL;
        }
    } else {
        close(fd[0]);
        if ((fp = fdopen(fd[1], type)) == NULL) {
            return NULL;
        }

    }
    childpid[fileno(fp)] = pid;
    return fp;
}

int my_pclose(FILE *fp) {
    // pid  --> fp , 我们可以通过 fp 拿到 对应的 fd

    int status, fd;
    pid_t pid;
    fd = fileno(fp);
    pid = childpid[fd];
    if (pid == 0) {
        errno = EINVAL;
        return -1;
    }
    
    fflush(fp);
    close(fd);
    wait4(pid, &status, 0, NULL);

    // 该函数，返回一个子进程的状态
    return status;
}
