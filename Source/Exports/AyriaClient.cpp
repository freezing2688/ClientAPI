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
        EXPORTMETHOD("FetchTotalMemory", [](va_list Placeholder) { return false; });
        EXPORTMETHOD("FetchFreeMemory", [](va_list Placeholder) { return false; });
        EXPORTMETHOD("FetchFreeDisk", [](va_list Placeholder) { return false; });
        EXPORTMETHOD("FetchUserID", [](va_list Placeholder) { return false; });
        EXPORTMETHOD("FetchGUID", [](va_list Placeholder) { return false; });
        EXPORTMETHOD("FetchMAC", [](va_list Placeholder) { return false; });
        EXPORTMETHOD("FetchCPU", [](va_list Placeholder) { return false; });
        EXPORTMETHOD("FetchGPU", [](va_list Placeholder) { return false; });
    }

    va_end(Variadic);
    return Result;
}
