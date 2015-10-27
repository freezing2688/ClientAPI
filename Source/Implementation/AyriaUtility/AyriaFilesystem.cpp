/*
    Initial author: (https://github.com/)Convery
    License: LGPL 3.0
    Started: 2015-10-27
    Notes:
        API implementation of filesystem management.
*/

#include <stdint.h>
#include <stdarg.h>
#include "..\..\Utility\Filesystem\CSVManager.h"

namespace APIImplentation
{
    CSVManager GlobalManager;
    uint32_t RowIndex = 0;
    uint32_t ColIndex = 0;

    bool CSVLoad(va_list Variadic)
    {
        // Variadic arguments.
        char *Filename = va_arg(Variadic, char *);

        // Reset the global index.
        RowIndex = 0;
        ColIndex = 0;

        // Read the entire file.
        GlobalManager.BeginRead(Filename);
        GlobalManager.ReadAll();

        return true;
    }
    bool CSVReadNext(va_list Variadic)
    {
        // Variadic arguments.
        char *Result = va_arg(Variadic, char *);
        std::vector<std::vector<std::string>> Buffer;

        // Copy the buffer.
        GlobalManager.GetBuffer(Buffer);

        // Clamp the index.
        if (ColIndex >= Buffer[RowIndex].size())
        {
            ColIndex = 0;
            RowIndex++;
        }
        if (RowIndex >= Buffer.size())
        {
            Result = "";
            ColIndex = 0;
            RowIndex = 0;
            return true;
        }

        strcpy_s(Result, 1024, Buffer[RowIndex][ColIndex].c_str());
        return true;
    }
};