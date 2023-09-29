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

	// Description: Overwrite memory protection for Read/Write and pop onto list
	// [in] pulAddress - Pointer to address/location
	// [in] ulSize - Size of bytes to overwrite protection
	VOID PushProtectWrite(uintptr_t* pulAddress, size_t ulSize);

	// Description: Restore memory protection and pop off list
	VOID PopProtectWrite();

	// Description: Restore all memory protections and clear list
	VOID RestoreAllWritables();
}