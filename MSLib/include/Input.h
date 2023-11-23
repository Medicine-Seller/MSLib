#include <Windows.h>

#ifndef INPUT_H
#define INPUT_H

namespace ms
{
	class Input
	{
	private:
		INPUT Device;
	public:
		Input();
		Input(BYTE inputType);
		UINT SendKey(const BYTE key);
		UINT SendMouseMove(const LONG x, const LONG y);
		UINT SendMouseClick(const LONG x, const LONG y);
		UINT SendMouseClickRelative(HWND& window, const LONG x, const LONG y);
	};

}


#endif

