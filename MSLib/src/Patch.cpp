#include "Patch.h"
#include "VirtualProtect.h"
#include "Util.h"
#include "Macros.h"


std::vector<BYTE> ms::Patch(uintptr_t* pulDestination, uintptr_t* pulSource, size_t ulSize, BOOL bKeepOldBytes)
{
#ifdef _DEBUG
	std::cout << COL2("Patch1::pulDestination", pulDestination) << std::endl;
	std::cout << COL2("Patch1::pulSource", pulSource) << std::endl;
	std::cout << COL2("Patch1::ulSize", ulSize) << std::endl;
#endif
	std::vector<BYTE> vOriginalBytes;

	PushProtectWrite(pulDestination, ulSize);
	if (bKeepOldBytes)
	{
		vOriginalBytes.resize(ulSize);
		memcpy(vOriginalBytes.data(), pulDestination, ulSize);
	}
	memcpy(pulDestination, pulSource, ulSize);

	PopProtectWrite();
	return vOriginalBytes;
}

BOOL ms::SPatch::Attach(STOP_CONDITION stopCondition)
{
	if (bAttached)
		return TRUE;

	std::vector<CHAR> vMask = GenerateMask(szSignature);
	std::vector<BYTE> vSig = StringToBytes(ReplaceString(szSignature, "?", "0"));

	if (vScannedPatternAddrs.empty())
	{
		std::vector<uintptr_t*> vAddrResults = ModuleAobScan(vSig, vMask, szModuleName, stopCondition);
		if (vAddrResults.empty())
		{
#ifdef _DEBUG
			std::cout << COL2("SPatch::Attach::" + szName, "Can not find pattern") << std::endl;
#endif
			return FALSE;
		}

		for (size_t i = 0; i < vAddrResults.size(); i++)
		{
			vScannedPatternAddrs.push_back(vAddrResults[i]);
			vScannedPatternAddrs[i] = IncrementByByte(vScannedPatternAddrs[i], dwOffset); 
		}
	}

	std::vector<BYTE> vPatchBytes = StringToBytes(szPatchBytes);
	for (auto& addr : vScannedPatternAddrs)
	{
		std::vector<BYTE> vBytesBeforeWrite = Patch(addr, reinterpret_cast<uintptr_t*>(vPatchBytes.data()), vPatchBytes.size(), TRUE);
		vOriginalBytes.push_back(vBytesBeforeWrite);
#ifdef _DEBUG
		std::cout << COL2("SPatch::Attach::" + szName, addr) << std::endl;
#endif
	}

	bAttached = TRUE;
	return TRUE;
}

VOID ms::SPatch::Detach()
{
	if (!bAttached || vScannedPatternAddrs.empty())
		return;

	for (size_t i = 0; i < vScannedPatternAddrs.size(); i++)
	{
		Patch(vScannedPatternAddrs[i], reinterpret_cast<uintptr_t*>(vOriginalBytes[i].data()), vOriginalBytes[i].size(), FALSE);
		vOriginalBytes[i].clear();
#ifdef _DEBUG
		std::cout << COL2("SPatch::Detach::" + szName, "Detached") << std::endl;
#endif
	}

	vOriginalBytes.clear();
	bAttached = FALSE;
}
