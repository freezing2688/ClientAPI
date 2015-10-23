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

bool __cdecl AyriaKeyvalue(size_t Command, ...)
{
    bool Result = false;
    va_list Variadic;
    va_start(Variadic, Command);

    switch (Command)
    {
        // Takes a size_t key and a char data[32] as argument.
        EXPORTMETHOD("WriteLocalTransient", [](va_list Placeholder) { return false; });
        EXPORTMETHOD("WriteLocalPersistent", [](va_list Placeholder) { return false; });
        EXPORTMETHOD("WriteRemoteTransient", [](va_list Placeholder) { return false; });
        EXPORTMETHOD("WriteRemotePersistent", [](va_list Placeholder) { return false; });

        // Takes a size_t key and a char *data as argument.
        EXPORTMETHOD("ReadLocalTransient", [](va_list Placeholder) { return false; });
        EXPORTMETHOD("ReadLocalPersistent", [](va_list Placeholder) { return false; });
        EXPORTMETHOD("ReadRemoteTransient", [](va_list Placeholder) { return false; });
        EXPORTMETHOD("ReadRemotePersistent", [](va_list Placeholder) { return false; });
    }

    va_end(Variadic);
    return Result;
}
