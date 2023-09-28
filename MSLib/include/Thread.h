#pragma once
#include <Windows.h>

namespace ms
{
	enum THREAD_ACTION
	{
		RESUME,
		SUSPEND,
		TERMINATE
	};

	VOID ModuleThreads(LPCWSTR lpModuleName, THREAD_ACTION threadAction);
	ULONG_PTR GetThreadStartAddress(HANDLE hThread);
}
