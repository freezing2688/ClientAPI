/*
    Initial author: (https://github.com/)Convery
    License: LGPL 3.0
    Started: 2015-10-25
    Notes:
        Requested utilities to save space in other modules.
*/

#pragma warning (disable:4307)
#include "..\Utility\Crypto\FNV1a.h"
#include "ClientAPI.h"

// More readable definition of the method.
#define EXPORTMETHOD(Command, Functor)      \
    case FNV1a_Compiletime(Command):        \
        Result = Functor(Variadic);         \
        break

// Implementations.
namespace APIImplentation
{

    /**
     Decompress an lz4 buffer
     
     @param Variadic a variadic argument containing the parameters to the function. Expected args are char *Plaintext, char **OutputBuffer, uint32_t PlaintextLength, int32_t *NumberOfBytesChanged.
     @return True if the decompression was handled. NOTE: if decompression fails NumberOfBytesChanged will be < 0.
     */
    extern bool LZ4Inflate(va_list Variadic);
    /**
     Compress an lz4 buffer
     
     @param Variadic a variadic argument containing the parameters to the function. Expected args are char *Plaintext, char **OutputBuffer, uint32_t PlaintextLength, int32_t *NumberOfBytesChanged.
     @return True if the decompression was handled. NOTE: if decompression fails NumberOfBytesChanged will be < 0.
     */
    extern bool LZ4Deflate(va_list Variadic);

    /**
     Encrypt a plaintext string using the AES256 encryption algorithm
     
     @param Variadic a variadic argument containing the parameters to the function. Expected args are char *Plaintext, char *Ciphertext, char *InitializationVector, char *Encryptionkey, uint32_t Length.
     @return True if the encryption was handled.
     */
    extern bool AES256Encrypt(va_list Variadic);
    /**
     Decrypt a plaintext string using the AES256 encryption algorithm
     
     @param Variadic a variadic argument containing the parameters to the function. Expected args are char *Plaintext, char *Ciphertext, char *InitializationVector, char *Encryptionkey, uint32_t Length.
     @return True if the decryption was handled.
     */
    extern bool AES256Decrypt(va_list Variadic);
    /**
     Encrypt a plaintext string using the DES3 encryption algorithm
     
     @param Variadic a variadic argument containing the parameters to the function. Expected args are char *Plaintext ,char *Ciphertext,unsigned char *lpKey1,unsigned char *lpKey2,uint32_t Length.
     @return True if the encryption was successful, false otherwise. If encryption was successful Ciphertext will contain the encrypted ciphertext.
     */
    extern bool DES3Encrypt(va_list Variadic);
     /**
     Decrypt a plaintext string using the DES3 encryption algorithm
     
     @param Variadic a variadic argument containing the parameters to the function. Expected args are char *Plaintext ,unsigned char *Ciphertext,unsigned char *lpKey1,unsigned char *lpKey2,uint32_t Length.
     @return True if the decryption was successful, false otherwise. If decryption was successful Plaintext will contain the decrypted plaintext.
     */
    extern bool DES3Decrypt(va_list Variadic);
    /**
     Hash a plaintext string using the FNV1 hashing algorithm
     
     @param Variadic a variadic argument containing the parameters to the function. Expected args are char *Plaintext, uint32_t PlainLength, char *Result.
     @return True if the hashing was handled. If hashing was successful Result will contain the hash.
     */
    extern bool FNV1aHash_64(va_list Variadic);
    /**
     Hash a plaintext string using the SM3 hashing algorithm
     
     @param Variadic a variadic argument containing the parameters to the function. Expected args are char *Plaintext, uint32_t PlainLength, char *Result.
     @return True if the hashing was handled. If hashing was successful Result will contain the hash.
     */
    extern bool SM3Hash_192(va_list Variadic);
};

bool __cdecl AyriaUtility(size_t Command, ...)
{
    bool Result = false;
    va_list Variadic;
    va_start(Variadic, Command);

    switch (Command)
    {
        // Encryption; takes a char *Plaintext, char *Ciphertext, char *IV, char *Key, uint32_t Length.
        EXPORTMETHOD("AES256Encrypt", APIImplentation::AES256Encrypt);
        EXPORTMETHOD("AES256Decrypt", APIImplentation::AES256Decrypt);
        EXPORTMETHOD("DES3Encrypt", APIImplentation::DES3Encrypt);
        EXPORTMETHOD("DES3Decrypt", APIImplentation::DES3Decrypt);
        EXPORTMETHOD("TEAEncrypt", [](va_list Placeholder) { return false; });
        EXPORTMETHOD("TEADecrypt", [](va_list Placeholder) { return false; });
        EXPORTMETHOD("RC6Encrypt", [](va_list Placeholder) { return false; });
        EXPORTMETHOD("RC6Decrypt", [](va_list Placeholder) { return false; });

        // Keyexchange; takes a char *Request (when peer) and a char *Response.
        EXPORTMETHOD("HHSStart", [](va_list Placeholder) { return false; });
        EXPORTMETHOD("HHSContinue", [](va_list Placeholder) { return false; });
        EXPORTMETHOD("RSAStart", [](va_list Placeholder) { return false; });
        EXPORTMETHOD("RSAContinue", [](va_list Placeholder) { return false; });

        // Hashing; takes a char *Plaintext, uint32_t PlainLength, char *Result.
        EXPORTMETHOD("FNV1aHash_64", APIImplentation::FNV1aHash_64);
        EXPORTMETHOD("SM3Hash_192", APIImplentation::SM3Hash_192);
        EXPORTMETHOD("SHA3Hash_256", [](va_list Placeholder) { return false; });

        // File readers; takes a char *Data.
        EXPORTMETHOD("CSVLoad", [](va_list Placeholder) { return false; });
        EXPORTMETHOD("CSVReadNext", [](va_list Placeholder) { return false; });
        EXPORTMETHOD("CSVSkipLine", [](va_list Placeholder) { return false; });

        // Compression; takes a char *Plaintext, char **ModifiedBuffer, uint32_t PlainLength, int32_t *ModifiedLength.
        EXPORTMETHOD("LZ4Inflate", APIImplentation::LZ4Inflate);
        EXPORTMETHOD("LZ4Deflate", APIImplentation::LZ4Deflate);
    }

    va_end(Variadic);
    return Result;
}
