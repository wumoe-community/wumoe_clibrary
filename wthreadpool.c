#include <stdlib.h>
#include <pthread.h>
#include "wthreadpool.h"

typedef struct thread_task {
    void (*function)(void *);
    void *argument;
    struct thread_task *next;
} thread_task;

typedef struct wthread_pool {
    thread_task *head;
    size_t shutdown;
    pthread_mutex_t lock;
    pthread_cond_t notify;
    pthread_t *threads;
    size_t maxnum_thread;
} wthread_pool;

static void *thread_pool_thread(void *pool);

wthread_pool *new_thread_pool(size_t maxnum_thread) {
    wthread_pool *new_pool = (wthread_pool *) malloc(sizeof(wthread_pool));
    new_pool->maxnum_thread = maxnum_thread;
    new_pool->shutdown = 0;
    new_pool->head = NULL;
    new_pool->threads = (pthread_t *)malloc(sizeof(pthread_t) * new_pool->maxnum_thread);
    pthread_mutex_init(&new_pool->lock, NULL);
    pthread_cond_init(&new_pool->notify, NULL);
    for(int i = 0; i < maxnum_thread; ++i)
        pthread_create(&new_pool->threads[i],NULL,thread_pool_thread,new_pool);
    return new_pool;
}

void thread_pool_add(wthread_pool *pool, void (*routine)(void *), void *argument) {
    thread_task *task = (thread_task *) malloc(sizeof(thread_task));
    task->function = routine;
    task->argument = argument;
    task->next = NULL;
    pthread_mutex_lock(&pool->lock);
    thread_task *backups = pool->head;
    if(!backups)
        pool->head = task;
    else {
        while(backups->next)
            backups = backups->next;
        backups->next = task;
    }
    pthread_mutex_unlock(&pool->lock);
    pthread_cond_signal(&pool->notify);
}

void thread_pool_destroy(wthread_pool *pool) {
    pool->shutdown = 1;
    pthread_mutex_lock(&pool->lock);
    pthread_cond_broadcast(&pool->notify);
    pthread_mutex_unlock(&pool->lock);
    for(int i = 0; i < pool->maxnum_thread; ++i)
        pthread_join(pool->threads[i],NULL);
    free(pool->threads);
    thread_task *backups;
    while(pool->head) {
        backups = pool->head;
        pool->head = pool->head->next;
        free(backups);
    }
    pthread_mutex_destroy(&pool->lock);
    pthread_cond_destroy(&pool->notify);
    free(pool);
}

static void *thread_pool_thread(void *pool) {
    wthread_pool *tmp = (wthread_pool *) pool;
    thread_task *task;
    while (1) {
        pthread_mutex_lock(&tmp->lock);
        while(!tmp->head && !tmp->shutdown)
            pthread_cond_wait(&tmp->notify,&tmp->lock);
        if(tmp->shutdown) {
            pthread_mutex_unlock(&tmp->lock);
            pthread_exit(NULL);
        }
        task = tmp->head;
        tmp->head = tmp->head->next;
        pthread_mutex_unlock(&tmp->lock);
        task->function(task->argument);
        free(task);
    }
}
