#include <Windows.h>
#include <vector>

#ifndef VIRTUALPROTECT_H
#define VIRTUALPROTECT_H

namespace ms
{
	struct SProtect
	{
		uintptr_t* Address;
		SIZE_T WriteSize;
		ULONG OriginalProtect;
	};

	static std::vector<SProtect> ProtectList;

	NTSTATUS PushProtectWriteEx(HANDLE processHandle, uintptr_t* address, SIZE_T writeSize, ULONG newProtect);
	NTSTATUS PushProtectWrite(uintptr_t* address, SIZE_T writeSize, ULONG newProtect);

	NTSTATUS PopProtectWriteEx(HANDLE processHandle);
	NTSTATUS PopProtectWrite();

	VOID RestoreAllWritablesEx(HANDLE processHandle);
	VOID RestoreAllWritables();
}

#endif