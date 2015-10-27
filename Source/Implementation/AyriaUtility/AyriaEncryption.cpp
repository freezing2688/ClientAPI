/*
    Initial author: (https://github.com/)Convery
    License: LGPL 3.0
    Started: 2015-10-26
    Notes:
        API implementation of encryption.
*/

#include <stdint.h>
#include <stdarg.h>
#include "..\..\Utility\Crypto\AES256.h"
#include "..\..\Utility\Crypto\McbDES2.hpp"

namespace APIImplentation
{
    bool AES256Encrypt(va_list Variadic)
    {
        // Variadic arguments.
        char *Plaintext = va_arg(Variadic, char *);
        char *Ciphertext = va_arg(Variadic, char *);
        char *InitializationVector = va_arg(Variadic, char *);
        char *Encryptionkey = va_arg(Variadic, char *);
        uint32_t Length = va_arg(Variadic, uint32_t);

        ByteArray Encrypted;
        Aes256 aes((unsigned char *)Encryptionkey, (unsigned char *)InitializationVector);
        aes.encrypt_start(Length, Encrypted);
        aes.encrypt_continue((const unsigned char *)Plaintext, Length, Encrypted);
        aes.encrypt_end(Encrypted);

        // Copy to the pre-allocated buffer.
        if (Encrypted.size())
            memcpy(Ciphertext, Encrypted.data(), Length);

        return true;
    }
    bool AES256Decrypt(va_list Variadic)
    {
        // Variadic arguments.
        char *Plaintext = va_arg(Variadic, char *);
        char *Ciphertext = va_arg(Variadic, char *);
        char *InitializationVector = va_arg(Variadic, char *);
        char *Encryptionkey = va_arg(Variadic, char *);
        uint32_t Length = va_arg(Variadic, uint32_t);

        ByteArray Decrypted;
        Aes256 aes((unsigned char *)Encryptionkey, (unsigned char *)InitializationVector);
        aes.decrypt_start(Length);
        aes.decrypt_continue((const unsigned char *)Ciphertext, Length, Decrypted);
        aes.decrypt_end(Decrypted);

        // Copy to the pre-allocated buffer.
        if (Decrypted.size())
            memcpy(Plaintext, Decrypted.data(), Length);

        return true;
    }
    
    bool DES3Encrypt(va_list Variadic)
    {
        char *Plaintext = va_arg(Variadic, char *);
        char *Ciphertext = va_arg(Variadic, char *);
        unsigned char *lpKey1 = va_arg(Variadic, unsigned char *);
        unsigned char *lpKey2 = va_arg(Variadic, unsigned char *);
        uint32_t Length = va_arg(Variadic, uint32_t);

        McbDES desEncrypt;
        
        desEncrypt.McbSetKey1(lpKey1);
        desEncrypt.McbSetKey2(lpKey2);
        
        if (desEncrypt.McbEncrypt(Plaintext))
        {
            memcpy(Ciphertext, desEncrypt.McbGetCryptogram(), desEncrypt.McbGetCryptogramSize());
            return true;
        }
        return false;
    }
    bool DES3Decrypt(va_list Variadic)
    {
         char *Plaintext = va_arg(Variadic, char *);
         unsigned char *Ciphertext = va_arg(Variadic, unsigned char *);
         unsigned char *lpKey1 = va_arg(Variadic, unsigned char *);
         unsigned char *lpKey2 = va_arg(Variadic, unsigned char *);
         uint32_t Length = va_arg(Variadic, uint32_t);

         McbDES desDecrypt;
         
         desDecrypt.McbSetKey1(lpKey1);
         desDecrypt.McbSetKey2(lpKey2);
         
         if(desDecrypt.McbDecrypt(Ciphertext, Length)){
             memcpy(Plaintext, desDecrypt.McbGetPlainText(), desDecrypt.McbGetPlainTextSize());
             return true;
         }
         return false;
     }
};
