/*
    Initial author: (https://github.com/)Convery
    License: LGPL 3.0
    Started: 2015-10-22
    Notes:
        Exports take a command and any number of arguments.
        The bool result indicates if the request was handled.
*/

#pragma once
#include <stdarg.h>

#ifdef __linux__
    #define EXPORT_ATTR __attribute__((visibility("default")))
    #define IMPORT_ATTR
#elif _WIN32
    #define EXPORT_ATTR __declspec(dllexport)
    #define IMPORT_ATTR __declspec(dllimport)
#else
    #define EXPORT_ATTR
    #define IMPORT_ATTR
    #pragma warning Unknown dynamic link import/export semantics.
#endif

#define EXPORT(ExportName) EXPORT_ATTR bool __cdecl ExportName(size_t Command, ...)

extern "C"
{
    // Social methods handle clans, friends, play history and 
    EXPORT(AyriaSocial);

    // Chat methods handle both p2p and global chats (e.g. IRC).
    EXPORT(AyriaChat);

    // Storage methods handle local, remote and cloud file IO.
    EXPORT(AyriaStorage);

    // Keyvalue methods handle local and remote storage; both persistent and transient.
    EXPORT(AyriaKeyvalue);

    // Lobby methods handle hosting, migration and searching of networked groups.
    EXPORT(AyriaLobby);

    // Counter methods handle incrementation and decrementation of application specific keys.
    EXPORT(AyriaCounter);

    // Client methods handle fetching of user information such as GUID and geolocation.
    EXPORT(AyriaClient);

    // Developer methods handle non-user invoked requests. Such as API initialization, auth and banchecks.
    EXPORT(AyriaDeveloper);

    // Utility methods handle features like compression, encrypton and filemanagement.
    EXPORT(AyriaUtility);
};
