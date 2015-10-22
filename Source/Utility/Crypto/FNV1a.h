/*
    Initial author: (https://github.com/)Convery
    License: LGPL 3.0
    Started: 2015-10-22
    Notes:
        Compiletime hashing of strings and runtime hashing of data.
*/

#pragma once

// FNV1 constants.
constexpr size_t FNV1_Prime = (sizeof(void *) == 8 ? 1099511628211u : 16777619u);
constexpr size_t FNV1_Offset = (sizeof(void *) == 8 ? 14695981039346656037u : 2166136261u);

// FNV1a implementation.
constexpr size_t FNV1a_Compiletime(const char *String, size_t LastValue = FNV1_Offset)
{
    return *String ? FNV1a_Compiletime(String + 1, (*String ^ LastValue) * FNV1_Prime) : LastValue;
};
inline size_t FNV1a_Runtime(char *Data, size_t Length)
{
    size_t Hash = FNV1_Offset;

    for (size_t i = 0; i < Length; ++i)
    {
        Hash ^= Data[i];
        Hash *= FNV1_Prime;
    }

    return Hash;
};
