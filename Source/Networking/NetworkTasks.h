/*
    Initial author: (https://github.com/)Convery
    License: LGPL 3.0
    Started: 2015-10-31
    Notes:
        Async tasks for sending and receiving data.
*/

#pragma once

// General purpose task definition.
template<typename OnSuccess, typename OnFailure, typename Task, typename ... Args>
auto RunTask(OnSuccess &&SuccessFunctor, OnFailure &&FailureFunctor, Task &&TaskFunctor, Args &&...Arguments);

// Debugging task prints performance information.
template<typename OnSuccess, typename OnFailure, typename Task, typename ... Args>
auto RunDebugTask(const char *Taskname, OnSuccess &&SuccessFunctor, OnFailure &&FailureFunctor, Task &&TaskFunctor, Args &&...Arguments);

// Macro for easier handling.
#ifndef NDEBUG
#define RUNTASK(Taskname, SuccessFunctor, FailureFunctor, TaskFunctor, Arguments) RunDebugTask(Taskname, SuccessFunctor, FailureFunctor, TaskFunctor, Arguments)
#else
#define RUNTASK(Taskname, SuccessFunctor, FailureFunctor, TaskFunctor, Arguments) RunTask(SuccessFunctor, FailureFunctor, TaskFunctor, Arguments)
#endif
