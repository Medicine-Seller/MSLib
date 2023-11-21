#include <string>

#ifndef CONSOLE_H
#define CONSOLE_H

namespace ms
{
	// Description: Allocate console
	// [in] title - The console title
	// Return: True if console is allocated
	bool ConsoleOpen(std::string title = "Console");

	// Description: Free Console
	// Return: True if console is freed
	bool ConsoleClose();
}

#endif





