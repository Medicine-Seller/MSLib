#include <Windows.h>
#include <vector>

#ifndef VIRTUALPROTECT_H
#define VIRTUALPROTECT_H

namespace ms
{
	class VirtualProtect
	{
	private:
		struct Protection
		{
			PVOID Address;
			SIZE_T WriteSize;
			ULONG OriginalProtect;
		};

		static std::vector<Protection> ProtectionList;

	public:


		static NTSTATUS PushProtectWriteEx(HANDLE processHandle, PVOID address, SIZE_T writeSize, ULONG newProtect);
		static NTSTATUS PushProtectWrite(PVOID address, SIZE_T writeSize, ULONG newProtect);

		static NTSTATUS PopProtectWriteEx(HANDLE processHandle);
		static NTSTATUS PopProtectWrite();

		static VOID RestoreAllWritablesEx(HANDLE processHandle);
		static VOID RestoreAllWritables();

		static NTSTATUS SetProtectVirtualMemoryModuleEx(HANDLE processHandle, HMODULE moduleHandle, ULONG newProtect);
		static NTSTATUS SetProtectVirtualMemoryModule(HMODULE moduleHandle, ULONG newProtect);
		static NTSTATUS SetProtectVirtualMemoryEx(HANDLE processHandle, PVOID baseAddress, SIZE_T regionSize, ULONG newProtect, ULONG* oldProtect);
		static NTSTATUS SetProtectVirtualMemory(PVOID baseAddress, SIZE_T regionSize, ULONG newProtect, ULONG* oldProtect);
	};
	
}

#endif