#include "Console.h"
#include <Windows.h>
#include <iostream>

bool ms::ConsoleOpen(std::string title)
{
	if (!AllocConsole())
		return false;

	SetConsoleTitleA(title.c_str());
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