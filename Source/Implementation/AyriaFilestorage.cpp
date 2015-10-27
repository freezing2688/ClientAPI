/*
    Initial author: (https://github.com/)Convery
    License: LGPL 3.0
    Started: 2015-10-27
    Notes:
        API implementation of filestorage.
*/

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#include <stdint.h>
#include <stdarg.h>
#include <direct.h>
#include <windows.h>
#include <vector>
#include "..\Utility\String\VariadicString.h"

#define PRIVILEGE_ISSUE 12
#define WRITE_ISSUE 13
#define READ_ISSUE 14
#define BUFFER_TOO_SMALL 16
#define NOT_FOUND 404

struct MemoryFile
{
    bool Free{ true };
    char *Filename{ nullptr };
    size_t Length{};
    void *Buffer{ nullptr };
};

namespace APIImplentation
{
    std::vector<MemoryFile> MemoryStorage;

    bool WriteUserfile(va_list Variadic)
    {
        // Variadic arguments.
        char *Filename = va_arg(Variadic, char *);
        size_t BufferSize = va_arg(Variadic, size_t);
        char *Filebuffer = va_arg(Variadic, char *);

        return false;
    }
    bool WriteLocalfile(va_list Variadic)
    {
        // Variadic arguments.
        char *Filename = va_arg(Variadic, char *);
        size_t BufferSize = va_arg(Variadic, size_t);
        char *Filebuffer = va_arg(Variadic, char *);
        FILE *Filehandle;

        // Ensure that the path exists.
        _mkdir("Ayria");
        _mkdir("Ayria\\Storage");

        // Create a new file or overwrite an existing one.
        if (fopen_s(&Filehandle, va("Ayria\\Storage\\%s", Filename), "wb") != 0)
        {
            SetLastError(PRIVILEGE_ISSUE);
            return false;
        }

        // Write to disk.
        // TODO (Convery):
        // Maybe write the file in 4 chunks or threaded to avoid blocking?
        if (fwrite(Filebuffer, BufferSize, 1, Filehandle) != 1)
        {
            remove(va("Ayria\\Storage\\%s", Filename));
            SetLastError(WRITE_ISSUE);
            return false;
        }

        fclose(Filehandle);
        return false;
    }
    bool WriteMemoryfile(va_list Variadic)
    {
        // Variadic arguments.
        char *Filename = va_arg(Variadic, char *);
        size_t BufferSize = va_arg(Variadic, size_t);
        char *Filebuffer = va_arg(Variadic, char *);
        MemoryFile *File{ nullptr };

        // Iterate through the storage and remove any existing files.
        for (auto Iterator = MemoryStorage.begin(); Iterator != MemoryStorage.end(); ++Iterator)
        {
            if (Iterator->Filename != nullptr)
            {
                if (_strcmpi(Iterator->Filename, Filename) == 0)
                {
                    memset(Iterator->Buffer, 0, Iterator->Length);
                    free(Iterator->Buffer);
                    delete[] Iterator->Filename;
                    Iterator->Free = true;
                    break;
                }
            }
        }

        // Iterate through the storage and reuse any free slots.
        for (auto Iterator = MemoryStorage.begin(); Iterator != MemoryStorage.end(); ++Iterator)
        {
            if (Iterator->Free)
            {
                File = &(*Iterator);
                File->Free = false;
                break;
            }
        }

        // Create a new entry if needed.
        if (File == nullptr)
        {
            File = new MemoryFile();
            MemoryStorage.push_back(*File);
            File = &MemoryStorage.back();
        }
            
        // Initialize the file.
        File->Filename = new char[strlen(Filename) + 1]();
        File->Length = BufferSize;
        File->Buffer = malloc(BufferSize);
        
        strcpy_s(File->Filename, strlen(Filename) + 1, Filename);
        memcpy(File->Buffer, Filebuffer, BufferSize);

        return true;
    }
    bool WritePublicfile(va_list Variadic)
    {
        // Variadic arguments.
        char *Filename = va_arg(Variadic, char *);
        size_t BufferSize = va_arg(Variadic, size_t);
        char *Filebuffer = va_arg(Variadic, char *);

        return false;
    }

    bool ReadUserfile(va_list Variadic)
    {
        // Variadic arguments.
        char *Filename = va_arg(Variadic, char *);
        size_t *BufferSize = va_arg(Variadic, size_t *);
        char *Filebuffer = va_arg(Variadic, char *);

        return false;
    }
    bool ReadLocalfile(va_list Variadic)
    {
        // Variadic arguments.
        char *Filename = va_arg(Variadic, char *);
        size_t *BufferSize = va_arg(Variadic, size_t *);
        char *Filebuffer = va_arg(Variadic, char *);
        size_t Filesize = 0;
        FILE *Filehandle;

        // Create a new file or overwrite an existing one.
        if (fopen_s(&Filehandle, va("Ayria\\Storage\\%s", Filename), "wb") != 0)
        {
            SetLastError(PRIVILEGE_ISSUE);
            return false;
        }

        // Get the filesize.
        fseek(Filehandle, 0, SEEK_END);
        Filesize = ftell(Filehandle);
        rewind(Filehandle);

        // Rangecheck.
        if (Filesize > *BufferSize)
        {
            fclose(Filehandle);
            SetLastError(BUFFER_TOO_SMALL);
            return false;
        }

        // Read into the buffer.
        // TODO (Convery):
        // Maybe read the file in 4 chunks or threaded to avoid blocking?
        if (fread_s(Filebuffer, *BufferSize, Filesize, 1, Filehandle) != 1)
        {
            fclose(Filehandle);
            SetLastError(READ_ISSUE);
            return false;
        }

        fclose(Filehandle);
        return true;
    }
    bool ReadMemoryfile(va_list Variadic)
    {
        // Variadic arguments.
        char *Filename = va_arg(Variadic, char *);
        size_t *BufferSize = va_arg(Variadic, size_t *);
        char *Filebuffer = va_arg(Variadic, char *);

        // Iterate through the storage and find the requested file.
        for (auto Iterator = MemoryStorage.begin(); Iterator != MemoryStorage.end(); ++Iterator)
        {
            if (Iterator->Filename != nullptr)
            {
                if (_strcmpi(Iterator->Filename, Filename) == 0)
                {
                    if (Iterator->Length > *BufferSize)
                    {
                        SetLastError(BUFFER_TOO_SMALL);
                        return false;
                    }

                    memcpy(Filebuffer, Iterator->Buffer, Iterator->Length);
                    *BufferSize = Iterator->Length;
                    return true;
                }
            }
        }

        SetLastError(NOT_FOUND);
        return false;
    }
    bool ReadPublicfile(va_list Variadic)
    {
        // Variadic arguments.
        char *Filename = va_arg(Variadic, char *);
        size_t *BufferSize = va_arg(Variadic, size_t *);
        char *Filebuffer = va_arg(Variadic, char *);

        return false;
    }
};
