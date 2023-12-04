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
	std::string ReplaceString(std::string str, std::string findString, std::string replaceString);
	std::vector<BYTE>StringToBytes(const std::string bytesString);

	MODULEINFO GetModuleInfoEx(const HANDLE processHandle, const HMODULE moduleHandle);
	MODULEINFO GetModuleInfo(const HMODULE moduleHandle);

	std::string ConvertWideToAnsi(const std::wstring& wstr);
	std::wstring ConvertAnsiToWide(const std::string& str);

	BOOL IsCurrentProcessActive();
	VOID GetWindowSize(const HWND window, UINT& width, UINT& height);
	PVOID AllocateMemoryNearAddress(const PVOID address, const SIZE_T size);

	template<typename DataType>
	DataType IncrementByByte(const DataType address, const LONGLONG offset)
	{
		return reinterpret_cast<DataType>(reinterpret_cast<BYTE*>(address) + offset);
	}

	template<typename DataType>
	DataType GetAddress(const uintptr_t base, const std::vector<uintptr_t> offsets)
	{
		for (unsigned int i = 0; i < offsets.size(); i++)
		{
			if (base < 0xFFFFFF || IsBadReadPtr((uintptr_t*)base, sizeof(uintptr_t)))
			{
				base = 0;
				break;
			}

			if (i == offsets.size() - 1)
				base = (uintptr_t)((BYTE*)base + offsets[i]);
			else
				base = *(uintptr_t*)((BYTE*)base + offsets[i]);
		}

		return (DataType)base;
	}

	template <typename DataType = ULONG>
	DataType GetConstant(const std::vector<ConstantString>& constantStrings, const std::string str)
	{
		for (auto& e : constantStrings)
			if (e.StringValue == str)
				return static_cast<DataType>(e.Value);

		return -1;
	}

	template<typename DataType>
	std::string GetConstantString(const std::vector<ConstantString>& constantStrings, const DataType value)
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

	template <typename DataType>
	std::string ToString(const DataType value)
	{
		std::stringstream ss;
		ss << value;
		return ss.str();
	}
}

#endif