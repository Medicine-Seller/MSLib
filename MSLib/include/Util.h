#pragma once
#include <string>
#include <vector>
#include <Windows.h>

namespace ms
{
	// Description: Replace substrings in string
	// [in] szStr - Original string
	// [in] szFind - Substring to find
	// [in] szReplace - Substring to replace
	// Return: Modified substring
	std::string ReplaceString(std::string szStr, std::string szFind, std::string szReplace);

	// Description: Convert hex string into vector of byte
	// [in] szBytes - String of bytes(hex)
	// Return: Vector of bytes
	std::vector<BYTE> StringToBytes(std::string szBytes);

	// Description: Generate mask from signature
	// [in] szSig - String of signature
	// Return: Vector of char
	std::vector<CHAR> GenerateMask(std::string szSig);


	// Description: Addition by bytes
	// [in] pulAddress - Pointer to address
	// [in] ulOffset - Number of bytes to be added
	// Return: Operated address
	template<typename DataType>
	DataType* IncrementByByte(DataType* pulAddress, intptr_t ulOffset)
	{
		return reinterpret_cast<DataType*>(reinterpret_cast<BYTE*>(pulAddress) + ulOffset);
	}

	// Description: Get address from list of offset by dereference
	// [in] base - Address base
	// [in] offsets - Vector containing offsets
	// Return: Address
	template<typename DataType>
	DataType GetAddress(uintptr_t base, std::vector<uintptr_t> offsets)
	{
		for (unsigned int i = 0; i < offsets.size(); i++)
		{
			if (base < 0xFFFFFF || IsBadReadPtr((uintptr_t*)base, sizeof(uintptr_t)))
			{
				base = 0;
				break;
			}

			if (i == offsets.size() - 1) //If last offset, don't dereference
				base = (uintptr_t)((BYTE*)base + offsets[i]);
			else
				base = *(uintptr_t*)((BYTE*)base + offsets[i]);
		}

		return (DataType)base;
	}
}
