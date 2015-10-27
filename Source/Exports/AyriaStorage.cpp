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

// Implementations.
namespace APIImplentation
{
    extern bool WriteUserfile(va_list Variadic);
    extern bool WriteLocalfile(va_list Variadic);
    extern bool WriteMemoryfile(va_list Variadic);
    extern bool WritePublicfile(va_list Variadic);

    extern bool ReadUserfile(va_list Variadic);
    extern bool ReadLocalfile(va_list Variadic);
    extern bool ReadMemoryfile(va_list Variadic);
    extern bool ReadPublicfile(va_list Variadic);
};

bool __cdecl AyriaStorage(size_t Command, ...)
{
    bool Result = false;
    va_list Variadic;
    va_start(Variadic, Command);

    switch (Command)
    {
        // Takes a char *filename, size_t buffersize, char *buffer; applicationID is sent automatically.
        EXPORTMETHOD("WriteUserfile", APIImplentation::WriteUserfile);
        EXPORTMETHOD("WriteLocalfile", APIImplentation::WriteLocalfile);
        EXPORTMETHOD("WriteMemoryfile", APIImplentation::WriteMemoryfile);
        EXPORTMETHOD("WritePublicfile", APIImplentation::WritePublicfile);

        // Takes a char *filename, size_t *buffersize, char *buffer; applicationID is sent automatically.
        EXPORTMETHOD("ReadUserfile", APIImplentation::ReadUserfile);
        EXPORTMETHOD("ReadLocalfile", APIImplentation::ReadLocalfile);
        EXPORTMETHOD("ReadMemoryfile", APIImplentation::ReadMemoryfile);
        EXPORTMETHOD("ReadPublicfile", APIImplentation::ReadPublicfile);
    }

    va_end(Variadic);
    return Result;
}
