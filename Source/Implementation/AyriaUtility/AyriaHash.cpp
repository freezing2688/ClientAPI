/*
    Initial author: (https://github.com/)Convery
    License: LGPL 3.0
    Started: 2015-10-26
    Notes:
        API implementation of hashing
*/

#include <stdint.h>
#include <stdarg.h>
#include "..\..\Utility\Crypto\SM3.h"
#include "..\..\Utility\Crypto\FNV1a.h"

namespace APIImplentation
{
    bool FNV1aHash_64(va_list Variadic)
    {
        // Variadic arguments.
        char *Plaintext = va_arg(Variadic, char *);
        uint32_t PlainLength = va_arg(Variadic, uint32_t);
        char *Result = va_arg(Variadic, char *);
        uint64_t Hash = 14695981039346656037u;

        for (size_t i = 0; i < PlainLength; ++i)
        {
            Hash ^= Plaintext[i];
            Hash *= 1099511628211u;
        }
        
        memcpy(Result, &Hash, 8);
        return true;
    }
    bool SM3Hash_192(va_list Variadic)
    {
        // Variadic arguments.
        char *Plaintext = va_arg(Variadic, char *);
        uint32_t PlainLength = va_arg(Variadic, uint32_t);
        char *Result = va_arg(Variadic, char *);

        sm3((const unsigned char *)Plaintext, PlainLength, (unsigned char *)Result);
        return true;
    }
};
