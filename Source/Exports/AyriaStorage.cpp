/*
    Initial author: (https://github.com/)Convery
    License: LGPL 3.0
    Started: 2015-10-25
    Notes:
        Local and remote filestorage.
*/

#pragma warning (disable:4307)
#include "..\Utility\Crypto\FNV1a.h"
#include "ClientAPI.h"

// More readable definition of the method.
#define EXPORTMETHOD(Command, Functor)      \
    case FNV1a_Compiletime(Command):        \
        Result = Functor(Variadic);         \
        break

bool __cdecl AyriaStorage(size_t Command, ...)
{
    bool Result = false;
    va_list Variadic;
    va_start(Variadic, Command);

    switch (Command)
    {
        // Takes a char *filename; applicationID is sent automatically.
        EXPORTMETHOD("WriteUserfile", [](va_list Placeholder) { return false; });
        EXPORTMETHOD("WriteLocalfile", [](va_list Placeholder) { return false; });
        EXPORTMETHOD("WriteMemoryfile", [](va_list Placeholder) { return false; });
        EXPORTMETHOD("WritePublicfile", [](va_list Placeholder) { return false; });

        // Takes a char *filename, size_t buffersize, char *buffer; applicationID is sent automatically.
        EXPORTMETHOD("ReadUserfile", [](va_list Placeholder) { return false; });
        EXPORTMETHOD("ReadLocalfile", [](va_list Placeholder) { return false; });
        EXPORTMETHOD("ReadMemoryfile", [](va_list Placeholder) { return false; });
        EXPORTMETHOD("ReadPublicfile", [](va_list Placeholder) { return false; });
    }

    va_end(Variadic);
    return Result;
}
