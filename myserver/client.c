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
#define BACKLOG 5

#define handle_err(msg) {\
    perror(msg);\
    exit(1);\
}

int main() {

    int sockfd;
    char buf[MAXLEN] = {0};
    char rspbuf[MAXLEN] = {0};
    struct sockaddr_in servaddr, cliaddr;
    socklen_t servaddr_len, cliaddr_len = sizeof(struct sockaddr_in);

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) 
        handle_err("socket");

    bzero(&servaddr, sizeof(struct sockaddr_in));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(SERV_PORT);
    inet_pton(AF_INET, SERV_IP, &servaddr.sin_addr);

    if (connect(sockfd, (struct sockaddr *)&servaddr, sizeof(struct sockaddr_in)) == -1)
        handle_err("connect");
    int n = 0;

    while(n = read(0, buf, MAXLEN)) {

        write(sockfd, buf, MAXLEN);
        if (strncmp(buf, "quit", 4) == 0) {
            break;
        }
        bzero(&buf, sizeof(buf));
        n = read(sockfd, buf, MAXLEN);
    
        write(1, buf, n);
        bzero(&buf, sizeof(buf));
    }

    close(sockfd);
    return 0;
}
