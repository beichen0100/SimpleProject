/*************************************************************************
	> File Name: task_queue.h
	> Author: Qiyue
	> Mail: xiliiu@163.com
	> Memo: somebody die, and somebody relife!
	> Created Time: Wed 23 Mar 2022 08:52:00 PM CST
 ************************************************************************/
#ifndef _TASK_QUEUE_H
#define _TASK_QUEUE_H

typedef struct Task{
    int fd;
    struct Task *next;
} Task;

typedef struct Task_queue {
    struct Task *head, *tail;
    pthread_mutex_t mutex;
    pthread_cond_t cond;
} Task_queue;

Task_queue *task_queue_init();
void task_queue_push(Task_queue *tq, int fd);
Task task_queue_pop(Task_queue *tq);
void task_queue_free(Task_queue *tq);

#endif

