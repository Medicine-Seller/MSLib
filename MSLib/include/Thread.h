#include <Windows.h>
#include <vector>

#ifndef THREAD_H
#define THREAD_H

namespace ms
{
	class Thread
	{
	private:

	public:
		enum THREAD_STATE
		{
			RESUME,
			SUSPEND,
			TERMINATE
		};

		NTSTATUS GetThreadStartAddress(HANDLE threadHandle, PVOID* threadStartAddress);
		NTSTATUS GetModuleThreads(PCSTR moduleName, std::vector<HANDLE>* threadHandles);
		BOOL SetState(HANDLE threadHandle, THREAD_STATE threadState);
		BOOL SetState(std::vector<HANDLE> threadHandles, THREAD_STATE threadState);
	};
}

#endif