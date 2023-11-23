#include "NtApi.h"
#include <iomanip>
#include "Util.h"
#include "Macros.h"

NTSTATUS ms::NtAPI::GetVirtualMemoryInfoEx(HANDLE processHandle, PVOID baseAddress, MEMORY_BASIC_INFORMATION* mbi)
{
    if (!mbi)
        return STATUS_INTERNAL_ERROR;

    auto NtQueryVirtualMemory = GetProcedure<NtQueryVirtualMemory_t>("ntdll.dll", "NtQueryVirtualMemory");
    if (!NtQueryVirtualMemory)
        return STATUS_INTERNAL_ERROR;

    NTSTATUS status = NtQueryVirtualMemory(processHandle, baseAddress, MemoryBasicInformation, mbi, sizeof(*mbi), NULL);

    return status;
}

NTSTATUS ms::NtAPI::GetVirtualMemoryInfo(PVOID baseAddress, MEMORY_BASIC_INFORMATION* mbi)
{
    return GetVirtualMemoryInfoEx(GetCurrentProcess(), baseAddress, mbi);
}

NTSTATUS ms::NtAPI::QuerySystemInformation(SYSTEM_INFORMATION_CLASS systemInformationClass, PVOID systemInformation, ULONG systemInformationLength, ULONG* returnLength)
{
    auto NtQuerySystemInformation = GetProcedure<NtQuerySystemInformation_t>("ntdll.dll", "NtQuerySystemInformation");
    if (!NtQuerySystemInformation)
        return STATUS_INTERNAL_ERROR;

    ULONG tempReturnLength;
    NTSTATUS status = NtQuerySystemInformation(systemInformationClass, systemInformation, systemInformationLength, &tempReturnLength);
    if (returnLength)
        *returnLength = tempReturnLength;

    return status;
}
