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

	// Description: Get threads within a module
	// [in] lpModuleName - Module name
	// Return: A vector of thread handles
	std::vector<HANDLE> GetModuleThreads(LPCWSTR lpModuleName);

	// Description: Modify thread
	// [in] hThread - Handle to thread
	// [in] actionType - Define action taken on thread
	// Return: True if thread is successfully modified
	BOOL ModifyThread(HANDLE hThread, THREAD_ACTION actionType);
	
	// Description: Modify a vector of thread
	// [in] vThread - A vector of handle to threads
	// [in] actionType - Define action taken on threads
	// Return: True if at least one thread is modified successfully
	BOOL ModifyThread(std::vector<HANDLE> hThread, THREAD_ACTION actionType);

	// Description: Get the thread start address
	// [in] hThread - Handle to thread
	// Return: Starting address of thread if success
	ULONG_PTR GetThreadStartAddress(HANDLE hThread);
}
