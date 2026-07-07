#ifndef FENCERS2_SERVER_THREADS_H
#define FENCERS2_SERVER_THREADS_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <pthread.h>

typedef void (*Task)(void*);

typedef struct TaskQueue {
    Task* tasks;
    size_t cap;
    size_t head;
    size_t tail;
    size_t count;
} TaskQueue;

TaskQueue TaskQueue_create(const size_t cap);

void TaskQueue_destroy(TaskQueue* queue);

bool TaskQueue_push(TaskQueue* queue, const Task task);

Task TaskQueue_pop(TaskQueue* queue);

typedef struct ThreadPool {
    pthread_mutex_t queue_lock;
    pthread_cond_t not_empty;
    pthread_cond_t not_full;
    TaskQueue queue;
    pthread_mutex_t running_lock;
    bool running;
    size_t num_workers;
    pthread_t* threads;
} ThreadPool;

ThreadPool* ThreadPool_start(const size_t workers, const size_t queue_cap);

void ThreadPool_stop(ThreadPool* pool);

void ThreadPool_push(ThreadPool* pool, Task task);

#endif