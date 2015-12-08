/*
    Initial author: (https://github.com/)Convery
    Started: 2015-10-27
    License: LGPL 3.0
    Notes:
        Simple serialization of datatypes.
*/

#include <algorithm>
#include "Bytebuffer.h"
#include "..\..\Macros.h"
#ifndef NO_TTMATH
#include "..\Datatypes\ttmathint.h"
#endif

// Constructors.
#pragma optimize( "", off )
Bytebuffer::Bytebuffer(uint32_t InputLength, void *InputData)
{
    if (InputData != nullptr)
        InternalBuffer.append((uint8_t *)InputData, InputLength);
    else
        InternalBuffer.reserve(InputLength);

    InternalPosition = 0;
}
Bytebuffer::Bytebuffer(std::basic_string<uint8_t> *InputData)
{
    InternalBuffer = *InputData;
    InternalPosition = 0;
}
Bytebuffer::Bytebuffer(std::string *InputData)
{
    InternalBuffer.append((uint8_t *)InputData->data(), InputData->length());
    InternalPosition = 0;
}
Bytebuffer::Bytebuffer()
{
    InternalPosition = 0;
}
Bytebuffer::~Bytebuffer()
{
    // Clear the memory and deallocate.
    std::fill(InternalBuffer.begin(), InternalBuffer.end(), 0xCC);
    InternalBuffer.clear();
    InternalBuffer.shrink_to_fit();
    InternalPosition = 0;
}
#pragma optimize( "", on )

// Property access.
uint32_t Bytebuffer::GetSize()
{
    return (uint32_t)InternalBuffer.size();
}
uint32_t Bytebuffer::GetLength()
{
    return (uint32_t)InternalBuffer.size();
}
uint32_t Bytebuffer::GetPosition()
{
    return InternalPosition;
}
bool Bytebuffer::SetPosition(uint32_t Pos)
{
    if (Pos < InternalBuffer.size())
        InternalPosition = Pos;
    else
        return false;

    return true;
}
template <typename ReturnType>
std::unique_ptr<ReturnType> *Bytebuffer::GetBuffer()
{
    ReturnType *Buffer = (ReturnType *)new char[InternalBuffer.size()]();
    memcpy(Buffer, InternalBuffer.data(), InternalBuffer.size());

    return new std::unique_ptr<ReturnType>(Buffer);
}

// Core functionallity.
bool Bytebuffer::ReadDatatype(eBytebufferType Type)
{
    if (PeekByte() == (uint8_t)Type)
        SetPosition(GetPosition() + 1);
    else
        return false;

    return true;
}
bool Bytebuffer::WriteDatatype(eBytebufferType Type)
{
    return RawWrite(1, &Type);
}
bool Bytebuffer::RawRead(uint32_t ReadCount, void *OutBuffer)
{
    // Range check.
    if (InternalPosition + ReadCount > InternalBuffer.size())
    {
        //fDebugPrint("%s tried to read out of bounds.", __func__);
        return false;
    }

    // If we have a buffer, simply copy to it.
    if (OutBuffer != nullptr)
        memcpy(OutBuffer, InternalBuffer.data() + InternalPosition, ReadCount);

    // Increment the position and return.
    InternalPosition += ReadCount;
    return true;
}
bool Bytebuffer::RawWrite(uint32_t WriteCount, void *InBuffer)
{
    // If we don't have a buffer, increment and leave.
    if (InBuffer == nullptr)
    {
        InternalPosition += WriteCount;
        return true;
    }

    // If we are at the end of the stream, append.
    if (InternalPosition == InternalBuffer.size())
    {
        InternalBuffer.append((const uint8_t *)InBuffer, WriteCount);
        InternalPosition += WriteCount;
        return true;
    }

    // Range check.
    if (InternalPosition > InternalBuffer.size())
    {
        //fDebugPrint("%s tried to write out of bounds.", __func__);
        return false;
    }

    // If we can fit the data in already allocated memory, copy.
    if (InternalPosition + WriteCount < InternalBuffer.size())
    {
        for (uint32_t i = 0; i < WriteCount; ++i)
            InternalBuffer[i] = ((const uint8_t *)InBuffer)[i];

        InternalPosition += WriteCount;
        return true;
    }

    // Write the data as preallocated and appended.
    uint32_t AllocatedBytes = (uint32_t)InternalBuffer.size() - InternalPosition;
    return RawWrite(AllocatedBytes, InBuffer)
        && RawWrite(WriteCount - AllocatedBytes, (uint8_t *)InBuffer + AllocatedBytes);
}

