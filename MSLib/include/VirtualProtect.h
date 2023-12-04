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
		static NTSTATUS PushProtectWriteEx(const HANDLE processHandle, const PVOID address, const SIZE_T writeSize, const ULONG newProtect);
		static NTSTATUS PushProtectWrite(const PVOID address, const SIZE_T writeSize, const ULONG newProtect);

		static NTSTATUS PopProtectWriteEx(const HANDLE processHandle);
		static NTSTATUS PopProtectWrite();

		static VOID RestoreAllWritablesEx(const HANDLE processHandle);
		static VOID RestoreAllWritables();

		static NTSTATUS SetProtectVirtualMemoryModuleEx(const HANDLE processHandle, const HMODULE moduleHandle, const ULONG newProtect);
		static NTSTATUS SetProtectVirtualMemoryModule(const HMODULE moduleHandle, const ULONG newProtect);
		static NTSTATUS SetProtectVirtualMemoryEx(const HANDLE processHandle, PVOID baseAddress, SIZE_T regionSize, const ULONG newProtect, ULONG* oldProtect);
		static NTSTATUS SetProtectVirtualMemory(PVOID baseAddress, SIZE_T regionSize, const ULONG newProtect, ULONG* oldProtect);
	};
	
}

#endif