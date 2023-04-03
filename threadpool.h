#ifndef WUMOE_THREADPOOL_H
#define WUMOE_THREADPOOL_H

typedef struct thread_pool thread_pool;

thread_pool *new_thread_pool(size_t maxnum_thread);

void thread_pool_add(thread_pool *pool, void (*routine)(void *), void *argument);

void thread_pool_destroy(thread_pool *pool);

#endif
