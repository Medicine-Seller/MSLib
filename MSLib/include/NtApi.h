#include <Windows.h>
#include "NtApiTypes.h"

#ifndef NTAPI_H
#define NTAPI_H

namespace ms
{
	class NtAPI
	{
	private:

	public:

		template <typename FunctionPointer>
		static FunctionPointer GetProcedure(PCSTR moduleName, PCSTR procedureName)
		{
			HMODULE module = GetModuleHandleA(moduleName);
			if (!module)
				return 0;

			FunctionPointer functionPtr = (FunctionPointer)GetProcAddress(module, procedureName);
			return functionPtr;
		}

		static NTSTATUS QuerySystemInformation(SYSTEM_INFORMATION_CLASS systemInformationClass, PVOID systemInformation, ULONG systemInformationLength, ULONG* returnLength);
		static NTSTATUS GetVirtualMemoryInfoEx(HANDLE processHandle, PVOID baseAddress, MEMORY_BASIC_INFORMATION* mbi);
		static NTSTATUS GetVirtualMemoryInfo(PVOID baseAddress, MEMORY_BASIC_INFORMATION* mbi);
	};
	
}

#endif