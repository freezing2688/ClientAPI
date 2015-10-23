/*
    Initial author: (https://github.com/)Convery
    License: LGPL 3.0
    Started: 2015-10-22
    Notes:
        Incrementation and decrementation of application specific keys.
*/

#pragma warning (disable:4307)
#include "..\Utility\Crypto\FNV1a.h"
#include "ClientAPI.h"

// More readable definition of the method.
#define EXPORTMETHOD(Command, Functor)      \
    case FNV1a_Compiletime(Command):        \
        Result = Functor(Variadic);         \
        break

bool __cdecl AyriaCounter(size_t Command, ...)
{
    bool Result = false;
    va_list Variadic;
    va_start(Variadic, Command);

    switch (Command)
    {
        // Takes a size_t key as argument.
        EXPORTMETHOD("IncrementLocal", [](va_list Placeholder) { return false; });
        EXPORTMETHOD("IncrementRemote", [](va_list Placeholder) { return false; });

        // Takes a size_t key as argument.
        EXPORTMETHOD("DecrementLocal", [](va_list Placeholder) { return false; });
        EXPORTMETHOD("DecrementRemote", [](va_list Placeholder) { return false; });
    }

    va_end(Variadic);
    return Result;
}