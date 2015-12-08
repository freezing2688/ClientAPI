/*
    Initial author: (https://github.com/)Convery
    License: LGPL 3.0
    Started: 2015-12-08
    Notes:
        This struct is intended to be used sparsely.
*/

#pragma once
#include <future>

// Definition for functors used by the cache system.
using CacheTask = bool(struct CacheEntry *Cache);

// Base data entry for cached data entries.
struct CacheEntry
{
    // Time properties.
    uint64_t LastUpdate{ 0 };
    uint32_t UpdateRate{ 5000 };

    // Data properties.
    uint32_t DataSize{ 0 };
    void *Data{ nullptr };
    bool Dirty{ false };

    // Functors for updating the data.
    CacheTask *InitialTask{ nullptr };
    CacheTask *UpdateTask{ nullptr };

    // Update thread and future.
    std::future<void> UpdateFuture;
    static void UpdateThread(CacheEntry *Caller)
    {
        while (true)
        {
            // Thread termination requested.
            if (Caller->UpdateRate == 0)
                return;

            // Steady update rate, reduce in a derived class if needed.
            std::this_thread::sleep_for(std::chrono::milliseconds(100));

            // Update the cache if old enough.
            if (time(NULL) - Caller->LastUpdate > Caller->UpdateRate)
            {
                if (Caller->UpdateTask != nullptr)
                    Caller->Dirty = Caller->UpdateTask(Caller);

                Caller->LastUpdate = time(NULL);
            }
        }
    }
    void StartThread()
    {
        UpdateFuture = std::async(std::launch::async, CacheEntry::UpdateThread, this);
    }
    void StopThread()
    {
        UpdateRate = 0;
    }

    // Constructors.
    CacheEntry()
    {
        if (InitialTask != nullptr)
            Dirty = InitialTask(this);

        LastUpdate = time(NULL);
    }
    CacheEntry(CacheTask *Initial)
    {
        InitialTask = Initial;
        if (InitialTask != nullptr)
            Dirty = InitialTask(this);

        LastUpdate = time(NULL);
    }
    CacheEntry(CacheTask *Initial, CacheTask *Update)
    {
        UpdateTask = Update;
        InitialTask = Initial;
        if (InitialTask != nullptr)
            Dirty = InitialTask(this);

        LastUpdate = time(NULL);                
    }
    CacheEntry(CacheTask *Initial, CacheTask *Update, uint32_t Rate)
    {
        UpdateRate = Rate;
        UpdateTask = Update;
        InitialTask = Initial;
        if (InitialTask != nullptr)
            Dirty = InitialTask(this);

        LastUpdate = time(NULL);
    }
};
