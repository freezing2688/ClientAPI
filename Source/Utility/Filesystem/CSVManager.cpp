/*
    Initial author: (https://github.com/)Convery
    License: LGPL 3.0
    Started: 2015-10-26
    Notes:
        CSV reading and writing.
*/

#include <stdio.h>
#include "CSVManager.h"

bool CSVManager::BeginRead(const char *Filename)
{
    Buffer.clear();
    Buffer.shrink_to_fit();

    return fopen_s(&Filehandle, Filename, "rb") == 0;
}
bool CSVManager::ReadNext(std::string &NextItem)
{
    return false;
}
bool CSVManager::SkipLine()
{
    return false;
}

bool CSVManager::Insert(int32_t Row, int32_t Col, std::string Entry)
{
    // Insert rows until we have the insert row number.
    if (Row > 0)
        for (size_t i = Buffer.size(); i < (unsigned)Row + 1; ++i)
            Buffer.push_back(std::vector<std::string>());
    else
        Row = Buffer.size() > 0 ? Buffer.size() - 1 : 0;

    // Insert cols until we have the insert col number.
    if (Col > 0)
        for (size_t i = Buffer[Row].size(); i < (unsigned)Col + 1; ++i)
            Buffer[Row].push_back("");
    else
        Col = Buffer[Row].size() > 0 ? Buffer[Row].size() - 1 : 0;

    // Insert the entry.
    Buffer[Row][Col] = Entry;
    return true;
}
bool CSVManager::WriteFile(const char *Filename)
{
    return false;
}
bool CSVManager::CloseFile()
{
    return fclose(Filehandle) == 0;
}