// Simple specialized read templates.
#define SIMPLE_TEMPLATE(Type, Enum)   \
template <> bool Bytebuffer::TemplateRead(Type *Buffer, bool Typechecked) \
{   \
    if (!Typechecked || ReadDatatype(Enum))    \
        return RawRead(sizeof(*Buffer), Buffer);    \
    return false;   \
};  \
template <> Type Bytebuffer::TemplateRead(bool Typechecked) \
{   \
    Type Result{};   \
    TemplateRead<Type>(&Result, Typechecked);   \
    return Result;  \
}; \
template <> bool Bytebuffer::TemplateWrite(Type Buffer, bool Typechecked) \
{   \
    if (Typechecked)    \
        WriteDatatype(Enum); \
    return RawWrite(sizeof(Buffer), &Buffer);    \
};

SIMPLE_TEMPLATE(bool, eBytebufferType::BOOL);
SIMPLE_TEMPLATE(char, eBytebufferType::SINT8);
SIMPLE_TEMPLATE(int8_t, eBytebufferType::SINT8);
SIMPLE_TEMPLATE(uint8_t, eBytebufferType::UINT8);
SIMPLE_TEMPLATE(int16_t, eBytebufferType::SINT16);
SIMPLE_TEMPLATE(uint16_t, eBytebufferType::UINT16);
SIMPLE_TEMPLATE(int32_t, eBytebufferType::SINT32);
SIMPLE_TEMPLATE(uint32_t, eBytebufferType::UINT32);
SIMPLE_TEMPLATE(int64_t, eBytebufferType::SINT64);
SIMPLE_TEMPLATE(uint64_t, eBytebufferType::UINT64);
#ifndef NO_TTMATH
SIMPLE_TEMPLATE(ttmath::Int<TTMATH_BITS(128)>, eBytebufferType::SINT128);
SIMPLE_TEMPLATE(ttmath::UInt<TTMATH_BITS(128)>, eBytebufferType::UINT128);
#endif
SIMPLE_TEMPLATE(float, eBytebufferType::FLOAT32);
SIMPLE_TEMPLATE(double, eBytebufferType::FLOAT64);

