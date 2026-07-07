#include "../include/threads.h"

TaskQueue TaskQueue_create(const size_t cap) {
    TaskQueue queue = { .cap = cap };
    queue.tasks = (Task*)malloc(sizeof(Task) * cap);

    return queue;
}

void TaskQueue_destroy(TaskQueue* queue) {
    if (!queue) return;

    if (queue->tasks) {
        free(queue->tasks);
        queue->tasks = NULL;
        queue->cap = 0;
        queue->head = 0;
        queue->tail = 0;
        queue->count = 0;
    }
}

bool TaskQueue_push(TaskQueue* queue, const Task task) {
    if (!queue || queue->count == queue->cap) {
        return false;
    }

    queue->tasks[queue->head] = task;
    queue->head = (queue->head + 1) % queue->cap;
    queue->count++;

    return true;
}

Task TaskQueue_pop(TaskQueue* queue) {
    if (!queue || queue->count == 0) {
        return NULL;
    }

    Task task = queue->tasks[queue->tail];
    queue->tail = (queue->tail + 1) % queue->cap;
    queue->count--;

    return task;
}

static void* worker(void* arg) {
    ThreadPool* pool = (ThreadPool*)arg;

    do {
        pthread_mutex_lock(&pool->running_lock);
        if (!pool->running) break;
        pthread_mutex_unlock(&pool->running_lock);

        pthread_mutex_lock(&pool->queue_lock);
        while (pool->queue.count == 0) {
            pthread_cond_wait(&pool->not_empty, &pool->queue_lock);
        }
        Task task = TaskQueue_pop(&pool->queue);
        pthread_cond_signal(&pool->not_full);
        pthread_mutex_unlock(&pool->queue_lock);

        if (task) task(NULL);
    } while (true);

    return NULL;
}

ThreadPool* ThreadPool_start(const size_t workers, const size_t queue_cap) {
    ThreadPool* pool = (ThreadPool*)malloc(sizeof(ThreadPool));
    pool->num_workers = workers;
    pool->queue = TaskQueue_create(queue_cap);
    pool->running = true;

    pthread_mutex_init(&pool->queue_lock, NULL);
    pthread_mutex_init(&pool->running_lock, NULL);

    pthread_cond_init(&pool->not_empty, NULL);
    pthread_cond_init(&pool->not_full, NULL);

    pool->threads = (pthread_t*)malloc(sizeof(pthread_t) * workers);
    for (size_t i = 0; i < workers; i++) {
        pthread_create(&pool->threads[i], NULL, worker, pool);
    }

    return pool;
}

void ThreadPool_stop(ThreadPool* pool) {
    if (!pool) return;

    pthread_mutex_lock(&pool->running_lock);
    pool->running = false;
    pthread_mutex_unlock(&pool->running_lock);

    pthread_mutex_lock(&pool->queue_lock);
    pthread_cond_broadcast(&pool->not_empty);
    pthread_mutex_unlock(&pool->queue_lock);

    for (size_t i = 0; i < pool->num_workers; i++) {
        pthread_join(pool->threads[i], NULL);
    }
    free(pool->threads);

    TaskQueue_destroy(&pool->queue);

    pthread_mutex_destroy(&pool->running_lock);
    pthread_mutex_destroy(&pool->queue_lock);

    pthread_cond_destroy(&pool->not_empty);
    pthread_cond_destroy(&pool->not_full);

    free(pool);
}

void ThreadPool_push(ThreadPool* pool, Task task) {
    if (!pool) return;

    pthread_mutex_lock(&pool->queue_lock);
    while (pool->queue.count == pool->queue.cap) {
        pthread_cond_wait(&pool->not_full, &pool->queue_lock);
    }
    TaskQueue_push(&pool->queue, task);
    pthread_cond_signal(&pool->not_empty);
    pthread_mutex_unlock(&pool->queue_lock);
}