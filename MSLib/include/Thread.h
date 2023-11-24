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

		static NTSTATUS GetThreadStartAddress(HANDLE threadHandle, PVOID* threadStartAddress);
		static NTSTATUS GetModuleThreads(PCSTR moduleName, std::vector<HANDLE>* threadHandles);
		static BOOL SetState(HANDLE threadHandle, THREAD_STATE threadState);
		static BOOL SetState(std::vector<HANDLE> threadHandles, THREAD_STATE threadState);
	};
}

#endif