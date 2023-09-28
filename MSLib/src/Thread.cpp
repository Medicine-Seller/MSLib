#include "Thread.h"
#include "Macros.h"
#include <TlHelp32.h>
#include <Psapi.h>

enum THREADINFOCLASS
{
	ThreadQuerySetWin32StartAddress = 9,
};

typedef NTSTATUS(__stdcall* f_NtQueryInformationThread)(HANDLE, THREADINFOCLASS, void*, ULONG_PTR, ULONG_PTR*);

ULONG_PTR ms::GetThreadStartAddress(HANDLE hThread)
{
	HMODULE hNtdll = GetModuleHandleA("ntdll.dll");
	if (hNtdll == INVALID_HANDLE_VALUE)
		return 0;

	auto NtQueryInformationThread = reinterpret_cast<f_NtQueryInformationThread>(GetProcAddress(hNtdll, "NtQueryInformationThread"));
	if (!NtQueryInformationThread)
		return 0;

	ULONG_PTR ulStartAddress = 0;
	NTSTATUS status = NtQueryInformationThread(hThread, ThreadQuerySetWin32StartAddress, &ulStartAddress, sizeof(ULONG_PTR), nullptr);

	if (status)
		return 0;

	return ulStartAddress;
}

VOID ms::ModuleThreads(LPCWSTR lpModuleName, THREAD_ACTION type)
{
	DWORD dwProcessId = GetCurrentProcessId();
	HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, 0);
	HMODULE hModule = GetModuleHandle(lpModuleName);

	if (hSnap == INVALID_HANDLE_VALUE || hModule == INVALID_HANDLE_VALUE)
		return;

	THREADENTRY32 threadEntry;
	threadEntry.dwSize = sizeof(THREADENTRY32);

	MODULEINFO modInfo;
	GetModuleInformation(GetCurrentProcess(), hModule, &modInfo, sizeof(modInfo));

	if (!Thread32First(hSnap, &threadEntry))
	{
		CloseHandle(hSnap);
		return;
	}

	do
	{
		if (threadEntry.th32OwnerProcessID != dwProcessId)
			continue;
		
		HANDLE hThread = OpenThread(THREAD_ALL_ACCESS, 0, threadEntry.th32ThreadID);
		if (hThread == INVALID_HANDLE_VALUE)
			continue;

		ULONG_PTR lpThreadStartAddress = GetThreadStartAddress(hThread);
		ULONG_PTR lpModuleBase = reinterpret_cast<ULONG_PTR>(modInfo.lpBaseOfDll);
		if (lpModuleBase < lpThreadStartAddress && lpThreadStartAddress < lpModuleBase + modInfo.SizeOfImage)
		{
			switch (type)
			{
			case THREAD_ACTION::RESUME: ResumeThread(hThread); break;
			case THREAD_ACTION::SUSPEND: SuspendThread(hThread); break;
			case THREAD_ACTION::TERMINATE: TerminateThread(hThread, 0); break;
			}
#ifdef _DEBUG
			std::cout << COL2("ModuleThreads::Thread modified", reinterpret_cast<uintptr_t*>(lpThreadStartAddress)) << std::endl;
#endif
		}
		CloseHandle(hThread);

	} while (Thread32Next(hSnap, &threadEntry));

	CloseHandle(hSnap);
	return;
}