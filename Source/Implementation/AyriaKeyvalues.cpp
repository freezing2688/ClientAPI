/*
    Initial author: (https://github.com/)Convery
    License: LGPL 3.0
    Started: 2015-10-27
    Notes:
        API implementation of Keyval modification.
*/

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#include <stdint.h>
#include <stdarg.h>
#include <direct.h>
#include <windows.h>
#include "..\Database\unqlite.h"

#define PRIVILEGE_ISSUE 12
#define WRITE_ISSUE 13

namespace APIImplentation
{
    bool IncrementLocal(va_list Variadic)
    {
        // Variadic arguments.
        uint64_t Key = va_arg(Variadic, uint64_t);
        unqlite *LocalDatabase;
        int64_t Value;
        int64_t RecordLength = sizeof(int64_t);

        // Verify that we have the folder structure needed.
        _mkdir("Ayria");
        _mkdir("Ayria\\Database");

        // Open the local database.
        if (unqlite_open(&LocalDatabase, "Ayria\\Database\\Counter.db", UNQLITE_OPEN_CREATE) != UNQLITE_OK)
        {
            SetLastError(PRIVILEGE_ISSUE);
            return false;
        }

        // Read the previous value.
        if (unqlite_kv_fetch(LocalDatabase, &Key, sizeof(Key), &Value, &RecordLength) != UNQLITE_NOTFOUND)
        {
            Value = 0;
        }

        // Increment the value and insert it.
        Value++;
        if (unqlite_kv_store(LocalDatabase, &Key, sizeof(Key), &Value, sizeof(Value)) != UNQLITE_OK)
        {
            unqlite_close(LocalDatabase);
            SetLastError(WRITE_ISSUE);
            return false;
        }

        unqlite_close(LocalDatabase);
        return true;
    }
    bool IncrementRemote(va_list Variadic)
    {
        // Variadic arguments.
        uint64_t Key = va_arg(Variadic, uint64_t);

        return false;
    }
    bool DecrementLocal(va_list Variadic)
    {
        // Variadic arguments.
        uint64_t Key = va_arg(Variadic, uint64_t);
        unqlite *LocalDatabase;
        int64_t Value;
        int64_t RecordLength = sizeof(int64_t);

        // Verify that we have the folder structure needed.
        _mkdir("Ayria");
        _mkdir("Ayria\\Database");

        // Open the local database.
        if (unqlite_open(&LocalDatabase, "Ayria\\Database\\Counter.db", UNQLITE_OPEN_CREATE) != UNQLITE_OK)
        {
            SetLastError(PRIVILEGE_ISSUE);
            return false;
        }

        // Read the previous value.
        if (unqlite_kv_fetch(LocalDatabase, &Key, sizeof(Key), &Value, &RecordLength) != UNQLITE_NOTFOUND)
        {
            Value = 0;
        }

        // Decrement the value and insert it.
        Value--;
        if (unqlite_kv_store(LocalDatabase, &Key, sizeof(Key), &Value, sizeof(Value)) != UNQLITE_OK)
        {
            unqlite_close(LocalDatabase);
            SetLastError(WRITE_ISSUE);
            return false;
        }

        unqlite_close(LocalDatabase);
        return true;
    }
    bool DecrementRemote(va_list Variadic)
    {
        // Variadic arguments.
        uint64_t Key = va_arg(Variadic, uint64_t);

        return false;
    }

