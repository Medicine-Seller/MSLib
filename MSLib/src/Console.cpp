#include "Console.h"
#include <iostream>

bool ms::ConsoleOpen(LPCTSTR lpTitle)
{
	if (!AllocConsole())
		return false;

	SetConsoleTitle(lpTitle);
	FILE* newstreamptr;
	freopen_s(&newstreamptr, "CONIN$", "r", stdin);
	freopen_s(&newstreamptr, "CONOUT$", "w", stdout);
	freopen_s(&newstreamptr, "CONOUT$", "w", stderr);
	return true;
}

bool ms::ConsoleClose()
{
	return FreeConsole();
}