/*
    Initial author: (https://github.com/)Convery
    License: LGPL 3.0
    Started: 2015-12-08
    Notes:
        A simple system to log debug data to a file.
*/

#pragma once
#include <stdio.h>
#include <mutex>
#include "VariadicString.h"

// Easier to edit with a constant path.
constexpr const char *Filepath = "AyriaClient.log";

// Open and close a file rather than flushing it properly incase
// we have other tools that need write access.
inline void AppendToLogfile(const char *Message, const char *Prefix)
{
    FILE *Filehandle;
    static std::mutex WriteMutex;

    WriteMutex.lock();
    if (0 == fopen_s(&Filehandle, Filepath, "a"))
    {
        fputs(va("[%-8s] ", Prefix), Filehandle);
        fputs(Message, Filehandle);
        fputs("\n", Filehandle);
        fclose(Filehandle);
    }
    WriteMutex.unlock();
}
inline void DeleteLogfile()
{
    std::remove(Filepath);
}
