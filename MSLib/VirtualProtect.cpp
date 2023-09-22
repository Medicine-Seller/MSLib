#include "MainHeader.h"

VOID ms::PushProtectWrite(uintptr_t* pulAddress, size_t ulSize)
{
	vProtectList.push_back({ pulAddress, ulSize, 0 });
	VirtualProtect(pulAddress, ulSize, PAGE_EXECUTE_READWRITE, &vProtectList[vProtectList.size() - 1].dwOriginalProtect);
}

VOID ms::PopProtectWrite()
{
	if (vProtectList.empty())
		return;

	SProtect& orignalProtect = vProtectList[vProtectList.size() - 1];
	VirtualProtect(orignalProtect.pulAddress, orignalProtect.ulSize, orignalProtect.dwOriginalProtect, NULL);
	vProtectList.pop_back();
}

VOID ms::RestoreAllWritables()
{
	for (auto& protect : vProtectList)
		VirtualProtect(protect.pulAddress, protect.ulSize, protect.dwOriginalProtect, NULL);

	vProtectList.clear();
}
