/*************************************************************************
	> File Name: task_queue.c
	> Author: Qiyue
	> Mail: xiliiu@163.com
	> Memo: somebody die, and somebody relife!
	> Created Time: Wed 23 Mar 2022 09:10:49 PM CST
 ************************************************************************/

#include "head.h"

// 任务队列初始化
Task_queue* task_queue_init() {
    Task_queue *tq = (Task_queue *)malloc(sizeof(Task_queue));
    tq->head = NULL;
    tq->tail = NULL;
    pthread_mutex_init(&tq->mutex, NULL);
    pthread_cond_init(&tq->cond, NULL);
    return tq;
}

// 任务队列加入任务
void task_queue_push(Task_queue *tq, int fd) {
    pthread_mutex_lock(&tq->mutex); // 试图获得锁
    DBG("task_queue_push_start\n");
    Task *t = (Task *)malloc(sizeof(Task));
    t->fd = fd;
    t->next = NULL;

    if (tq->head == NULL) {
        tq->head = t;
        tq->tail = t;
    } else {
        tq->tail->next = t;
        tq->tail = t;
    }

    // 广播通知，有任务来了
    pthread_cond_broadcast(&tq->cond);
    pthread_mutex_unlock(&tq->mutex);
    return ;
}

// 任务出队
Task task_queue_pop(Task_queue *tq) {
    pthread_mutex_lock(&tq->mutex);

    while (!tq->head) {
        DBG("waiting for coming of task!\n");
        pthread_cond_wait(&tq->cond, &tq->mutex);
    }

    Task tmp_task, *th;
    th = tq->head;
    tmp_task = *th;
    tq->head = tq->head->next;

    if (!tq->head) 
        tq->tail = NULL;

    free(th);
    pthread_mutex_unlock(&tq->mutex);
    return tmp_task;
}


// 任务队列清空函数
void task_queue_free(Task_queue *tq) {
    pthread_mutex_lock(&tq->mutex);
    if (!tq->head) 
        return ;
    
    Task *k, *q;
    k = tq->head;
    // 循环free掉整个任务队列
    while(k) {
        q = k;
        k = k->next;
        free(q);
    }

    free(tq);
    pthread_mutex_unlock(&tq->mutex);
    pthread_mutex_destroy(&tq->mutex);
    pthread_cond_destroy(&tq->cond);
    return ;
}