// Advanced storagetypes.
bool Bytebuffer::ReadBlob(std::string *Output, bool Typechecked)
{
    if (!Typechecked || ReadDatatype(eBytebufferType::BLOB))
    {
        uint32_t BlobLength = TemplateRead<uint32_t>();

        // Range check.
        if (InternalPosition + BlobLength > InternalBuffer.size())
        {
            DebugPrint(va("%s, blob (%u) is larger than the buffer. Check your endians.", __func__, BlobLength));
            return false;
        }

        // Read byte by byte.
        for (uint32_t i = 0; i < BlobLength; ++i)
            Output->push_back(TemplateRead<uint8_t>(false));

        return true;
    }
    return false;
}
bool Bytebuffer::ReadBlob(std::basic_string<uint8_t> *Output, bool Typechecked)
{
    if (!Typechecked || ReadDatatype(eBytebufferType::BLOB))
    {
        uint32_t BlobLength = TemplateRead<uint32_t>();

        // Range check.
        if (InternalPosition + BlobLength > InternalBuffer.size())
        {
            DebugPrint(va("%s, blob (%u) is larger than the buffer. Check your endians.", __func__, BlobLength));
            return false;
        }

        // Read byte by byte.
        for (uint32_t i = 0; i < BlobLength; ++i)
            Output->push_back(TemplateRead<uint8_t>(false));

        return true;
    }
    return false;
}
bool Bytebuffer::ReadBlob(uint32_t DataLength, void *DataBuffer, bool Typechecked)
{
    if (!Typechecked || ReadDatatype(eBytebufferType::BLOB))
    {
        uint32_t BlobLength = TemplateRead<uint32_t>();

        // Range check.
        if (InternalPosition + BlobLength > InternalBuffer.size())
        {
            DebugPrint(va("%s, blob (%u) is larger than the buffer. Check your endians.", __func__, BlobLength));
            return false;
        }

        // Read byte by byte.
        for (uint32_t i = 0; i < std::min(BlobLength, DataLength); ++i)
            ((uint8_t *)DataBuffer)[i] = TemplateRead<uint8_t>(false);

        return true;
    }
    return false;
}
std::string Bytebuffer::ReadBlob(bool Typechecked)
{
    std::string Result;

    if (!Typechecked || ReadDatatype(eBytebufferType::BLOB))
    {
        uint32_t BlobLength = TemplateRead<uint32_t>();

        // Range check.
        if (InternalPosition + BlobLength > InternalBuffer.size())
        {
            DebugPrint(va("%s, blob (%u) is larger than the buffer. Check your endians.", __func__, BlobLength));
            return false;
        }

        // Read byte by byte.
        for (uint32_t i = 0; i < BlobLength; ++i)
            Result.push_back(TemplateRead<uint8_t>(false));

    }
    return Result;
}
bool Bytebuffer::ReadString(std::string &Output, bool Typechecked)
{
    if (!Typechecked || ReadDatatype(eBytebufferType::ASCIISTRING))
    {
        uint32_t StringLength = (uint32_t)strlen((char *)InternalBuffer.data() + InternalPosition) + 1;
        Output.append((char *)InternalBuffer.data() + InternalPosition);
        return RawRead(StringLength);
    }
    return false;
}
std::string Bytebuffer::ReadString(bool Typechecked)
{
    std::string Result;

    if (!Typechecked || ReadDatatype(eBytebufferType::ASCIISTRING))
    {
        uint32_t StringLength = (uint32_t)strlen((char *)InternalBuffer.data() + InternalPosition) + 1;
        Result.append((char *)InternalBuffer.data() + InternalPosition);
        RawRead(StringLength);
    }

    return Result;
}
bool Bytebuffer::WriteBlob(std::string *Input, bool Typechecked)
{
    if (Typechecked)
        WriteDatatype(eBytebufferType::BLOB);

    TemplateWrite<uint32_t>((uint32_t)Input->size());
    return RawWrite((uint32_t)Input->size(), (void *)Input->data());
}
bool Bytebuffer::WriteBlob(std::basic_string<uint8_t> *Input, bool Typechecked)
{
    if (Typechecked)
        WriteDatatype(eBytebufferType::BLOB);

    TemplateWrite<uint32_t>((uint32_t)Input->size());
    return RawWrite((uint32_t)Input->size(), (void *)Input->data());
}
bool Bytebuffer::WriteBlob(uint32_t DataLength, void *DataBuffer, bool Typechecked)
{
    if (Typechecked)
        WriteDatatype(eBytebufferType::BLOB);

    TemplateWrite(DataLength);
    return RawWrite(DataLength, DataBuffer);
}
bool Bytebuffer::WriteString(std::string &Input, bool Typechecked)
{
    if (Typechecked)
        WriteDatatype(eBytebufferType::ASCIISTRING);

    return RawWrite((uint32_t)Input.size(), (void *)Input.c_str());
}

// Utility methods.
uint8_t Bytebuffer::PeekByte()
{
    uint8_t Byte = 0;
    if (TemplateRead(&Byte, false))
        SetPosition(GetPosition() - 1);

    return Byte;
}
void Bytebuffer::Rewind()
{
    InternalPosition = 0;
}
void Bytebuffer::Clear()
{
    // Clear the memory and deallocate.
    std::fill(InternalBuffer.begin(), InternalBuffer.end(), 0xCC);
    InternalBuffer.clear();
    InternalBuffer.shrink_to_fit();
    InternalPosition = 0;
}
