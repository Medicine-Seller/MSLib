#include <iomanip>
#include "NtApi.h"
#include "Util.h"
#include "Macros.h"

typedef NTSYSCALLAPI NTSTATUS (NTAPI* pfnNtQueryVirtualMemory)(
    _In_ HANDLE ProcessHandle,
    _In_opt_ PVOID BaseAddress,
    _In_ MEMORY_INFORMATION_CLASS MemoryInformationClass,
    _Out_writes_bytes_(MemoryInformationLength) PVOID MemoryInformation,
    _In_ SIZE_T MemoryInformationLength,
    _Out_opt_ PSIZE_T ReturnLength
);

NTSTATUS ms::GetVirtualMemoryInfoEx(HANDLE processHandle, PVOID baseAddress, MEMORY_BASIC_INFORMATION* mbi)
{
    if (mbi == nullptr)
        return -1;

    HMODULE ntdll = GetModuleHandleA("ntdll.dll");
    if (!ntdll)
        return STATUS_INVALID_HANDLE;

    pfnNtQueryVirtualMemory NtQueryVirtualMemory = (pfnNtQueryVirtualMemory)GetProcAddress(ntdll, "NtQueryVirtualMemory");
    if (!NtQueryVirtualMemory)
        return -1;

    SIZE_T ulReturnLength;
    NTSTATUS status = NtQueryVirtualMemory(processHandle, baseAddress, MemoryBasicInformation, mbi, sizeof(*mbi), &ulReturnLength);

    return status;
}

NTSTATUS ms::GetVirtualMemoryInfo(PVOID baseAddress, MEMORY_BASIC_INFORMATION* mbi)
{
    return GetVirtualMemoryInfoEx(GetCurrentProcess(), baseAddress, mbi);
}

typedef NTSYSCALLAPI NTSTATUS (NTAPI*pfnNtProtectVirtualMemory)(
    _In_ HANDLE ProcessHandle,
    _Inout_ PVOID* BaseAddress,
    _Inout_ PSIZE_T RegionSize,
    _In_ ULONG NewProtect,
    _Out_ PULONG OldProtect
);

NTSTATUS ms::SetProtectVirtualMemoryEx(HANDLE processHandle, PVOID baseAddress, SIZE_T regionSize, ULONG newProtect, ULONG* oldProtect)
{
    HMODULE ntdll = GetModuleHandleA("ntdll.dll");
    
    if (!ntdll)
        return STATUS_INVALID_HANDLE;

    pfnNtProtectVirtualMemory NtProtectVirtualMemory = (pfnNtProtectVirtualMemory)GetProcAddress(ntdll, "NtProtectVirtualMemory");
    
    if (!NtProtectVirtualMemory)
        return -1;

    ULONG ulOldProtect;
    NTSTATUS status = NtProtectVirtualMemory(processHandle, &baseAddress, &regionSize, newProtect, &ulOldProtect);
    
    if (oldProtect != nullptr)
        *oldProtect = ulOldProtect;

    CloseHandle(ntdll);
    return status;
}

NTSTATUS ms::SetProtectVirtualMemory(PVOID baseAddress, SIZE_T regionSize, ULONG newProtect, ULONG* oldProtect)
{
    HANDLE processHandle = OpenProcess(PROCESS_VM_OPERATION, false, GetCurrentProcessId());
    NTSTATUS status = SetProtectVirtualMemoryEx(processHandle, baseAddress, regionSize, newProtect, oldProtect);
    CloseHandle(processHandle);
    return status;
}

NTSTATUS ms::SetProtectVirtualMemoryModuleEx(HANDLE processHandle, HMODULE moduleHandle, ULONG newProtect)
{
    MODULEINFO modInfo = GetModuleInfo(moduleHandle);
    return SetProtectVirtualMemoryEx(processHandle, modInfo.lpBaseOfDll, modInfo.SizeOfImage, newProtect, nullptr);
}

NTSTATUS ms::SetProtectVirtualMemoryModule(HMODULE moduleHandle, ULONG newProtect)
{
    HANDLE processHandle = OpenProcess(PROCESS_VM_OPERATION, false, GetCurrentProcessId());
    NTSTATUS status = SetProtectVirtualMemoryModuleEx(processHandle, moduleHandle, newProtect);
    CloseHandle(processHandle);
    return status;
}

typedef NTSYSCALLAPI NTSTATUS (NTAPI* pfnNtQuerySystemInformation)(
    _In_ SYSTEM_INFORMATION_CLASS SystemInformationClass,
    _Out_writes_bytes_opt_(SystemInformationLength) PVOID SystemInformation,
    _In_ ULONG SystemInformationLength,
    _Out_opt_ PULONG ReturnLength
);

NTSTATUS ms::QuerySystemInformation(SYSTEM_INFORMATION_CLASS systemInformationClass, PVOID systemInformation, ULONG systemInformationLength, ULONG* returnLength)
{
    HMODULE ntdll = GetModuleHandleA("ntdll.dll");
    if (!ntdll)
        return STATUS_INVALID_HANDLE;

    pfnNtQuerySystemInformation NtQuerySystemInformation = (pfnNtQuerySystemInformation)GetProcAddress(ntdll, "NtQuerySystemInformation");
    if (!NtQuerySystemInformation)
        return -1;

    ULONG tempReturnLength;
    NTSTATUS status = NtQuerySystemInformation(systemInformationClass, systemInformation, systemInformationLength, &tempReturnLength);
    if (returnLength != nullptr)
        *returnLength = tempReturnLength;

    CloseHandle(ntdll);
    return status;
}
