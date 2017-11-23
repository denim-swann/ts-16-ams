// Team Swinburne 2016

#ifndef __AMS_SPORADIC_TASK_HPP__
#define __AMS_SPORADIC_TASK_HPP__

#include "BaseTask.hpp"

template <class T>
struct SporadicTask : public BaseTask<T>
{
    static void base_procedure()
    {
        T::procedure();
    }
};

#endif // __AMS_SPORADIC_TASK_HPP__
