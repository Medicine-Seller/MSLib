#include "UtilTemplates.h"

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
	std::string GetConstantString(const ULONGLONG value);
}

#endif