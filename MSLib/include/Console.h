#include <Windows.h>

namespace ms
{
	// Description: Allocate console
	// [in] lpTitle - The console title
	// Return: True if console is allocated
	bool ConsoleOpen(LPCTSTR lpTitle = L"Console");

	// Description: Free Console
	// Return: True if console is freed
	bool ConsoleClose();
}







