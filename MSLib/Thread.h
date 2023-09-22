#pragma once

enum THREAD_ACTION
{
	RESUME,
	SUSPEND,
	TERMINATE
};

VOID ModuleThreads(LPCWSTR lpModuleName, THREAD_ACTION threadAction);
ULONG_PTR GetThreadStartAddress(HANDLE hThread);
