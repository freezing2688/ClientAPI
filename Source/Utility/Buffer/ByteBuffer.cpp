/*
    Initial author: (https://github.com/)Convery
    Started: 2015-10-27
    License: LGPL 3.0
    Notes:
        Simple serialization of datatypes.
*/

#include "Bytebuffer.h"
#include "..\Datatypes\ttmathint.h"

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
    return std::unique_ptr<ReturnType>(new uint8_t[InternalBuffer.size()](InternalBuffer.data()));
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
#define SIMPLE_READ(Type, Enum)   \
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
};

SIMPLE_READ(bool, eBytebufferType::BOOL);
SIMPLE_READ(int8_t, eBytebufferType::SINT8);
SIMPLE_READ(uint8_t, eBytebufferType::UINT8);
SIMPLE_READ(int16_t, eBytebufferType::SINT16);
SIMPLE_READ(uint16_t, eBytebufferType::UINT16);
SIMPLE_READ(int32_t, eBytebufferType::SINT32);
SIMPLE_READ(uint32_t, eBytebufferType::UINT32);
SIMPLE_READ(int64_t, eBytebufferType::SINT64);
SIMPLE_READ(uint64_t, eBytebufferType::UINT64);
SIMPLE_READ(ttmath::Int<TTMATH_BITS(128)>, eBytebufferType::SINT128);
SIMPLE_READ(ttmath::UInt<TTMATH_BITS(128)>, eBytebufferType::UINT128);
SIMPLE_READ(float, eBytebufferType::FLOAT32);
SIMPLE_READ(double, eBytebufferType::FLOAT64);

// Complex specialized read templates.
template <> bool Bytebuffer::TemplateRead(std::string *Buffer, bool Typechecked)
{
    if (!Typechecked || ReadDatatype(eBytebufferType::ASCIISTRING))
    {
        uint32_t StringLength = (uint32_t)strlen((char *)InternalBuffer.data() + InternalPosition) + 1;
        return RawRead(StringLength, Buffer);
    }
    return false;
};
template <> std::string Bytebuffer::TemplateRead(bool Typechecked)
{
    std::string Result{};
    TemplateRead<std::string>(&Result, Typechecked);
    return Result;
};
template <> bool Bytebuffer::TemplateRead(std::wstring *Buffer, bool Typechecked)
{
    if (!Typechecked || ReadDatatype(eBytebufferType::UNICODESTRING))
    {
        uint32_t StringLength = (uint32_t)wcslen((wchar_t *)InternalBuffer.data() + InternalPosition) + 1;
        return RawRead(StringLength * 2, Buffer);
    }
    return false;
};
template <> std::wstring Bytebuffer::TemplateRead(bool Typechecked)
{
    std::wstring Result{};
    TemplateRead<std::wstring>(&Result, Typechecked);
    return Result;
};
template <> bool Bytebuffer::TemplateRead(std::basic_string<uint8_t> *Buffer, bool Typechecked)
{
    if (!Typechecked || ReadDatatype(eBytebufferType::BLOB))
    {
        uint32_t BlobLength = TemplateRead<uint32_t>();

        // Range check.
        if (InternalPosition + BlobLength > InternalBuffer.size())
        {
            //fDebugPrint("%s, blob (%u) is larger than the buffer. Check your endians.", __func__, BlobLength);
            return false;
        }

        // Read byte by byte.
        for (uint32_t i = 0; i < BlobLength; ++i)
            Buffer->push_back(TemplateRead<uint8_t>(false));

        return true;
    }
    return false;
};
template <> std::basic_string<uint8_t> Bytebuffer::TemplateRead(bool Typechecked)
{
    std::basic_string<uint8_t> Result{};
    TemplateRead<std::basic_string<uint8_t>>(&Result, Typechecked);
    return Result;
};

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
