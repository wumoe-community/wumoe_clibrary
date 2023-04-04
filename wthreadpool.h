#ifndef WUMOE_THREADPOOL_H
#define WUMOE_THREADPOOL_H

typedef struct wthread_pool wthread_pool;

wthread_pool *new_thread_pool(size_t maxnum_thread);

void thread_pool_add(wthread_pool *pool, void (*routine)(void *), void *argument);

void thread_pool_destroy(wthread_pool *pool);

#endif
