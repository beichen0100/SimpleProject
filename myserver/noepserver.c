/*************************************************************************
	> File Name: server.c
	> Author: Qiyue
	> Mail: xiliiu@163.com
	> Memo: somebody die, and somebody relife!
	> Created Time: Wed 23 Mar 2022 04:15:07 PM CST
 ************************************************************************/

#include "head.h"

#define MAXLEN 80
#define SERV_PORT 8000
#define SERV_IP "127.0.0.1"
#define BACKLOG 2
#define MAX_EVENTS 256
#define NUM_OF_THREAD 2

#define handle_err(msg) {\
    printf("[%s : %d] ", __func__, __LINE__);\
    perror(msg);\
    exit(1);\
}
char uptolow_lowtoup(char ch);
char transfor(char ch) {
    if (ch >= 'a' && ch <= 'z') {
        ch &= '_';
    } else if(ch >= 'A' && ch <= 'Z'){
        ch |= ' ';
    }
    return ch;
}

// 线程处理函数, 将 fd 传过来
void *doupper(void *arg) {
    pthread_detach(pthread_self());
    
    Task_queue *tq = (Task_queue *)arg;
    char buf[MAXLEN] = {0};

    while(1) {
        Task tt = task_queue_pop(tq);
        printf("get task id = %d\n", tt.fd);
        int connfd = tt.fd, n;
        while(1) {

            bzero(&buf, strlen(buf));
            n = read(connfd, buf, MAXLEN);
            if (n == -1) 
                handle_err("read");
            
            write(1, buf, n);  // 输出到服务器的标准输出
            if (strncmp(buf, "quit", 4) == 0) {
                // 如果是 quit，那么就关闭 tcp 连接
                printf("task id = %d overed\n", connfd);
                close(connfd);
                break;
            }
            // 小写转大写，大写转小写 
            for(int i = 0; i < n; i++) 
                buf[i] = transfor(buf[i]);

            write(connfd, buf, n);
            printf("finish task id = %d\n\n", connfd);
        }
    }
    return (void *)0;
}


int main() {

    int listenfd, connfd;
    char buf[MAXLEN] = {0}, str[INET_ADDRSTRLEN] = {0};
    struct sockaddr_in servaddr, cliaddr;
    socklen_t servaddr_len;
    
    Task_queue *tq = task_queue_init();
    pthread_t tid;
    for (int i = 0; i < NUM_OF_THREAD; i++) {
        pthread_create(&tid, NULL, doupper, (void *)tq);
        printf("create a thread id = %#lx\n", tid);
    }

    listenfd = socket(AF_INET, SOCK_STREAM, 0);
    if (listenfd == -1) 
        handle_err("socket");
    
    bzero(&servaddr, sizeof(struct sockaddr_in));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(SERV_PORT);
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);  //这个 INADDR_ANY 是 0

    if(bind(listenfd,(struct sockaddr *)&servaddr, sizeof(struct sockaddr_in)) == -1)
        handle_err("bind");

    //绑定了之后，开始监听 listen()
    listen(listenfd, BACKLOG);
    printf("tcp server is ready!\n");

    int n = 0;
    while(1) {
        socklen_t cliaddr_len = sizeof(cliaddr);
        connfd = accept(listenfd, (struct sockaddr *)&cliaddr, &cliaddr_len);
        if (connfd == -1) 
            handle_err("accept");

        printf("received from %s : %d\n", 
               inet_ntop(AF_INET, &cliaddr.sin_addr, str, sizeof(str)), ntohs(cliaddr.sin_port));
        
        printf("push fd = %d in task_queue\n", connfd);
        task_queue_push(tq, connfd);
    }
    task_queue_free(tq);
    close(listenfd);
    return 0;
}
