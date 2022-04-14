/*************************************************************************
	> File Name: dict.c
	> Author: Qiyue
	> Mail: xiliiu@163.com
	> Memo: somebody die, and somebody relife!
	> Created Time: Sun 27 Feb 2022 10:08:05 AM CST
 ************************************************************************/
#include "head.h"
#include "color.h"

#define LL_BLUE "\e[1;34m"


// 单链表实现电子词典
typedef struct Node {
    char *key;
    char *trans;
    struct Node *next;
} Node;

/* 
    加载词典, 返回单链表
*/
Node *loadDict(char *file) {
    FILE *fp;
    if ((fp = fopen(file, "r")) == NULL) {
        perror("fopen 13");
        return NULL;
    }
    // 建立缓冲区
    char buff[1024] = {0}; // 初始化避免脏数据
    Node *head = NULL;
    while (fgets(buff, sizeof(buff), fp) != NULL) {
        Node *node = (Node *)malloc(sizeof(Node)); // 申请空间

        node->key = (char *)calloc((strlen(buff) + 1), sizeof(char));
        strcpy(node->key, buff + 1);      // copy 的时候把 # 号忽略掉
        
        /* 这里，要注意字符串后面咯不到的字符 \0 和 \n  */
        node->key[strlen(node->key) - 1] = '\0';

        memset(buff, 0, sizeof(buff));    // 避免数据污染

        // 读入翻译,
        // 程序的健壮性.写程序的时候，明确什么时候可能会发生什么错误，
        if(fgets(buff, sizeof(buff), fp) == NULL) {
            printf("31-read translate err!\n");
            return NULL;
        }
        node->trans = (char *)calloc((strlen(buff) + 1), sizeof(char));
        strcpy(node->trans, buff + 6);
        node->trans[strlen(node->trans) - 1] = '\0';
        // 头插法建立链表
        node->next = head;
        head = node;
    }
    fclose(fp);    
    return head;
}

void test(Node *head) {
    while (1) {
        printf("key = %s, trans = %s\n", head->key, head->trans);
        head = head->next;
    }
}

Node *findNode(Node *head, const char *key) {
    DBG(GREEN"function finidNode~\n");
    Node *node = head;
    while (node && strcmp(node->key, key)) {
        node = node->next;
    }
    return node;
}

void printTrans(const char *trans) {
    // 针对 @ 字符，进行分割
    char *buff = (char *)calloc((strlen(trans) + 1), sizeof(char));
    strcpy(buff, trans);
    char *token = strtok(buff, "@");
    DBG(YELLOW" key = %s\n", token);
    while(token) {
        printf(LL_BLUE"%s\n"NONE, token);
        token = strtok(NULL, "@");
    }
    free(buff);
    printf("\n");
    return ;
}

void findKey(Node *head) {
    if(!head) {
        printf(RED"84-head is Null\n");
        return ;
    }
    char input[1024] = {0};
    while(1) {
        printf(L_RED"Please input a word or string:\n"NONE);
        scanf("%[^\n]", input);
        getchar();
        if(!strlen(input)) continue;
        printf("input = %s\n", input);

        char *token = "";
        token = strtok(input, " ");
        while(token) {
            DBG(YELLOW"<D> : 108 %s\n"NONE, token);
            DBG(YELLOW"<D> : 107 Before findNode 函数\n"NONE);
            Node *find = findNode(head, token);
            DBG(YELLOW"<D> : 109 After findNode 函数\n"NONE);

            if (find) {
                DBG(YELLOW"<D> :111 调试信息\n"NONE);
                //如果找到，不为空
                DBG(GREEN"<D> :112 %s:\n"NONE, find->key);
                printf(GREEN"\nphares : %s\n"NONE, token);
//                printTrans(find->trans);
                printf(L_BLUE"%s\n"NONE, find->trans);
                printf("************************************\n");
            } else {
                DBG(YELLOW"<D> : 116 调试信息\n"NONE);
                printf(L_RED"Not Found!\n\n"NONE);
                printf("************************************\n");
            }
            DBG(YELLOW" 125-token = %s\n", token);
            token = strtok(NULL, " ");   // strtok 全局变量影响
            DBG(YELLOW" 126-token = %s\n", token);
        }

        bzero(input, sizeof(input)); // 清楚上次的输入
    }
}
int main () {
    Node *node;
    char filename[10] = "dict.txt";
    char *str = "how are you";
    if ((node = loadDict(filename)) == NULL) {
        perror("50-loadDict");
        return 1;  // 这里写 exit 还是 return 比较好
    }
    findKey(node);

//    test(node);

    return 0;
}
