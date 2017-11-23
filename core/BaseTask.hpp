// Team Swinburne 2016

#ifndef __AMS_BASE_TASK_HPP__
#define __AMS_BASE_TASK_HPP__

#include "mbed.h"

template <class T>
struct BaseTask
{
    static Thread m_thread;

    static void create()
    {
        T::initialise();
        m_thread.start(T::base_procedure);
        m_thread.set_priority(T::thread_priority);
    }
};

template <typename T> Thread BaseTask<T>::m_thread;

#endif // __AMS_BASE_TASK_HPP__
