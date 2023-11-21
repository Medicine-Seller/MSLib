#include "VirtualProtect.h"
#include "Constants.h"
#include "NtApi.h"

NTSTATUS ms::PushProtectWriteEx(HANDLE processHandle, uintptr_t* address, SIZE_T writeSize, ULONG newProtect)
{
	ULONG originalProtect;
	NTSTATUS status = SetProtectVirtualMemoryEx(processHandle, reinterpret_cast<PVOID>(address), writeSize, newProtect, &originalProtect);
	if (status != STATUS_SUCCESS)
		return status;

	ProtectList.push_back({ address, writeSize, originalProtect });
	return status;
}

NTSTATUS ms::PushProtectWrite(uintptr_t* address, SIZE_T writeSize, ULONG newProtect)
{
	ULONG originalProtect;
	NTSTATUS status = SetProtectVirtualMemory(address, writeSize, newProtect, &originalProtect);
	if (status != STATUS_SUCCESS)
		return status;

	ProtectList.push_back({ address, writeSize, originalProtect });
	return status;
}

NTSTATUS ms::PopProtectWriteEx(HANDLE processHandle)
{
	SProtect& originalProtect = ProtectList[ProtectList.size() - 1];
	NTSTATUS status = SetProtectVirtualMemoryEx(processHandle, reinterpret_cast<PVOID>(originalProtect.Address), originalProtect.WriteSize, originalProtect.OriginalProtect, nullptr);
	
	if (status != STATUS_SUCCESS)
		return status;

	ProtectList.pop_back();
	return status;
}

NTSTATUS ms::PopProtectWrite()
{
	SProtect& originalProtect = ProtectList[ProtectList.size() - 1];
	NTSTATUS status = SetProtectVirtualMemory(reinterpret_cast<PVOID>(originalProtect.Address), originalProtect.WriteSize, originalProtect.OriginalProtect, nullptr);

	if (status != STATUS_SUCCESS)
		return status;

	ProtectList.pop_back();
	return status;
}

VOID ms::RestoreAllWritablesEx(HANDLE processHandle)
{
	for (auto& protect : ProtectList)
		SetProtectVirtualMemoryEx(processHandle, reinterpret_cast<PVOID>(protect.Address), protect.WriteSize, protect.OriginalProtect, nullptr);

	ProtectList.clear();
}

VOID ms::RestoreAllWritables()
{
	for (auto& protect : ProtectList)
		SetProtectVirtualMemory(reinterpret_cast<PVOID>(protect.Address), protect.WriteSize, protect.OriginalProtect, nullptr);
	
	ProtectList.clear();
}