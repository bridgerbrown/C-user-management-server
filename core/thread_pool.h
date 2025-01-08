#ifndef THREADPOOL_H
#define THREADPOOL_H

#include "../data_structures/queue.h"

#include <pthread.h>

struct ThreadJob {
    void * (*job)(void *arg);
    void *arg;
};

struct ThreadPool {
    int thread_count;
    int active;
    struct Queue work;
    pthread_t *pool;
    pthread_mutex_t lock;
    pthread_cond_t signal;

    void (*add_work)(struct ThreadPool *thread_pool, struct ThreadJob thread_job);
};

struct ThreadPool thread_pool_constructor(int num_threads);
void thread_pool_destructor(struct ThreadPool *thread_pool);

struct ThreadJob thread_job_constructor(void * (*job)(void *arg), void *arg);
void *execute_thread_job(void *arg);
void add_work(struct ThreadPool *thread_pool, struct ThreadJob thread_job);
#endif
