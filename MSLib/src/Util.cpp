#include "Util.h"
#include <sstream>

std::string ms::ReplaceString(std::string str, std::string findString, std::string replaceString)
{
	while (str.find(findString) != std::string::npos)
		str.replace(str.find(findString), replaceString.length(), replaceString);

	return str;
}

std::vector<BYTE>ms::StringToBytes(std::string bytesString)
{
	std::istringstream iss(bytesString);
	std::vector<BYTE> bytes;
	short byte;

	while (iss >> std::hex >> byte)
		bytes.push_back(static_cast<BYTE>(byte));

	return bytes;
}

MODULEINFO ms::GetModuleInfoEx(HANDLE processHandle, HMODULE moduleHandle)
{
	MODULEINFO modInfo;
	GetModuleInformation(processHandle, moduleHandle, &modInfo, sizeof(modInfo));
	return modInfo;
}

MODULEINFO ms::GetModuleInfo(HMODULE moduleHandle)
{
	return GetModuleInfoEx(GetCurrentProcess(), moduleHandle);
}

std::string ms::ConvertWideToAnsi(const std::wstring& wstr)
{
	int count = WideCharToMultiByte(CP_ACP, 0, wstr.c_str(), static_cast<int>(wstr.length()), NULL, 0, NULL, NULL);
	std::string str(count, 0);
	WideCharToMultiByte(CP_ACP, 0, wstr.c_str(), -1, &str[0], count, NULL, NULL);
	return str;
}

std::wstring ms::ConvertAnsiToWide(const std::string& str)
{
	int count = MultiByteToWideChar(CP_ACP, 0, str.c_str(), static_cast<int>(str.length()), NULL, 0);
	std::wstring wstr(count, 0);
	MultiByteToWideChar(CP_ACP, 0, str.c_str(), static_cast<int>(str.length()), &wstr[0], count);
	return wstr;
}

BOOL ms::IsCurrentProcessActive()
{
	HWND foregroundWindow = GetForegroundWindow();

	if (!foregroundWindow)
		return FALSE;

	DWORD foregroundProcessId;
	GetWindowThreadProcessId(foregroundWindow, &foregroundProcessId);

	DWORD currentProcessId = GetCurrentProcessId();
	if (foregroundProcessId != currentProcessId)
		return FALSE;

	return TRUE;
}

VOID ms::GetWindowSize(HWND window, UINT& width, UINT& height)
{
	RECT clientRect;
	GetClientRect(window, &clientRect);

	POINT upperLeft = { clientRect.left, clientRect.top };
	POINT lowerRight = { clientRect.right, clientRect.bottom };

	ClientToScreen(window, &upperLeft);
	ClientToScreen(window, &lowerRight);

	width = lowerRight.x - upperLeft.x;
	height = lowerRight.y - upperLeft.y;
}

PVOID ms::AllocateMemoryNearAddress(PVOID address, SIZE_T size) 
{
	const uintptr_t range = 0x7FFF0000;

	uintptr_t base = reinterpret_cast<uintptr_t>(address);
	uintptr_t end = base + range;

	PVOID allocatedAddress = NULL;
	for (uintptr_t i = base; i < end; i += 0x1000)
	{
		allocatedAddress = VirtualAlloc(reinterpret_cast<LPVOID>(i), size, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
		if (allocatedAddress)
			break;

		if (base - i <= 0x1000)
			continue;

		allocatedAddress = VirtualAlloc(reinterpret_cast<LPVOID>(base - i), size, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
		if (allocatedAddress)
			break;
	}

	return allocatedAddress;
}