/*
    Initial author: (https://github.com/)Convery
    License: LGPL 3.0
    Started: 2015-11-05
    Notes:
        A simple way to store data being updated remotely.
*/

#pragma once
#include <stdint.h>

// Base task definition.
using CacheTask = bool(struct CacheData *Cache);

// Add or remove a data entry.
size_t CreateTask(struct CacheData *Cache, CacheTask Updatetask, CacheTask Creationtask);
size_t CreateTask(struct CacheData *Cache);
void DeleteTask(size_t Index);

// Update the data struct.
void InvokeTask(size_t Index, CacheTask Updatetask);
void InvokeTask(size_t Index);
