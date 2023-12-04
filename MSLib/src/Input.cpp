#include "Input.h"

ms::Input::Input()
{
	Input(0);
}

ms::Input::Input(const BYTE inputType)
{
	memset(&Device, 0, sizeof(Device));
	switch (inputType)
	{
	case INPUT_KEYBOARD: Device.type = INPUT_KEYBOARD; break;
	case INPUT_MOUSE: Device.type = INPUT_MOUSE; break;
	default: Device.type = INPUT_KEYBOARD;
	}
}

UINT ms::Input::SendKey(const BYTE key)
{
	if (Device.type != INPUT_KEYBOARD)
		return 0;

	Device.ki.wVk = key;
	Device.ki.dwFlags = 0;

	UINT result = 0;
	result += SendInput(1, &Device, sizeof(INPUT));

	Device.ki.dwFlags = KEYEVENTF_KEYUP;
	result += SendInput(1, &Device, sizeof(INPUT));
	return result;
}

UINT ms::Input::SendMouseMove(const LONG x, const LONG y)
{
	if (Device.type != INPUT_MOUSE)
		return 0;

	Device.mi.dwFlags = MOUSEEVENTF_MOVE | MOUSEEVENTF_ABSOLUTE;
	Device.mi.dx = x * 65535 / GetSystemMetrics(SM_CXSCREEN);
	Device.mi.dy = y * 65535 / GetSystemMetrics(SM_CYSCREEN);
	return SendInput(1, &Device, sizeof(INPUT));
}

UINT ms::Input::SendMouseClick(const LONG x, const LONG y)
{
	if (Device.type != INPUT_MOUSE)
		return 0;

	UINT result = SendMouseMove(x, y);

	Device.mi.dwFlags = MOUSEEVENTF_LEFTDOWN;
	result += SendInput(1, &Device, sizeof(INPUT));

	Device.mi.dwFlags = MOUSEEVENTF_LEFTUP;
	result += SendInput(1, &Device, sizeof(INPUT));

	return result;
}

UINT ms::Input::SendMouseClickRelative(const HWND& window, const LONG x, const LONG y)
{
	RECT clientWindow;
	if (!GetClientRect(window, &clientWindow))
		return 0;

	POINT screenPoint = { clientWindow.left + x, clientWindow.top + y };
	if (!ClientToScreen(window, &screenPoint))
		return 0;

	return SendMouseClick(screenPoint.x, screenPoint.y);
}

