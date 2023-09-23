#include "MSLib.h"

enum THREADINFOCLASS
{
	ThreadQuerySetWin32StartAddress = 9,
};

typedef NTSTATUS(__stdcall* f_NtQueryInformationThread)(HANDLE, THREADINFOCLASS, void*, ULONG_PTR, ULONG_PTR*);

ULONG_PTR ms::GetThreadStartAddress(HANDLE hThread)
{
	auto NtQueryInformationThread = reinterpret_cast<f_NtQueryInformationThread>(GetProcAddress(GetModuleHandleA("ntdll.dll"), "NtQueryInformationThread"));
	if (!NtQueryInformationThread)
		return 0;

	ULONG_PTR ulStartAddress = 0;
	NTSTATUS Ret = NtQueryInformationThread(hThread, ThreadQuerySetWin32StartAddress, &ulStartAddress, sizeof(ULONG_PTR), nullptr);

	if (Ret)
		return 0;

	return ulStartAddress;
}

VOID ms::ModuleThreads(LPCWSTR lpModuleName, THREAD_ACTION threadAction)
{
	DWORD dwProcessId = GetCurrentProcessId();
	HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, 0);
	HMODULE hModule = GetModuleHandle(lpModuleName);

	if (hSnap == INVALID_HANDLE_VALUE || hModule == INVALID_HANDLE_VALUE)
		return;

	THREADENTRY32 threadEntry;
	threadEntry.dwSize = sizeof(THREADENTRY32);

	MODULEINFO lpModInfo;
	GetModuleInformation(GetCurrentProcess(), hModule, &lpModInfo, sizeof(lpModInfo));

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

		ULONG_PTR ulThreadStartAddress = GetThreadStartAddress(hThread);
		ULONG_PTR ulModuleBase = reinterpret_cast<ULONG_PTR>(lpModInfo.lpBaseOfDll);
		if (ulModuleBase < ulThreadStartAddress && ulThreadStartAddress < ulModuleBase + lpModInfo.SizeOfImage)
		{
			switch (threadAction)
			{
			case THREAD_ACTION::RESUME: ResumeThread(hThread); break;
			case THREAD_ACTION::SUSPEND: SuspendThread(hThread); break;
			case THREAD_ACTION::TERMINATE: TerminateThread(hThread, 0); break;
			}
#ifdef ENABLE_LOGGING
			std::cout << COL2("ModuleThreads::" + ulThreadStartAddress, "Modified");
#endif
		}
		CloseHandle(hThread);
	} while (Thread32Next(hSnap, &threadEntry));

	CloseHandle(hSnap);
	return;
}