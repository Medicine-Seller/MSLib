#include "Thread.h"
#include "Macros.h"
#include <TlHelp32.h>
#include <Psapi.h>

enum THREADINFOCLASS
{
	ThreadQuerySetWin32StartAddress = 9,
};

typedef NTSTATUS(__stdcall* pfnNtQueryInformationThread)(HANDLE, THREADINFOCLASS, void*, ULONG_PTR, ULONG_PTR*);

ULONG_PTR ms::GetThreadStartAddress(HANDLE threadHandle)
{
	HMODULE ntdll = GetModuleHandleA("ntdll.dll");
	if (!ntdll)
		return STATUS_INVALID_HANDLE;

	auto NtQueryInformationThread = reinterpret_cast<pfnNtQueryInformationThread>(GetProcAddress(ntdll, "NtQueryInformationThread"));
	if (!NtQueryInformationThread)
		return -1;

	ULONG_PTR ulStartAddress = 0;
	NTSTATUS status = NtQueryInformationThread(threadHandle, ThreadQuerySetWin32StartAddress, &ulStartAddress, sizeof(ULONG_PTR), nullptr);

	if (status)
		return 0;

	return ulStartAddress;
}

std::vector<HANDLE> ms::GetModuleThreads(LPCWSTR moduleName)
{
	ULONG dwProcessId = GetCurrentProcessId();
	HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, 0);
	HMODULE moduleHandle = GetModuleHandle(moduleName);

	if (!hSnap || !moduleHandle)
		return {};

	THREADENTRY32 threadEntry;
	threadEntry.dwSize = sizeof(THREADENTRY32);

	if (!Thread32First(hSnap, &threadEntry))
	{
		CloseHandle(hSnap);
		return {};
	}

	std::vector<HANDLE> vThreadHandles;
	MODULEINFO modInfo;
	GetModuleInformation(GetCurrentProcess(), moduleHandle, &modInfo, sizeof(modInfo));

	do
	{
		if (threadEntry.th32OwnerProcessID != dwProcessId)
			continue;

		HANDLE threadHandle = OpenThread(THREAD_ALL_ACCESS, 0, threadEntry.th32ThreadID);
		if (!threadHandle)
			continue;

		ULONG_PTR lpThreadStartAddress = GetThreadStartAddress(threadHandle);
		ULONG_PTR lpModuleBase = reinterpret_cast<ULONG_PTR>(modInfo.lpBaseOfDll);

		if (lpModuleBase < lpThreadStartAddress && lpThreadStartAddress < lpModuleBase + modInfo.SizeOfImage)
			vThreadHandles.push_back(threadHandle);

	} while (Thread32Next(hSnap, &threadEntry));

	CloseHandle(hSnap);
	return vThreadHandles;
}

BOOL ms::ModifyThread(HANDLE threadHandle, THREAD_ACTION actionType)
{
	ULONG dwResult = 0;
	switch (actionType)
	{
	case THREAD_ACTION::RESUME: dwResult = ResumeThread(threadHandle); break;
	case THREAD_ACTION::SUSPEND: dwResult = SuspendThread(threadHandle); break;
	case THREAD_ACTION::TERMINATE: dwResult = TerminateThread(threadHandle, 0); break;
	}

	if (dwResult == 0 || dwResult == -1)
		return FALSE;
	
	return TRUE;
}

BOOL ms::ModifyThread(std::vector<HANDLE> vThread, THREAD_ACTION actionType)
{
	BOOL bResult = 0;
	for (auto& threadHandle : vThread)
		if (threadHandle)
			bResult |= ModifyThread(threadHandle, actionType);

	return bResult;
}