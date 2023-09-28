#include "VirtualProtect.h"

VOID ms::PushProtectWrite(uintptr_t* pulAddress, size_t ulSize)
{
	g_vProtectList.push_back({ pulAddress, ulSize, 0 });
	VirtualProtect(pulAddress, ulSize, PAGE_EXECUTE_READWRITE, &g_vProtectList[g_vProtectList.size() - 1].dwOriginalProtect);
}

VOID ms::PopProtectWrite()
{
	if (g_vProtectList.empty())
		return;

	SProtect& orignalProtect = g_vProtectList[g_vProtectList.size() - 1];
	VirtualProtect(orignalProtect.pulAddress, orignalProtect.ulSize, orignalProtect.dwOriginalProtect, NULL);
	g_vProtectList.pop_back();
}

VOID ms::RestoreAllWritables()
{
	for (auto& protect : g_vProtectList)
		VirtualProtect(protect.pulAddress, protect.ulSize, protect.dwOriginalProtect, NULL);

	g_vProtectList.clear();
}
