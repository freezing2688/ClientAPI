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

// Implementations.
namespace APIImplentation
{
    extern bool IncrementLocal(va_list Variadic);
    extern bool IncrementRemote(va_list Variadic);
    extern bool DecrementLocal(va_list Variadic);
    extern bool DecrementRemote(va_list Variadic);
};

bool __cdecl AyriaCounter(size_t Command, ...)
{
    bool Result = false;
    va_list Variadic;
    va_start(Variadic, Command);

    switch (Command)
    {
        // Takes a uint64_t key as argument.
        EXPORTMETHOD("IncrementLocal", APIImplentation::IncrementLocal);
        EXPORTMETHOD("IncrementRemote", APIImplentation::IncrementRemote);

        // Takes a uint64_t key as argument.
        EXPORTMETHOD("DecrementLocal", APIImplentation::DecrementLocal);
        EXPORTMETHOD("DecrementRemote", APIImplentation::DecrementRemote);
    }

    va_end(Variadic);
    return Result;
}