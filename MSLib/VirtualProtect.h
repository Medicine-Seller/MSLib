#pragma once

struct SProtect
{
	uintptr_t* pulAddress;
	size_t ulSize;
	DWORD dwOriginalProtect;
};

static std::vector<SProtect> vProtectList;

VOID PushProtectWrite(uintptr_t* pulAddress, size_t ulSize);
VOID PopProtectWrite();
VOID RestoreAllWritables();