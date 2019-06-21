#ifndef THREADPOOL_H
#define THREADPOOL_H

#include <list>
#include <cstdio>
#include <exception>
#include <pthread.h>

#include "locker.h"

template< typename T>
class threadpool
{
public:
    //参数thread_number 是线程池中线程的数量，max_requests是请求队列中最多允许的、等待处理的请求的数量。
    threadpool(int thread_number = 4, int max_requests = 10000);
    ~threadpool();
    //往请求队列中添加任务；
    bool append(T *request);
private:
    //工作线程运行的函数，它不断从工作队列中取出任务并执行之
    static void* worker(void *arg);
    void run();
private:
    int m_thread_number;            //线程池中的线程数
    int m_max_requests;             //请求队列中允许的最大请求数
    pthread_t *m_threads;           //描述线程池的数组，大小为m_thread_number;
    std::list<T *> m_workqueue;     //请求队列
    locker m_queuelocker;           //保护请求队列的互斥锁
    sem m_queuestat;                //是否有任务需要处理
    bool m_stop;                    //是否结束线程
};

#endif