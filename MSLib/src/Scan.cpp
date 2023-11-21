#include "Scan.h"
#include "Util.h"
#include "Macros.h"
#include <Psapi.h>

bool ms::IsSignatureMatch(std::vector<BYTE>& signatureBytes, std::vector<CHAR>& mask, uintptr_t* address)
{
	bool bIsSignatureMatch = true;
	for (SIZE_T i = 0; i < mask.size(); i++)
	{
		if (mask[i] != '?' && signatureBytes[i] != *(reinterpret_cast<BYTE*>(address) + i))
		{
			bIsSignatureMatch = false;
			break;
		}
	}

	return bIsSignatureMatch;
}

VOID ms::Scan(std::vector<uintptr_t*>* addressResult, std::vector<BYTE>& signatureBytes, std::vector<CHAR>& mask, uintptr_t* region, SIZE_T moduleSize , STOP_CONDITION stopCondition)
{
	for (ULONG Offset = 0; Offset < moduleSize; Offset++)
	{
		uintptr_t* address = IncrementByByte(region, Offset);
		if (!IsSignatureMatch(signatureBytes, mask, address))
			continue;

		addressResult->push_back(address);

		if (stopCondition == STOP_CONDITION::FIRST_RESULT)
			break;
	}

	return;
}


std::vector<uintptr_t*> ms::AOBScan(std::vector<BYTE>& signatureBytes, std::vector<CHAR>& mask, uintptr_t* moduleBase, SIZE_T moduleSize, STOP_CONDITION stopCondition)
{
	MEMORY_BASIC_INFORMATION mbi;
	std::vector<uintptr_t*> vAddrResults;

	for (uintptr_t* region = moduleBase; region < moduleBase + moduleSize; region = IncrementByByte(region, mbi.RegionSize))
	{
		if (!VirtualQuery(region, &mbi, sizeof(mbi)) || mbi.State != MEM_COMMIT || mbi.Protect == PAGE_NOACCESS)
			continue;

		Scan(&vAddrResults, signatureBytes, mask, region, mbi.RegionSize, stopCondition);

		if (stopCondition == STOP_CONDITION::NONE)
			continue;
		else if (!vAddrResults.empty())
			break;
	}

	return vAddrResults;
}

std::vector<uintptr_t*> ms::AOBScanModule(std::vector<BYTE>& signatureBytes, std::vector<CHAR>& mask, std::string moduleName, STOP_CONDITION stopCondition)
{
	HMODULE moduleHandle = GetModuleHandleA(moduleName.c_str());
	if (!moduleHandle)
		return {};

	MODULEINFO ModInfo = GetModuleInfo(moduleHandle);

	return AOBScan(signatureBytes, mask, reinterpret_cast<uintptr_t*>(ModInfo.lpBaseOfDll), ModInfo.SizeOfImage, stopCondition);
}

std::vector<uintptr_t*> ms::AOBScanString(std::string signature, std::string moduleName, STOP_CONDITION stopCondition)
{
	std::vector<CHAR> mask = GenerateMask(signature);
	std::vector<BYTE> signatureBytes = StringToBytes(ReplaceString(signature, "?", "0"));

	return AOBScanModule(signatureBytes, mask, moduleName, stopCondition);
}


