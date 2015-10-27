/*
    Initial author: (https://github.com/)Convery
    License: LGPL 3.0
    Started: 2015-10-22
    Notes:
        Local and remote storage; both persistent and transient.
*/

#pragma warning (disable:4307)
#include "..\Utility\Crypto\FNV1a.h"
#include "ClientAPI.h"

// More readable definition of the method.
#define EXPORTMETHOD(Command, Functor)      \
    case FNV1a_Compiletime(Command):        \
        Result = Functor(Variadic);         \
        break

namespace APIImplentation
{
    extern bool WriteLocalTransient(va_list Variadic);
    extern bool WriteLocalPersistent(va_list Variadic);
    extern bool WriteRemoteTransient(va_list Variadic);
    extern bool WriteRemotePersistent(va_list Variadic);

    extern bool ReadLocalTransient(va_list Variadic);
    extern bool ReadLocalPersistent(va_list Variadic);
    extern bool ReadRemoteTransient(va_list Variadic);
    extern bool ReadRemotePersistent(va_list Variadic);
};

bool __cdecl AyriaKeyvalue(size_t Command, ...)
{
    bool Result = false;
    va_list Variadic;
    va_start(Variadic, Command);

    switch (Command)
    {
        // Takes a uint64_t key and a char data[32] as argument.      
        EXPORTMETHOD("WriteLocalTransient", APIImplentation::WriteLocalTransient);
        EXPORTMETHOD("WriteLocalPersistent", APIImplentation::WriteLocalPersistent);
        EXPORTMETHOD("WriteRemoteTransient", APIImplentation::WriteRemoteTransient);
        EXPORTMETHOD("WriteRemotePersistent", APIImplentation::WriteRemotePersistent);

        // Takes a uint64_t key and a char data[32] as argument.
        EXPORTMETHOD("ReadLocalTransient", APIImplentation::ReadLocalTransient);
        EXPORTMETHOD("ReadLocalPersistent", APIImplentation::ReadLocalPersistent);
        EXPORTMETHOD("ReadRemoteTransient", APIImplentation::ReadRemoteTransient);
        EXPORTMETHOD("ReadRemotePersistent", APIImplentation::ReadRemotePersistent);
    }

    va_end(Variadic);
    return Result;
}
