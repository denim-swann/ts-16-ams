// Team Swinburne 2016

#ifndef __AMS_CYCLIC_TASK_HPP__
#define __AMS_CYCLIC_TASK_HPP__

#include "BaseTask.hpp"

template <class T>
struct CyclicTask : public BaseTask<T>
{
    static void base_procedure()
    {
        for (;;) {
            T::procedure();
            Thread::wait(T::thread_period);
        }
    }
};

#endif // __AMS_CYCLIC_TASK_HPP__
