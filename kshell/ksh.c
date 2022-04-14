/*************************************************************************
	> File Name: myshell.c
	> Author: Qiyue
	> Mail: xiliiu@163.com
	> Memo: somebody die, and somebody relife!
	> Created Time: Mon 28 Feb 2022 07:54:23 PM CST
 ************************************************************************/
#include "head.h"
#define MAXARGS 10
char username[50], hostname[50], workdir[100];
void run_cmd(char *);
void init_var();
void print_prom() {
    printf(RED"%s"NONE"@"L_BLUE"%s"YELLOW" %s"NONE" $ ", username, hostname, workdir);
    fflush(stdout); // 清空缓冲，将内容输出
}

void do_nothing(int signum) {
    printf("\n");
    print_prom();
}

int main(int argc, char *argv[]) {
    pid_t pid;
    char buff[1024] = {0};
    init_var();
    signal(SIGINT, do_nothing); // 进程接收到 SIGINT 信号的时候，do_nothing

    while(1) {
        print_prom();
        fgets(buff, sizeof(buff), stdin);
        buff[strlen(buff) - 1] = '\0';

        if (strcmp(buff, "exit") == 0) {
            printf(BLUE"Bye!\n");
            exit(0);
        }
        if (!strlen(buff)) continue;  // ctrl + c 的时候，不读入字符，不然会bug 


        /*
        * 管道 ls | grep a | grep b | grep o
        * */
        char *cmd[100] = {0};
        int pipe_cnt = 0, ind = 0;
        if (strstr(buff, "|")) pipe_cnt += 1;
        cmd[ind++] = strtok(buff, "|");
        while (cmd[ind++] = strtok(NULL, "|")) {  } 

        DBG(YELLOW"<D> : sit the cmd below :\n");
        for (int i = 0; i < ind - 1; i++) {
            DBG(YELLOW"cmd[%d] = %s,", i, cmd[i]);
        }

        if (pipe_cnt == 0) {
            DBG(YELLOW"56-no pipe"NONE"\n");
            if ((pid = fork()) < 0) {
                perror("fork");
                exit(1);
            }
            if (pid == 0) {
                run_cmd(buff);
            } else {
                wait(NULL);
            }   
        } else {
            DBG(BLUE"67-has pipe"NONE"\n");
            int cnt = ind - 1;
            int pipefd[100][2];//准备好进程间通信的管道
            DBG(YELLOW"<D>-70 : sub process cnt = %d"NONE"\n", cnt);

            int fd_maxNum = sysconf(_SC_OPEN_MAX);
            printf("the max number of fd this program that on run time is = %d\n"NONE, fd_maxNum);
            for (int i = 0; i < cnt; i++) {
                // 打开管道
                if ((pipe(pipefd[i])) < 0) {
                    perror("pipe");
                    exit(1);
                }

                if ((pid = fork()) < 0) {
                    perror("fork-89");
                    exit(1);
                }
                if (pid == 0) {
                    if (i == 0) {
                        dup2(pipefd[i][1], 1); // 第一个进程，输出到管道里面
                    } else if(i == cnt - 1) {
                        dup2(pipefd[i - 1][0], 0);  // 最后一个进程，读的是某个进程的标准输出
                    } else {
                        dup2(pipefd[i][1], 1);
                        dup2(pipefd[i - 1][0], 0); // 从管道的读端 读取数据
                    }
                    run_cmd(cmd[i]);
                    DBG("<D> 102: cmd = "RED"%s"NONE"\n", cmd[i]);
                    break; //子进程不能继续循环
                } 

            }
            // parent process
            for (int i = 0; i < cnt; i++) {
                wait(NULL);
                DBG(YELLOW"after wait!"NONE"\n");
            }
            
        }
    }

    return 0;
}


/*
* 去除前后多余空格, 在这里没用上
* */
char *my_trim (char * filename) {
    int lcnt = 0, rcnt = 0, ind = 0;
    while(filename[lcnt] == ' ') lcnt++; //lcnt移动到第一个非空格字符处
    rcnt = lcnt;
    while(filename[rcnt] != ' ') rcnt++; // rcnt 移动到文件名后第一个空格处
    char *newfile = (char *)calloc(sizeof(filename), sizeof(char));
    for(int i = lcnt; i < rcnt; i++, ind++) {
        newfile[ind] = filename[i];
    }
    return newfile;
}


void run_cmd(char *cmd) {
    // 最多接受 MAXARGS 个参数
//    printf("after strtok, cmd = %s\n", cmd);
    char *args[MAXARGS];    
    int ind = 0, std_fd = -1;

    /*
    * 重定向实现
    * ls a.c b.c >a d.c ----等于 ls a.c b.c d.c >a
    * */
    if (strstr(cmd, "<")) {
        std_fd = 0;
    } else if (strstr(cmd, ">")) {
        std_fd = 1;
    }
    char *_cmd;
    char *filename, *_file;
    if (std_fd == -1) {
        _cmd = cmd;
        filename = NULL;
    } else {
        _cmd = strtok(cmd, "><");
        char _file[256] = {0};
        filename = strtok(NULL, "><"); // 获得文件名, 但是注意，文件名前后避免有多余空格
        //filename = my_trim(filename);
        sscanf(filename, "%s", _file);
        DBG("<D>-162: cmd = "RED"%s"NONE", filename = "RED"%s"NONE"\n", _cmd, _file);
        int file_fd;

        if (std_fd == 0) {
            if ((file_fd = open(_file, O_RDONLY)) < 0) {
                perror("open-166");
                exit(1);
            }
        } else {
            if ((file_fd = open(_file, O_RDWR | O_CREAT | O_TRUNC, 0644)) < 0) {
                perror("open-171");
                exit(1);
            }
        }
        dup2(file_fd, std_fd);
        close(file_fd);

    }

    DBG("<D>-180 : cmd = "RED"%s"NONE", filename = "RED"%s"NONE"\n", _cmd, filename);
    args[ind++] = strtok(_cmd, " ");
    while (args[ind++] = strtok(NULL, " ")) {
        
    }
//    printf("after strtok, cmd = %s, args[0] = %s, args[1] = %s\n", cmd, args[0], args[1]);
    if(execvp(cmd, args) < 0) {
        fprintf(stderr, RED"msh: Command not found: "NONE"\033[5;100m%s"NONE"\n", cmd);
        exit(127);
    }
}


void init_var() {
    strcpy(username, "beichen");
    strcpy(hostname, "mei");
    strcpy(workdir, "kshell");
}
