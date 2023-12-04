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

		static NTSTATUS GetThreadStartAddress(const HANDLE threadHandle, PVOID* threadStartAddress);
		static NTSTATUS GetModuleThreads(PCSTR moduleName, std::vector<HANDLE>* threadHandles);
		static BOOL SetThreadState(const HANDLE threadHandle, const THREAD_STATE threadState);
		static BOOL SetThreadState(const std::vector<HANDLE>* threadHandles, const THREAD_STATE threadState);
	};
}

#endif