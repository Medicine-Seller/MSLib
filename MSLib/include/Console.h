#include <Windows.h>

#ifndef CONSOLE_H
#define CONSOLE_H

namespace ms
{
	class Console
	{
	private:
	public:
		static BOOL Open(PCSTR title = "Console");
		static BOOL Close();
	};
}

#endif





