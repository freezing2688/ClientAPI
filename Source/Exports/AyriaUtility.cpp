/*
    Initial author: (https://github.com/)Convery
    License: LGPL 3.0
    Started: 2015-10-25
    Notes:
        Requested utilities to save space in other modules.
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
    extern bool LZ4Inflate(va_list Variadic);
    extern bool LZ4Deflate(va_list Variadic);
};

bool __cdecl AyriaUtility(size_t Command, ...)
{
    bool Result = false;
    va_list Variadic;
    va_start(Variadic, Command);

    switch (Command)
    {
        // Encryption; takes a char *Plaintext, char *Ciphertext, char *IV, char *Key, uint32_t Length.
        EXPORTMETHOD("AES256Encrypt", [](va_list Placeholder) { return false; });
        EXPORTMETHOD("AES256Decrypt", [](va_list Placeholder) { return false; });
        EXPORTMETHOD("DES3Encrypt", [](va_list Placeholder) { return false; });
        EXPORTMETHOD("DES3Decrypt", [](va_list Placeholder) { return false; });
        EXPORTMETHOD("TEAEncrypt", [](va_list Placeholder) { return false; });
        EXPORTMETHOD("TEADecrypt", [](va_list Placeholder) { return false; });
        EXPORTMETHOD("RC6Encrypt", [](va_list Placeholder) { return false; });
        EXPORTMETHOD("RC6Decrypt", [](va_list Placeholder) { return false; });

        // Keyexchange; takes a char *Request (when peer) and a char *Response.
        EXPORTMETHOD("HHSStart", [](va_list Placeholder) { return false; });
        EXPORTMETHOD("HHSContinue", [](va_list Placeholder) { return false; });
        EXPORTMETHOD("RSAStart", [](va_list Placeholder) { return false; });
        EXPORTMETHOD("RSAContinue", [](va_list Placeholder) { return false; });

        // Hashing; takes a char *Plaintext, uint32_t PlainLength, char *Result.
        EXPORTMETHOD("FNV1aHash_64", [](va_list Placeholder) { return false; });
        EXPORTMETHOD("SM3Hash_192", [](va_list Placeholder) { return false; });
        EXPORTMETHOD("SHA3Hash_256", [](va_list Placeholder) { return false; });

        // File readers; takes a char *Data.
        EXPORTMETHOD("CSVLoad", [](va_list Placeholder) { return false; });
        EXPORTMETHOD("CSVReadNext", [](va_list Placeholder) { return false; });
        EXPORTMETHOD("CSVSkipLine", [](va_list Placeholder) { return false; });

        // Compression; takes a char *Plaintext, char **ModifiedBuffer, uint32_t PlainLength, int32_t *ModifiedLength.
        EXPORTMETHOD("LZ4Inflate", APIImplentation::LZ4Inflate);
        EXPORTMETHOD("LZ4Deflate", APIImplentation::LZ4Deflate);
    }

    va_end(Variadic);
    return Result;
}
