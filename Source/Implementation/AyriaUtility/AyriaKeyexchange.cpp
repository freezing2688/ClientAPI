/*
	Initial author: (https://github.com/)Convery
	License: LGPL 3.0
	Started: 2015-10-28
	Notes:
		API implementation of keyexchange.
*/

#include <stdint.h>
#include <stdarg.h>
#include <Windows.h>
#include "..\..\Utility\Crypto\SM3.h"
#include "..\..\Utility\Datatypes\ttmathbig.h"

#define EXCHANGE_OUTOF_ORDER 34
#define EXCHANGE_INVALID 43

// The hedgehogscience keyexchange.
enum eHHSStatus : uint32_t
{
	NONE = 0,
	BASESHARED = 1,
	SECRETSHARED = 2,
	HELLO = 3,
	DONE = 4,
};
struct HHSKeyState
{
	eHHSStatus Status{ eHHSStatus::NONE };
	ttmath::UInt<TTMATH_BITS(1024)> Exponent;
	ttmath::UInt<TTMATH_BITS(2048)> Base;
	ttmath::Big<TTMATH_BITS(1024), TTMATH_BITS(2048)> Secret;
	uint8_t EncryptionKey[32]{};
};

namespace APIImplentation
{
	HHSKeyState HHSState;

	bool HHSStart(va_list Variadic)
	{
		// Variadic arguments.
		char *Request = va_arg(Variadic, char *);
		char *Response = va_arg(Variadic, char *);
		std::string TempString;

		// Clear the state.
		HHSState.Status = eHHSStatus::NONE;
		HHSState.Exponent = 0;
		HHSState.Base = 0;
		HHSState.Secret = 0;

		// Create a random string for the exponent.
		for (uint32_t i = 0; i < 255; ++i)
			TempString.push_back(((8253729 * i + 2396403) % time(NULL)) % 9 + 0x30);
		HHSState.Exponent.FromString(TempString, 10);

		// Create a random string for the base.
		TempString.clear();
		for (uint32_t i = 0; i < 255; ++i)
			TempString.push_back(((8253729 * time(NULL) + 2396403) % i * 9245232) % 9 + 0x30);
		HHSState.Base.FromString(TempString, 10);

		// Check if peer mode.
		if (Request != nullptr)
		{
			if (*(eHHSStatus *)Request != eHHSStatus::BASESHARED)
			{
				SetLastError(EXCHANGE_OUTOF_ORDER);
				return false;
			}

			// Return the users prefered base.
			HHSState.Status = eHHSStatus::BASESHARED;
			memcpy(Response, &HHSState.Status, sizeof(HHSState.Status));
			strcpy_s(Response + sizeof(HHSState.Status), 256, HHSState.Base.ToString().c_str());

			// As the peer have already selected one, create the shared base.
			HHSState.Base *= ttmath::UInt<TTMATH_BITS(2048)>(std::string(Request + sizeof(HHSState.Status)));
		}
		else
		{
			// Return the users prefered base.
			HHSState.Status = eHHSStatus::BASESHARED;
			memcpy(Response, &HHSState.Status, sizeof(HHSState.Status));
			strcpy_s(Response + sizeof(HHSState.Status), 256, HHSState.Base.ToString().c_str());
		}

		return true;
	}
	bool HHSContinue(va_list Variadic)
	{
		// Variadic arguments.
		char *Request = va_arg(Variadic, char *);
		char *Response = va_arg(Variadic, char *);

		// Temp vars.
		std::string Temp;
		PIMAGE_NT_HEADERS NTHeader;
		uint32_t dwEntryPoint;
		uint32_t nSectionCount;
		PIMAGE_SECTION_HEADER pSectionHeader;
		uint8_t VersionInfo[32]{};
	
		if (Request == nullptr)
		{
			// Return the users prefered base.
			HHSState.Status = eHHSStatus::BASESHARED;
			memcpy(Response, &HHSState.Status, sizeof(HHSState.Status));
			strcpy_s(Response + sizeof(HHSState.Status), 256, HHSState.Base.ToString().c_str());

			return true;
		}

		switch (*(eHHSStatus *)Request)
		{
		case eHHSStatus::NONE:
			SetLastError(EXCHANGE_INVALID); 
			break;

		case eHHSStatus::BASESHARED:
			// Create the shared base from the peers base.
			HHSState.Base *= ttmath::UInt<TTMATH_BITS(2048)>(std::string(Request + sizeof(HHSState.Status)));

			// Create the secret from the shared base and the users exponent.
			HHSState.Secret = HHSState.Base;
			HHSState.Secret.Pow(HHSState.Exponent);

			// Return the users secret.
			HHSState.Status = eHHSStatus::SECRETSHARED;
			memcpy(Response, &HHSState.Status, sizeof(HHSState.Status));
			strcpy_s(Response + sizeof(HHSState.Status), 256, HHSState.Secret.ToString().c_str());
			break;

		case eHHSStatus::SECRETSHARED:
			// Finalize the secret generation.
			HHSState.Secret.Pow(ttmath::Big<TTMATH_BITS(1024), TTMATH_BITS(2048)>(std::string(Request + sizeof(HHSState.Status))));

			// Create the encryptionkey.
			Temp = HHSState.Secret.ToString();
			sm3((const unsigned char *)Temp.data(), Temp.size(), HHSState.EncryptionKey);

			// Create the version info.
			NTHeader = (PIMAGE_NT_HEADERS)((DWORD_PTR)GetModuleHandleA(NULL) + ((PIMAGE_DOS_HEADER)GetModuleHandleA(NULL))->e_lfanew);
			dwEntryPoint = NTHeader->OptionalHeader.AddressOfEntryPoint;
			nSectionCount = NTHeader->FileHeader.NumberOfSections;
			pSectionHeader = reinterpret_cast<PIMAGE_SECTION_HEADER>((PBYTE)&NTHeader->OptionalHeader + NTHeader->FileHeader.SizeOfOptionalHeader);

			for (uint32_t i = 0; i < nSectionCount; ++i)
			{
				if (pSectionHeader->VirtualAddress <= dwEntryPoint && dwEntryPoint < pSectionHeader->VirtualAddress + pSectionHeader->Misc.VirtualSize)
					break;
				pSectionHeader++;
			}
			sm3((((PBYTE)GetModuleHandleA(NULL)) + (SIZE_T)((pSectionHeader->VirtualAddress))), pSectionHeader->Misc.VirtualSize, VersionInfo);

			// Return a hello message.
			HHSState.Status = eHHSStatus::HELLO;
			memcpy(Response, &HHSState.Status, sizeof(HHSState.Status));
			memcpy(Response + sizeof(HHSState.Status), VersionInfo, 32);
			break;

		case eHHSStatus::HELLO:
			// The client doesn't care for which version the server is running.
			HHSState.Status = eHHSStatus::DONE;
			break;

		default:
			SetLastError(EXCHANGE_INVALID);
			break;
		}

		return true;
	}
};
