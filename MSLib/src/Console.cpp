#include "Console.h"
#include <Windows.h>
#include <iostream>

BOOL ms::Console::Open(PCSTR title)
{
	if (!AllocConsole())
		return FALSE;

	if (!SetConsoleTitleA(title))
		return FALSE;

	FILE* newstreamptr;
	freopen_s(&newstreamptr, "CONIN$", "r", stdin);
	freopen_s(&newstreamptr, "CONOUT$", "w", stdout);
	freopen_s(&newstreamptr, "CONOUT$", "w", stderr);
	return TRUE;
}

BOOL ms::Console::Close()
{
	return FreeConsole();
}