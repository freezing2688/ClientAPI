/*
    Initial author: (https://github.com/)Convery
    License: LGPL 3.0
    Started: 2015-12-08
    Notes:
        Statistics tracking across all applications.
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
};

bool __cdecl AyriaStats(size_t Command, ...)
{
    bool Result = false;
    va_list Variadic;
    va_start(Variadic, Command);

    switch (Command)
    {

    }

    va_end(Variadic);
    return Result;
}