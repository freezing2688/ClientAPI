/*
    Initial author: (https://github.com/)Convery
    License: LGPL 3.0
    Started: 2015-11-04
    Notes:
        Information-gathering about the clients system.
*/

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#include <stdint.h>
#include <stdarg.h>
#include <vector>
#include <windows.h>
#include <iphlpapi.h>
#include "..\..\Utility\String\VariadicString.h"
#include "..\..\Cache\CacheBase.h"

#pragma comment(lib, "Iphlpapi.lib")

// Implementation of the API.
uint64_t TotalMemory()
{
    MEMORYSTATUSEX statex;

    // Query for memory info.
    statex.dwLength = sizeof(statex);
    GlobalMemoryStatusEx(&statex);

    return statex.ullTotalVirtual;
}
uint64_t FreeMemory()
{
    MEMORYSTATUSEX statex;

    // Query for memory info.
    statex.dwLength = sizeof(statex);
    GlobalMemoryStatusEx(&statex);

    return statex.ullAvailVirtual;
}
uint64_t FreeDisk()
{
    ULARGE_INTEGER FreeSpace;
    ULARGE_INTEGER TotalSpace;
    ULARGE_INTEGER TotalFreeSpace;

    // Query for diskspace.
    GetDiskFreeSpaceExA(NULL, &FreeSpace, &TotalSpace, &TotalFreeSpace);

    return FreeSpace.QuadPart;
}
uint64_t GetUserID()
{
    static CacheEntry *UserID = new CacheEntry(
        [](CacheEntry *Entry) 
    { 
        Entry->DataSize = 8;
        Entry->Data = new uint64_t();

        /*
            TODO: Contact the server.
        */

        return true; 
    },
        [](CacheEntry *Entry)
    {
        /*
            TODO: Contact the server.
        */

        return true; 
    },
        10000);

    return *(uint64_t *)UserID->Data;
}
uint64_t GetGUID()
{
    static CacheEntry *GUID = new CacheEntry(
        [](CacheEntry *Entry) 
    { 
        Entry->DataSize = 8;
        Entry->Data = new uint64_t();

        /*
            TODO: Decide on a method for generation.
        */

        return true; 
    },
        [](CacheEntry *Entry)
    {
        /*
            TODO: Decide on a method for generation.
        */

        return true; 
    },
        10000);

    return *(uint64_t *)GUID->Data;
}
void GetCPU(size_t *SystemUnits, size_t *ProcessUnits)
{
    /*
        NOTE:
        This function returns the number of logical units.
        4 physical cores + 4 Hyperthreading cores = 8 units.
    */
    ULONG_PTR SystemAffinityMask = 0;
    ULONG_PTR ProcessAffinityMask = 0;

    // Processor information.
    GetProcessAffinityMask(GetCurrentProcess(), &ProcessAffinityMask, &SystemAffinityMask);

    // Count the units.
    for (size_t i = 0; i < sizeof(ULONG_PTR) * 8; ++i)
    {
        if (((SystemAffinityMask) >> (i)) & 1) *SystemUnits++;
        if (((ProcessAffinityMask) >> (i)) & 1) *ProcessUnits++;
    }
}

namespace APIWrapper
{
    bool FetchTotalMemory(va_list Variadic)
    {
        uint64_t *Result = va_arg(Variadic, uint64_t *);
        
        *Result = TotalMemory();
        return true;
    }
    bool FetchFreeMemory(va_list Variadic)
    {
        uint64_t *Result = va_arg(Variadic, uint64_t *);

        *Result = FreeMemory();
        return true;
    }
    bool FetchFreeDisk(va_list Variadic)
    {
        uint64_t *Result = va_arg(Variadic, uint64_t *);

        *Result = FreeDisk();
        return true;
    }
    bool FetchUserID(va_list Variadic)
    {
        uint64_t *Result = va_arg(Variadic, uint64_t *);

        *Result = GetUserID();
        return true;
    }
    bool FetchGUID(va_list Variadic)
    {
        uint64_t *Result = va_arg(Variadic, uint64_t *);

        *Result = GetGUID();
        return true;
    }
    bool FetchMAC(va_list Variadic)
    {
        char *Result = va_arg(Variadic, char *);
        IP_ADAPTER_INFO *Adapter, *Info = nullptr;
        ULONG InfoSize = 0;
        std::string ResultString;

        // Fetch the size of the info.
        GetAdaptersInfo(Info, &InfoSize);

        // Fetch all adapters.
        Info = (IP_ADAPTER_INFO *)malloc(InfoSize);
        GetAdaptersInfo(Info, &InfoSize);

        // Build the csv string for each adapter.
        for (Adapter = Info; Adapter != nullptr; Adapter = Adapter->Next)
        {
            // Prepend the comma.
            if (Adapter != Info) ResultString.append(",");

            ResultString.append(va("%2.2x", Adapter->Address[0]));
            for (size_t i = 1; i < Adapter->AddressLength; ++i)
                ResultString.append(va(":%2.2x", Adapter->Address[i]));
        }

        // Cleanup.
        free(Info);

        // Copy the string to the buffer.
        strcpy_s(Result, 1024, ResultString.c_str());
        return true;
    }
    bool FetchCPU(va_list Variadic)
    {
        size_t *System = va_arg(Variadic, size_t *);
        size_t *Process = va_arg(Variadic, size_t *);

        GetCPU(System, Process);
        return true;
    }
    bool FetchGPU(va_list Variadic)
    {
        /*
            NOTE:
            This is only intended for debug info.
            Gets the name of all physical adapters.
        */
        char *Result = va_arg(Variadic, char *);
        std::vector<std::string> AdapterNames;
        std::vector<std::string> AdapterIDs;
        std::string ResultString;
        size_t AdapterCount = 0;

        // Enumerate all adapters.
        DISPLAY_DEVICE Device;
        Device.cb = sizeof(DISPLAY_DEVICE);
        while (EnumDisplayDevicesA(NULL, AdapterCount, &Device, 0))
        {
            AdapterNames.push_back(Device.DeviceString);
            AdapterIDs.push_back(Device.DeviceID);
            AdapterCount++;
        }

        // Format the data into a csv string.
        std::string LastID;
        for (size_t i = 0; i < AdapterCount; ++i)
        {
            // Skip virtual/USB adapters.
            if (AdapterIDs[i].size() < 4)
                continue;

            // Skip multiple ports on the same adapter.
            if (strcmp(AdapterIDs[i].c_str(), LastID.c_str()) == 0)
                continue;

            // Prepend the comma.
            if(i) ResultString.append(",");

            LastID = AdapterIDs[i];
            ResultString.append(AdapterNames[i]);
        }

        // Copy the string to the buffer.
        strcpy_s(Result, 1024, ResultString.c_str());
        return true;
    }
};
