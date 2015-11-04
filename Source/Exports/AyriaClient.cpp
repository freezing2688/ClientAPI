/*
    Initial author: (https://github.com/)Convery
    License: LGPL 3.0
    Started: 2015-10-25
    Notes:
        Information about the client.
*/

#pragma warning (disable:4307)
#include "..\Utility\Crypto\FNV1a.h"
#include "ClientAPI.h"

// More readable definition of the method.
#define EXPORTMETHOD(Command, Functor)      \
    case FNV1a_Compiletime(Command):        \
        Result = Functor(Variadic);         \
        break

// Implementations.
namespace APIImplentation
{
    extern bool FetchTotalMemory(va_list Variadic);
    extern bool FetchFreeMemory(va_list Variadic);
    extern bool FetchFreeDisk(va_list Variadic);
    extern bool FetchUserID(va_list Variadic);
    extern bool FetchGUID(va_list Variadic);
    extern bool FetchMAC(va_list Variadic);
    extern bool FetchCPU(va_list Variadic);
    extern bool FetchGPU(va_list Variadic);
};

bool __cdecl AyriaClient(size_t Command, ...)
{
    bool Result = false;
    va_list Variadic;
    va_start(Variadic, Command);

    switch (Command)
    {
        // Network information via STUN; takes a uint64_t *Result.
        EXPORTMETHOD("TestBandwidthDown", [](va_list Placeholder) { return false; });
        EXPORTMETHOD("FetchGeolocation", [](va_list Placeholder) { return false; });
        EXPORTMETHOD("TestBandwidthUp", [](va_list Placeholder) { return false; });
        EXPORTMETHOD("FetchExternalIP", [](va_list Placeholder) { return false; });
        EXPORTMETHOD("FetchNATStatus", [](va_list Placeholder) { return false; });
        
        // Client hardware information; takes a char *Result.
        EXPORTMETHOD("FetchTotalMemory", APIImplentation::FetchTotalMemory);
        EXPORTMETHOD("FetchFreeMemory", APIImplentation::FetchFreeMemory);
        EXPORTMETHOD("FetchFreeDisk", APIImplentation::FetchFreeDisk);
        EXPORTMETHOD("FetchUserID", APIImplentation::FetchUserID);
        EXPORTMETHOD("FetchGUID", APIImplentation::FetchGUID);
        EXPORTMETHOD("FetchMAC", APIImplentation::FetchMAC);
        EXPORTMETHOD("FetchCPU", APIImplentation::FetchCPU);
        EXPORTMETHOD("FetchGPU", APIImplentation::FetchGPU);
        
        // Application ownership, takes an unit32_t applicationID and a bool *Result.
        EXPORTMETHOD("DownloadedApplication", [](va_list Placeholder) { return false; });
        EXPORTMETHOD("VerifyApplication", [](va_list Placeholder) { return false; });
        EXPORTMETHOD("OwnsApplication", [](va_list Placeholder) { return false; });
    }

    va_end(Variadic);
    return Result;
}
