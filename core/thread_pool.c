#include "thread_pool.h"

#include <stdlib.h>
#include <stdio.h>

struct ThreadPool thread_pool_constructor(int thread_count) {
    struct ThreadPool thread_pool;
    thread_pool.thread_count = thread_count;
    thread_pool.active = 1;
    thread_pool.work = queue_constructor();

    thread_pool.lock = (pthread_mutex_t)PTHREAD_MUTEX_INITIALIZER;
    thread_pool.signal = (pthread_cond_t)PTHREAD_COND_INITIALIZER;
    pthread_mutex_lock(&thread_pool.lock);
    thread_pool.pool = malloc(sizeof(pthread_t[thread_count]));
    for (int i = 0; i < thread_count; i++) {
        pthread_create(&thread_pool.pool[i], NULL, execute_thread_job, &thread_pool);
    } 
    pthread_mutex_unlock(&thread_pool.lock);
    thread_pool.add_work = add_work;
    return thread_pool;
};

struct ThreadJob thread_job_constructor(void *(*job)(void *), void *arg) {
    struct ThreadJob thread_job;
    thread_job.job = job;
    thread_job.arg = arg;
    return thread_job;
};

void thread_pool_destructor(struct ThreadPool *thread_pool) {
    thread_pool->active = 0;
    for (int i = 0; i < thread_pool->thread_count; i++) {
        pthread_cond_signal(&thread_pool->signal);
    }
    for (int i = 0; i < thread_pool->thread_count; i++) {
        pthread_join(thread_pool->pool[i], NULL);
    }
    free(thread_pool->pool);
    queue_destructor(&thread_pool->work);
}

void * execute_thread_job(void *arg) {
    struct ThreadPool *thread_pool = (struct ThreadPool *)arg;
    while (thread_pool->active == 1) {
        pthread_mutex_lock(&thread_pool->lock);
        pthread_cond_wait(&thread_pool->signal, &thread_pool->lock);

        struct ThreadJob thread_job = *(struct ThreadJob *)thread_pool->work.peek(&thread_pool->work);
        thread_pool->work.pop(&thread_pool->work);
        pthread_mutex_unlock(&thread_pool->lock);

        if (thread_job.job) {
            thread_job.job(thread_job.arg);
        }
    }
    return NULL;
}

void add_work(struct ThreadPool *thread_pool, struct ThreadJob thread_job) {
    pthread_mutex_lock(&thread_pool->lock);
    thread_pool->work.push(&thread_pool->work, &thread_job, sizeof(thread_job));
    pthread_cond_signal(&thread_pool->signal);
    pthread_mutex_unlock(&thread_pool->lock);
}
