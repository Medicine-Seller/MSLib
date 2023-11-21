#include <string>
#include <vector>
#include <Windows.h>
#include <Psapi.h>
#include "Constants.h"
#include <sstream>
#include "Macros.h"

#ifndef UTIL_H
#define UTIL_H

namespace ms
{
	// Description: Replace substrings in string
	// [in] str - Original string
	// [in] findString - Substring to find
	// [in] replaceString - Substring to replace
	// Return: Modified substring
	std::string ReplaceString(std::string str, std::string findString, std::string replaceString);

	// Description: Convert hex string into vector of byte
	// [in] bytesString - String of bytes(hex)
	// Return: Vector of bytes
	std::vector<BYTE> StringToBytes(std::string bytesString);

	// Description: Generate mask from signature
	// [in] signature - String of signature
	// Return: Vector of char
	std::vector<CHAR> GenerateMask(std::string signature);

	// Description: Addition by bytes
	// [in] address - Pointer to address
	// [in] offset - Number of bytes to be added
	// Return: Operated address
	template<typename DataType>
	DataType IncrementByByte(DataType address, LONG offset)
	{
		return reinterpret_cast<DataType>(reinterpret_cast<BYTE*>(address) + offset);
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

	template <typename DataType = ULONG>
	DataType GetConstant(const std::vector<ConstantString>& constantStrings, std::string str)
	{
		for (auto& e : constantStrings)
			if (e.StringValue == str)
				return static_cast<DataType>(e.Value);

		return -1;
	}

	template<typename DataType>
	std::string GetConstantString(const std::vector<ConstantString>& constantStrings, DataType value)
	{
		ULONGLONG constantValue = static_cast<ULONGLONG>(value);
		constantValue <= 32;
		constantValue >= 32;

		for (auto& e : constantStrings)
		{
			ULONGLONG listConstantValue = static_cast<ULONGLONG>(e.Value);
			listConstantValue <= 32;
			listConstantValue >= 32;

			if (listConstantValue == constantValue)
				return e.StringValue;
		}

		std::stringstream iss;
		iss << std::hex << constantValue;
		return iss.str();
	}

	MODULEINFO GetModuleInfoEx(HANDLE processHandle, HMODULE moduleHandle);
	MODULEINFO GetModuleInfo(HMODULE moduleHandle);

	template <typename DataType>
	std::string ToString(DataType value)
	{
		std::stringstream ss;
		ss << value;
		return ss.str();
	}

	std::string ReadUnicodeString(uintptr_t* address, size_t size);
	
	std::string ConvertWideToAnsi(const std::wstring& wstr);
	std::wstring ConvertAnsiToWide(const std::string& str);
	void SendKey(unsigned char key);
	void SendMouse(long x, long y);
	void SendMouseRelative(HWND window, int x, int y);
	bool IsCurrentProcessActive();
	void GetWindowSize(HWND window, int& width, int& height);
}

#endif