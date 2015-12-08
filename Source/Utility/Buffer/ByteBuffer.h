/*
    Initial author: (https://github.com/)Convery
    Started: 2015-10-27
    License: LGPL 3.0
    Notes:
        Simple serialization of datatypes.
*/

#pragma once
#include <string>
#include <memory>

// Datatypes handled by this class.
enum class eBytebufferType : uint8_t
{
    NONE = 0,
    BOOL = 1,
    SINT8 = 2,
    UINT8 = 3,
    SINT16 = 4,
    UINT16 = 5,
    SINT32 = 6,
    UINT32 = 7,
    SINT64 = 8,
    UINT64 = 9,
#ifndef NO_TTMATH
    SINT128 = 10,
    UINT128 = 11,
#endif
    FLOAT32 = 12,
    FLOAT64 = 13,
    ASCIISTRING = 14,
    UNICODESTRING = 15,
    BLOB = 16,
    EOS = 17,
    MAX
};

/*
    The buffer will expand as needed, but will not shrink.
    The position property indicates where the next read/write.
*/
class Bytebuffer
{
    std::basic_string<uint8_t> InternalBuffer;
    uint32_t InternalPosition;

public:
    // Constructors.
    Bytebuffer(uint32_t InputLength, void *InputData = nullptr);
    Bytebuffer(std::basic_string<uint8_t> *InputData);
    Bytebuffer(std::string *InputData);
    Bytebuffer();
    ~Bytebuffer();

    // Property access.
    uint32_t GetSize();                         // Get the buffer length.
    uint32_t GetLength();                       // Get the buffer length.
    uint32_t GetPosition();                     // Return BufferPosition.
    bool SetPosition(uint32_t Pos);             // Set BufferPosition, returns true if Pos < Buffer.size.
    template <typename ReturnType>              // Return a pointer to a new buffer,
    std::unique_ptr<ReturnType> *GetBuffer();   // with the same content as the old one.

    // Utility methods.
    uint8_t PeekByte();                         // Return the next byte, useful to check for a datatype.
    void Rewind();                              // Sets the position to 0.
    void Clear();                               // Clears and deallocates all memory and calls Rewind.

    // Core functionallity.
    bool ReadDatatype(eBytebufferType Type);
    bool WriteDatatype(eBytebufferType Type);
    bool RawRead(uint32_t ReadCount, void *OutBuffer = nullptr);
    bool RawWrite(uint32_t WriteCount, void *InBuffer = nullptr);

    // Simple storagetypes.
    template <typename Type> Type TemplateRead(bool Typechecked = true);
    template <typename Type> bool TemplateRead(Type *Buffer, bool Typechecked = true);
    template <typename Type> bool TemplateWrite(Type Value, bool Typechecked = true);

    // Advanced storagetypes.
    bool ReadBlob(std::string *Output, bool Typechecked = true);
    bool ReadBlob(std::basic_string<uint8_t> *Output, bool Typechecked = true);
    bool ReadBlob(uint32_t DataLength, void *DataBuffer, bool Typechecked = true);
    std::string ReadBlob(bool Typechecked = true);
    bool ReadString(std::string &Output, bool Typechecked = true);
    std::string ReadString(bool Typechecked = true);
    bool WriteBlob(std::string *Input, bool Typechecked = true);
    bool WriteBlob(std::basic_string<uint8_t> *Input, bool Typechecked = true);
    bool WriteBlob(uint32_t DataLength, void *DataBuffer, bool Typechecked = true);
    bool WriteString(std::string &Input, bool Typechecked = true);
};
