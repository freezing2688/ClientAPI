/*
    Initial author: (https://github.com/)Convery
    License: LGPL 3.0
    Started: 2015-11-05
    Notes:
        A simple way to store data being updated remotely.
*/

#include "CacheData.h"
#include "CacheTask.h"
#include <future>
#include <thread>
#include <mutex>
#include <chrono>

// Internal variables.
std::mutex DataLock;
std::mutex TaskLock;
std::vector<struct CacheData *> DataEntries;
std::vector<std::pair<std::chrono::time_point<std::chrono::system_clock>, std::future<bool>>> TaskEntries;

// Update all the tasks.
void TaskThread()
{
    while (true)
    {
        auto Timestamp = std::chrono::system_clock::now();
        auto ElapsedTime = [Initial = std::chrono::high_resolution_clock::now()]()->std::chrono::duration<double, std::milli>
        { return std::chrono::high_resolution_clock::now() - Initial; };

        // Remove completed tasks.
        for (auto Iterator = TaskEntries.begin(); Iterator != TaskEntries.end(); ++Iterator)
        {
            // 10 second timeout for tasks.
            if ((Timestamp - Iterator->first).count() > 10000)
            {
                TaskLock.lock();
                TaskEntries.erase(Iterator);
                TaskLock.unlock();
                continue;
            }

            // Running at 30 tasks per sec.
            if (std::future_status::ready == Iterator->second.wait_for(std::chrono::milliseconds(33)))
            {
                TaskLock.lock();
                TaskEntries.erase(Iterator);
                TaskLock.unlock();
                continue;
            }
        }

        // Create new tasks for old records.
        DataLock.lock();
        for (size_t i = 0; i < DataEntries.size(); ++i)
        {
            if (DataEntries[i]->Active)
            {
                if (DataEntries[i]->TicksSinceUpdate > DataEntries[i]->UpdateRate)
                {
                    DataEntries[i]->TicksSinceUpdate = 0;
                    InvokeTask(i);
                }
            }
            else
            {
                DataEntries[i]->TicksSinceUpdate += ElapsedTime().count();
            }            
        }
        DataLock.unlock();

        // Sleep for half a sec to allow for other blocking operations.
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }
}
void SpawnTaskThread()
{
    static bool Spawned = false;
    if (!Spawned)
    {
        std::thread(TaskThread).detach();
        Spawned = true;
    }
}

// Add or remove a data entry.
size_t CreateTask(struct CacheData *Cache, CacheTask Updatetask, CacheTask Creationtask)
{
    size_t Index;
    SpawnTaskThread();

    DataLock.lock();
    Index = DataEntries.size();
    DataEntries.push_back(Cache);

    DataEntries[Index]->Active = true;
    DataEntries[Index]->UpdateTask = Updatetask;
    DataEntries[Index]->CreationTask = Creationtask;

    CreateTask(DataEntries[Index]);
    DataLock.unlock();
    return Index;
}
size_t CreateTask(struct CacheData *Cache)
{
    size_t Index;
    SpawnTaskThread();

    DataLock.lock();
    Index = DataEntries.size();
    DataEntries.push_back(Cache);

    if (DataEntries[Index] != nullptr)
        DataEntries[Index]->CreationTask(DataEntries[Index]);

    DataLock.unlock();
    return Index;
}
void DeleteTask(size_t Index)
{
    DataLock.lock();
    DataEntries[Index]->Active = false;
    DataLock.unlock();
}

// Update the data struct.
void InvokeTask(size_t Index, CacheTask Updatetask)
{
    TaskLock.lock();
    TaskEntries.push_back({std::chrono::system_clock::now(), std::async(std::launch::async, Updatetask, DataEntries[Index])});
    TaskLock.unlock();
}
void InvokeTask(size_t Index)
{
    TaskLock.lock();
    TaskEntries.push_back({std::chrono::system_clock::now(), std::async(std::launch::async, DataEntries[Index]->UpdateTask, DataEntries[Index])});
    TaskLock.unlock();
}
