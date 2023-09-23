#include "MSLib.h"

bool ms::SigMatched(std::vector<BYTE>& vSig, std::vector<CHAR>& vMask, uintptr_t* pulScanBeginAddr)
{
	bool bSigMatched = true;
	for (size_t i = 0; i < vMask.size(); i++)
	{
		if (vMask[i] != '?' && vSig[i] != *(reinterpret_cast<BYTE*>(pulScanBeginAddr) + i))
		{
			bSigMatched = false;
			break;
		}
	}

	return bSigMatched;
}

VOID ms::Scan(std::vector<uintptr_t*>* pAddrResults, std::vector<BYTE>& vSig, std::vector<CHAR>& vMask, uintptr_t* pulRegion, SIZE_T ulModuleSize , STOP_CONDITION stopCondition)
{
	for (DWORD dwOffset = 0; dwOffset < ulModuleSize; dwOffset++)
	{
		uintptr_t* pulScanBeginAddr = reinterpret_cast<uintptr_t*>(reinterpret_cast<BYTE*>(pulRegion) + dwOffset);
		if (!SigMatched(vSig, vMask, pulScanBeginAddr))
			continue;

		pAddrResults->push_back(pulScanBeginAddr);

		if (stopCondition == STOP_CONDITION::FIRST_RESULT)
			break;
	}

	return;
}

std::vector<uintptr_t*> ms::AobScan(std::vector<BYTE>& vSig, std::vector<CHAR>& vMask, uintptr_t* ulModuleBase, SIZE_T ulModuleSize, STOP_CONDITION stopCondition)
{

#ifdef _DEBUG
	std::cout << COL2("AobScan::ulModuleBase", ulModuleBase) << std::endl;
	std::cout << COL2("AobScan::ulModuleSize", reinterpret_cast<uintptr_t*>(ulModuleSize)) << std::endl;
	std::cout << COL1("AobScan::vSig");
	for (int e : vSig)
	{
		std::cout << std::hex << e << " ";
	}
	std::cout << std::endl;
	std::cout << COL1("AobScan::vMask");
	for (auto& e : vMask)
	{
		std::cout << e;
	}
	std::cout << std::endl;
#endif

	MEMORY_BASIC_INFORMATION mbi;
	std::vector<uintptr_t*> vAddrResults;

	for (uintptr_t* pulRegion = ulModuleBase; pulRegion < ulModuleBase + ulModuleSize; pulRegion = reinterpret_cast<uintptr_t*>(reinterpret_cast<BYTE*>(pulRegion) + mbi.RegionSize))
	{
		if (!VirtualQuery(pulRegion, &mbi, sizeof(mbi)) || mbi.State != MEM_COMMIT || mbi.Protect == PAGE_NOACCESS)
			continue;

		Scan(&vAddrResults, vSig, vMask, pulRegion, mbi.RegionSize, stopCondition);

		if (stopCondition == STOP_CONDITION::NONE)
			continue;
		else if (!vAddrResults.empty())
			break;
	}

	return vAddrResults;
}

std::vector<uintptr_t*> ms::ModuleAobScan(std::vector<BYTE>& vSig, std::vector<CHAR>& vMask, std::string szModuleName, STOP_CONDITION stopCondition)
{
	std::wstring wModuleName = std::wstring(szModuleName.begin(), szModuleName.end());
	LPCWSTR lpModuleName = wModuleName.c_str();

	HMODULE hModule = GetModuleHandle(lpModuleName);
	if (hModule == INVALID_HANDLE_VALUE)
		return {};

	MODULEINFO ModInfo;
	GetModuleInformation(GetCurrentProcess(), hModule, &ModInfo, sizeof(MODULEINFO));

	return AobScan(vSig, vMask, reinterpret_cast<uintptr_t*>(ModInfo.lpBaseOfDll), ModInfo.SizeOfImage, stopCondition);
}

std::vector<uintptr_t*> ms::StringAobScan(std::string szSig, std::string szModuleName, STOP_CONDITION stopCondition)
{
	std::vector<CHAR> vMask = GenerateMask(szSig);
	std::vector<BYTE> vSig = StringToBytes(ReplaceString(szSig, "?", "0"));

	return ModuleAobScan(vSig, vMask, szModuleName, stopCondition);
}


