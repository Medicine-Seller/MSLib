#pragma once
#include <Windows.h>
#include <vector>

namespace ms
{
	struct SProtect
	{
		uintptr_t* pulAddress;
		size_t ulSize;
		DWORD dwOriginalProtect;
	};

	static std::vector<SProtect> g_vProtectList;

	VOID PushProtectWrite(uintptr_t* pulAddress, size_t ulSize);
	VOID PopProtectWrite();
	VOID RestoreAllWritables();
}