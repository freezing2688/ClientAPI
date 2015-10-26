/*
    Initial author: (https://github.com/)Convery
    License: LGPL 3.0
    Started: 2015-10-25
    Notes:
        API implementation of compression.
*/

#include <stdint.h>
#include <stdarg.h>
#include "..\..\Utility\Compression\lz4.h"

namespace APIImplentation
{
    bool LZ4Inflate(va_list Variadic)
    {
        // Variadic arguments.
        char *Plaintext = va_arg(Variadic, char *);
        char **ModifiedBuffer = va_arg(Variadic, char **);
        int32_t PlainLength = va_arg(Variadic, int32_t);
        int32_t *ModifiedLength = va_arg(Variadic, int32_t *);

        // LZ4 compression is about 2.1, so a large buffer is needed.
        *ModifiedBuffer = new char[PlainLength * 3]();
        *ModifiedLength = LZ4_decompress_safe(Plaintext, *ModifiedBuffer, PlainLength, PlainLength * 3);

        // If decompression failed, delete the buffer.
        if (*ModifiedLength < 0)
            delete[] *ModifiedBuffer;

        return true;
    }
    bool LZ4Deflate(va_list Variadic)
    {
        // Variadic arguments.
        char *Plaintext = va_arg(Variadic, char *);
        char **ModifiedBuffer = va_arg(Variadic, char **);
        int32_t PlainLength = va_arg(Variadic, int32_t);
        int32_t *ModifiedLength = va_arg(Variadic, int32_t *);

        // LZ4 compression is about 2.1, so a large buffer is needed.
        *ModifiedBuffer = new char[PlainLength * 3]();
        *ModifiedLength = LZ4_compress_fast(Plaintext, *ModifiedBuffer, PlainLength, PlainLength * 3, 1);

        // If compression failed, delete the buffer.
        if (*ModifiedLength < 0)
            delete[] *ModifiedBuffer;

        return true;
    }
};
