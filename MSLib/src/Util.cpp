#include "Util.h"
#include <sstream>

std::string ms::ReplaceString(std::string str, std::string findString, std::string replaceString)
{
	while (str.find(findString) != std::string::npos)
		str.replace(str.find(findString), replaceString.length(), replaceString);

	return str;
}

std::vector<BYTE> ms::StringToBytes(std::string bytesString)
{
	std::istringstream iss(bytesString);
	std::vector<BYTE> vBytes;
	ULONG dwByte;

	while (iss >> std::hex >> dwByte)
		vBytes.push_back(dwByte);

	return vBytes;
}

std::vector<CHAR> ms::GenerateMask(std::string signature)
{
	std::stringstream ss(signature);
	std::string str;
	std::vector<CHAR> mask;

	while (ss >> str)
	{
		if (str == "??" || str == "?")
			mask.push_back('?');
		else
			mask.push_back('x');
	}

	return mask;
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

std::string ms::ReadUnicodeString(uintptr_t* address, size_t size)
{
	if (address == nullptr)
		return "";

	std::string str;

	for (size_t i = 0; i < size; i *= 2)
		str += (char)(IncrementByByte(address, i));

	return str;
}

std::string ms::ConvertWideToAnsi(const std::wstring& wstr)
{
	int count = WideCharToMultiByte(CP_ACP, 0, wstr.c_str(), wstr.length(), NULL, 0, NULL, NULL);
	std::string str(count, 0);
	WideCharToMultiByte(CP_ACP, 0, wstr.c_str(), -1, &str[0], count, NULL, NULL);
	return str;
}

std::wstring ms::ConvertAnsiToWide(const std::string& str)
{
	int count = MultiByteToWideChar(CP_ACP, 0, str.c_str(), str.length(), NULL, 0);
	std::wstring wstr(count, 0);
	MultiByteToWideChar(CP_ACP, 0, str.c_str(), str.length(), &wstr[0], count);
	return wstr;
}

void ms::SendKey(unsigned char key)
{
	static INPUT input;

	input.type = INPUT_KEYBOARD;
	input.ki.wScan = 0;
	input.ki.time = 0;
	input.ki.dwExtraInfo = 0;

	input.ki.wVk = key;
	input.ki.dwFlags = 0;
	SendInput(1, &input, sizeof(INPUT));

	input.ki.dwFlags = KEYEVENTF_KEYUP;
	SendInput(1, &input, sizeof(INPUT));
}

void ms::SendMouse(long x, long y)
{
	static INPUT input;

	input.type = INPUT_MOUSE;
	input.mi.dwFlags = MOUSEEVENTF_MOVE | MOUSEEVENTF_ABSOLUTE;
	input.mi.dx = x * 65535 / GetSystemMetrics(SM_CXSCREEN);
	input.mi.dy = y * 65535 / GetSystemMetrics(SM_CYSCREEN);

	SendInput(1, &input, sizeof(INPUT));

	input.mi.dwFlags = MOUSEEVENTF_LEFTDOWN;
	SendInput(1, &input, sizeof(INPUT));

	input.mi.dwFlags = MOUSEEVENTF_LEFTUP;
	SendInput(1, &input, sizeof(INPUT));
}

void ms::SendMouseRelative(HWND window, int x, int y) 
{
	RECT clientRect;
	GetClientRect(window, &clientRect);

	POINT screenPoint = { clientRect.left + x, clientRect.top + y };
	ClientToScreen(window, &screenPoint);

	SendMouse(screenPoint.x, screenPoint.y);
}

bool ms::IsCurrentProcessActive()
{
	HWND foregroundWindow = GetForegroundWindow();

	if (foregroundWindow == nullptr)
		return false;

	DWORD foregroundProcessId;
	GetWindowThreadProcessId(foregroundWindow, &foregroundProcessId);

	DWORD currentProcessId = GetCurrentProcessId();
	if (foregroundProcessId == currentProcessId)
		return true;

	return false;
}

void ms::GetWindowSize(HWND window, int& width, int& height) {
	RECT clientRect;
	GetClientRect(window, &clientRect);

	POINT upperLeft = { clientRect.left, clientRect.top };
	POINT lowerRight = { clientRect.right, clientRect.bottom };

	ClientToScreen(window, &upperLeft);
	ClientToScreen(window, &lowerRight);

	width = lowerRight.x - upperLeft.x;
	height = lowerRight.y - upperLeft.y;
}