    bool WriteLocalTransient(va_list Variadic)
    {
        // Variadic arguments.
        uint64_t Key = va_arg(Variadic, uint64_t);
        char *Value = va_arg(Variadic, char *);
        unqlite *LocalDatabase;

        // Verify that we have the folder structure needed.
        _mkdir("Ayria");
        _mkdir("Ayria\\Database");

        // Open the local database.
        if (unqlite_open(&LocalDatabase, "Ayria\\Database\\Keyvalue.temp", UNQLITE_OPEN_CREATE) != UNQLITE_OK)
        {
            SetLastError(PRIVILEGE_ISSUE);
            return false;
        }

        // Update the value.
        if (unqlite_kv_store(LocalDatabase, &Key, sizeof(Key), Value, 32) != UNQLITE_OK)
        {
            unqlite_close(LocalDatabase);
            SetLastError(WRITE_ISSUE);
            return false;
        }

        unqlite_close(LocalDatabase);
        return true;
    }
    bool WriteLocalPersistent(va_list Variadic)
    {
        // Variadic arguments.
        uint64_t Key = va_arg(Variadic, uint64_t);
        char *Value = va_arg(Variadic, char *);
        unqlite *LocalDatabase;

        // Verify that we have the folder structure needed.
        _mkdir("Ayria");
        _mkdir("Ayria\\Database");

        // Open the local database.
        if (unqlite_open(&LocalDatabase, "Ayria\\Database\\Keyvalue.db", UNQLITE_OPEN_CREATE) != UNQLITE_OK)
        {
            SetLastError(PRIVILEGE_ISSUE);
            return false;
        }

        // Update the value.
        if (unqlite_kv_store(LocalDatabase, &Key, sizeof(Key), Value, 32) != UNQLITE_OK)
        {
            unqlite_close(LocalDatabase);
            SetLastError(WRITE_ISSUE);
            return false;
        }

        unqlite_close(LocalDatabase);
        return true;
    }
    bool WriteRemoteTransient(va_list Variadic)
    {
        // Variadic arguments.
        uint64_t Key = va_arg(Variadic, uint64_t);
        char *Value = va_arg(Variadic, char *);

        return false;
    }
    bool WriteRemotePersistent(va_list Variadic)
    {
        // Variadic arguments.
        uint64_t Key = va_arg(Variadic, uint64_t);
        char *Value = va_arg(Variadic, char *);

        return false;
    }

    bool ReadLocalTransient(va_list Variadic)
    {
        // Variadic arguments.
        uint64_t Key = va_arg(Variadic, uint64_t);
        char *Value = va_arg(Variadic, char *);
        int64_t RecordLength = 32;
        unqlite *LocalDatabase;

        // Verify that we have the folder structure needed.
        _mkdir("Ayria");
        _mkdir("Ayria\\Database");

        // Open the local database.
        if (unqlite_open(&LocalDatabase, "Ayria\\Database\\Keyvalue.temp", UNQLITE_OPEN_CREATE) != UNQLITE_OK)
        {
            SetLastError(PRIVILEGE_ISSUE);
            return false;
        }

        // Update the value.
        if (unqlite_kv_fetch(LocalDatabase, &Key, sizeof(Key), Value, &RecordLength) != UNQLITE_OK)
        {
            unqlite_close(LocalDatabase);
            SetLastError(WRITE_ISSUE);
            return false;
        }

        unqlite_close(LocalDatabase);
        return true;
    }
    bool ReadLocalPersistent(va_list Variadic)
    {
        // Variadic arguments.
        uint64_t Key = va_arg(Variadic, uint64_t);
        char *Value = va_arg(Variadic, char *);
        int64_t RecordLength = 32;
        unqlite *LocalDatabase;

        // Verify that we have the folder structure needed.
        _mkdir("Ayria");
        _mkdir("Ayria\\Database");

        // Open the local database.
        if (unqlite_open(&LocalDatabase, "Ayria\\Database\\Keyvalue.db", UNQLITE_OPEN_CREATE) != UNQLITE_OK)
        {
            SetLastError(PRIVILEGE_ISSUE);
            return false;
        }

        // Update the value.
        if (unqlite_kv_fetch(LocalDatabase, &Key, sizeof(Key), Value, &RecordLength) != UNQLITE_OK)
        {
            unqlite_close(LocalDatabase);
            SetLastError(WRITE_ISSUE);
            return false;
        }

        unqlite_close(LocalDatabase);
        return true;
    }
    bool ReadRemoteTransient(va_list Variadic)
    {
        // Variadic arguments.
        uint64_t Key = va_arg(Variadic, uint64_t);
        char *Value = va_arg(Variadic, char *);

        return false;
    }
    bool ReadRemotePersistent(va_list Variadic)
    {
        // Variadic arguments.
        uint64_t Key = va_arg(Variadic, uint64_t);
        char *Value = va_arg(Variadic, char *);

        return false;
    }
};
