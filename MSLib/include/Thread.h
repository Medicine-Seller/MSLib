#include <Windows.h>
#include <vector>

#ifndef THREAD_H
#define THREAD_H

namespace ms
{
	enum THREAD_ACTION
	{
		RESUME,
		SUSPEND,
		TERMINATE
	};

	// Description: Get threads within a module
	// [in] moduleName - Module name
	// Return: A vector of thread handles
	std::vector<HANDLE> GetModuleThreads(LPCWSTR moduleName);

	// Description: Modify thread
	// [in] threadHandle - Handle to thread
	// [in] actionType - Define action taken on thread
	// Return: True if thread is successfully modified
	BOOL ModifyThread(HANDLE threadHandle, THREAD_ACTION actionType);
	
	// Description: Modify a vector of thread
	// [in] vThread - A vector of handle to threads
	// [in] actionType - Define action taken on threads
	// Return: True if at least one thread is modified successfully
	BOOL ModifyThread(std::vector<HANDLE> threadHandle, THREAD_ACTION actionType);

	// Description: Get the thread start address
	// [in] threadHandle - Handle to thread
	// Return: Starting address of thread if success
	ULONG_PTR GetThreadStartAddress(HANDLE threadHandle);
}

#endif