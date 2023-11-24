#include "Thread.h"
#include "NtApi.h"
#include <TlHelp32.h>
#include <Psapi.h>
#include "Constants.h"
#include "Util.h"

NTSTATUS ms::Thread::GetThreadStartAddress(HANDLE threadHandle, PVOID* threadStartAddress)
{
	auto NtQueryInformationThread = NtAPI::GetProcedure<NtQueryInformationThread_t>("ntdll.dll", "NtQueryInformationThread");
	if (!NtQueryInformationThread)
		return STATUS_INTERNAL_ERROR;

	PVOID startAddress;
	NTSTATUS status = NtQueryInformationThread(threadHandle, ThreadQuerySetWin32StartAddress, &startAddress, sizeof(PVOID), NULL);
	*threadStartAddress = startAddress;
	return status;
}

NTSTATUS ms::Thread::GetModuleThreads(PCSTR moduleName, std::vector<HANDLE>* threadHandles)
{
	ULONG processId = GetCurrentProcessId();
	HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, 0);
	HMODULE module = GetModuleHandleA(moduleName);

	if (!snapshot || !module)
		return STATUS_INTERNAL_ERROR;

	THREADENTRY32 threadEntry;
	threadEntry.dwSize = sizeof(THREADENTRY32);

	if (!Thread32First(snapshot, &threadEntry))
	{
		CloseHandle(snapshot);
		return STATUS_INTERNAL_ERROR;
	}

	MODULEINFO modInfo = GetModuleInfo(module);

	do
	{
		if (threadEntry.th32OwnerProcessID != processId)
			continue;

		HANDLE threadHandle = OpenThread(THREAD_ALL_ACCESS, 0, threadEntry.th32ThreadID);
		if (!threadHandle)
			continue;

		PVOID threadStartAddress = NULL;
		NTSTATUS status = GetThreadStartAddress(threadHandle, &threadStartAddress);
		if (!SUCCEEDED(status))
			continue;

		uintptr_t startAddress = reinterpret_cast<uintptr_t>(threadStartAddress);
		uintptr_t moduleBase = reinterpret_cast<uintptr_t>(modInfo.lpBaseOfDll);

		if (moduleBase < startAddress && startAddress < moduleBase + modInfo.SizeOfImage)
			threadHandles->push_back(threadHandle);

	} while (Thread32Next(snapshot, &threadEntry));

	CloseHandle(snapshot);
	return STATUS_SUCCESS;
}

BOOL ms::Thread::SetState(HANDLE threadHandle, THREAD_STATE threadState)
{
	DWORD result = 0;
	switch (threadState)
	{
	case THREAD_STATE::RESUME: result = ResumeThread(threadHandle); break;
	case THREAD_STATE::SUSPEND: result = SuspendThread(threadHandle); break;
	case THREAD_STATE::TERMINATE: result = TerminateThread(threadHandle, 0); break;
	}

	if (result == 0 || result == -1)
		return FALSE;
	
	return TRUE;
}

BOOL ms::Thread::SetState(std::vector<HANDLE> threadHandles, THREAD_STATE threadState)
{
	BOOL result = 0;
	for (auto& threadHandle : threadHandles)
		if (threadHandle)
			result |= SetState(threadHandle, threadState);

	return result;
}