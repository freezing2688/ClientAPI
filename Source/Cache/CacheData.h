/*
    Initial author: (https://github.com/)Convery
    License: LGPL 3.0
    Started: 2015-11-05
    Notes:
        A simple way to store data being updated remotely.
*/

#pragma once
#include "CacheTask.h"

// Base cache definition to be expanded upon.
struct CacheData
{
    bool Active;
    uint32_t TicksSinceUpdate{};
    uint32_t UpdateRate{ 5000 };

    CacheTask *UpdateTask;
    CacheTask *CreationTask;
};

/*
    Module specific implementations below this line.
*/
