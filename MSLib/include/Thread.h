#pragma once
#include <Windows.h>
#include <vector>

namespace ms
{
	enum THREAD_ACTION
	{
		RESUME,
		SUSPEND,
		TERMINATE
	};

	std::vector<HANDLE> GetModuleThreads(LPCWSTR lpModuleName);
	BOOL ModifyThread(HANDLE hThread, THREAD_ACTION actionType);
	BOOL ModifyThread(std::vector<HANDLE> hThread, THREAD_ACTION actionType);
	ULONG_PTR GetThreadStartAddress(HANDLE hThread);
}
