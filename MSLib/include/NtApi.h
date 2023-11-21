#include <Windows.h>
#include "NtApiTypes.h"

#ifndef NTAPI_H
#define NTAPI_H

namespace ms
{
	NTSTATUS QuerySystemInformation(SYSTEM_INFORMATION_CLASS systemInformationClass, PVOID systemInformation, ULONG systemInformationLength, ULONG* returnLength);
	NTSTATUS GetVirtualMemoryInfoEx(HANDLE processHandle, PVOID baseAddress, MEMORY_BASIC_INFORMATION* mbi);
	NTSTATUS GetVirtualMemoryInfo(PVOID baseAddress, MEMORY_BASIC_INFORMATION* mbi);
	NTSTATUS SetProtectVirtualMemoryModuleEx(HANDLE processHandle, HMODULE moduleHandle, ULONG newProtect);
	NTSTATUS SetProtectVirtualMemoryModule(HMODULE moduleHandle, ULONG newProtect);
	NTSTATUS SetProtectVirtualMemoryEx(HANDLE processHandle, PVOID baseAddress, SIZE_T regionSize, ULONG newProtect, ULONG* oldProtect);
	NTSTATUS SetProtectVirtualMemory(PVOID baseAddress, SIZE_T regionSize, ULONG newProtect, ULONG* oldProtect);

}

#endif