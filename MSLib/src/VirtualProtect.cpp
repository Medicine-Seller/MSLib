#include "VirtualProtect.h"
#include "NtApi.h"
#include "Util.h"

std::vector<ms::VirtualProtect::Protection> ms::VirtualProtect::ProtectionList;

NTSTATUS ms::VirtualProtect::PushProtectWriteEx(const HANDLE processHandle, const PVOID address, const SIZE_T writeSize, const ULONG newProtect)
{
	ULONG originalProtect;
	NTSTATUS status = SetProtectVirtualMemoryEx(processHandle, address, writeSize, newProtect, &originalProtect);
	if (!SUCCEEDED(status))
		return status;

	ProtectionList.push_back({ address, writeSize, originalProtect });
	return status;
}

NTSTATUS ms::VirtualProtect::PushProtectWrite(const PVOID address, const SIZE_T writeSize, const ULONG newProtect)
{
	HANDLE processHandle = OpenProcess(PROCESS_VM_OPERATION, FALSE, GetCurrentProcessId());
	NTSTATUS status = PushProtectWriteEx(processHandle, address, writeSize, newProtect);
	CloseHandle(processHandle);
	return status;
}

NTSTATUS ms::VirtualProtect::PopProtectWriteEx(const HANDLE processHandle)
{
	Protection& originalProtect = ProtectionList[ProtectionList.size() - 1];
	NTSTATUS status = SetProtectVirtualMemoryEx(processHandle, originalProtect.Address, originalProtect.WriteSize, originalProtect.OriginalProtect, nullptr);
	if (!SUCCEEDED(status))
		return status;

	ProtectionList.pop_back();
	return status;
}

NTSTATUS ms::VirtualProtect::PopProtectWrite()
{
	HANDLE processHandle = OpenProcess(PROCESS_VM_OPERATION, FALSE, GetCurrentProcessId());
	NTSTATUS status = PopProtectWriteEx(processHandle);
	CloseHandle(processHandle);
	return status;
}

VOID ms::VirtualProtect::RestoreAllWritablesEx(const HANDLE processHandle)
{
	for (auto& protect : ProtectionList)
		SetProtectVirtualMemoryEx(processHandle, reinterpret_cast<PVOID>(protect.Address), protect.WriteSize, protect.OriginalProtect, nullptr);

	ProtectionList.clear();
}

VOID ms::VirtualProtect::RestoreAllWritables()
{
	for (auto& protect : ProtectionList)
		SetProtectVirtualMemory(reinterpret_cast<PVOID>(protect.Address), protect.WriteSize, protect.OriginalProtect, nullptr);
	
	ProtectionList.clear();
}

NTSTATUS ms::VirtualProtect::SetProtectVirtualMemoryEx(const HANDLE processHandle, PVOID baseAddress, SIZE_T regionSize, const ULONG newProtect, ULONG* oldProtect)
{
	auto NtProtectVirtualMemory = NtAPI::GetProcedure<NtProtectVirtualMemory_t>("ntdll.dll", "NtProtectVirtualMemory");
	if (!NtProtectVirtualMemory)
		return STATUS_INTERNAL_ERROR;

	ULONG tempOldProtect;
	NTSTATUS status = NtProtectVirtualMemory(processHandle, &baseAddress, &regionSize, newProtect, &tempOldProtect);

	if (oldProtect)
		*oldProtect = tempOldProtect;

	return status;
}

NTSTATUS ms::VirtualProtect::SetProtectVirtualMemory(PVOID baseAddress, const SIZE_T regionSize, const ULONG newProtect, ULONG* oldProtect)
{
	HANDLE processHandle = OpenProcess(PROCESS_VM_OPERATION, FALSE, GetCurrentProcessId());
	NTSTATUS status = SetProtectVirtualMemoryEx(processHandle, baseAddress, regionSize, newProtect, oldProtect);
	CloseHandle(processHandle);
	return status;
}

NTSTATUS ms::VirtualProtect::SetProtectVirtualMemoryModuleEx(const HANDLE processHandle, const HMODULE moduleHandle, const ULONG newProtect)
{
	MODULEINFO modInfo = GetModuleInfo(moduleHandle);
	return SetProtectVirtualMemoryEx(processHandle, modInfo.lpBaseOfDll, modInfo.SizeOfImage, newProtect, nullptr);
}

NTSTATUS ms::VirtualProtect::SetProtectVirtualMemoryModule(const HMODULE moduleHandle, const ULONG newProtect)
{
	HANDLE processHandle = OpenProcess(PROCESS_VM_OPERATION, FALSE, GetCurrentProcessId());
	NTSTATUS status = SetProtectVirtualMemoryModuleEx(processHandle, moduleHandle, newProtect);
	CloseHandle(processHandle);
	return status;
}