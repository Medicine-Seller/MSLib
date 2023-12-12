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
		static NTSTATUS QuerySystemInformation(const SYSTEM_INFORMATION_CLASS systemInformationClass, PVOID systemInformation, const ULONG systemInformationLength, ULONG* returnLength);
		static NTSTATUS GetVirtualMemoryInfoEx(const HANDLE processHandle, PVOID baseAddress, MEMORY_BASIC_INFORMATION* mbi);
		static NTSTATUS GetVirtualMemoryInfo(const PVOID baseAddress, MEMORY_BASIC_INFORMATION* mbi);
	};
	
}

#endif