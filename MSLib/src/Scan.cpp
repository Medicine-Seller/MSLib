#include "Scan.h"
#include "Util.h"
#include <Psapi.h>
#include "Logger.h"

BOOL ms::Scan::IsSignatureMatch(PVOID address, std::vector<BYTE>& signature, std::vector<CHAR>& mask)
{
	BOOL IsSignatureMatch = TRUE;
	for (SIZE_T i = 0; i < mask.size(); i++)
	{
		if (mask[i] != '?' && signature[i] != *(reinterpret_cast<BYTE*>(address) + i))
		{
			IsSignatureMatch = FALSE;
			break;
		}
	}

	return IsSignatureMatch;
}

VOID ms::Scan::ScanBytes(std::vector<PVOID>* addressResults, std::vector<BYTE>& signature, std::vector<CHAR>& mask, PVOID region, SIZE_T moduleSize , STOP_CONDITION stopCondition)
{
	for (ULONG Offset = 0; Offset < moduleSize; Offset++)
	{
		PVOID address = IncrementByByte(region, Offset);
		if (!IsSignatureMatch(address, signature, mask))
			continue;

		addressResults->push_back(address);

		if (stopCondition == STOP_CONDITION::FIRST_RESULT)
			break;
	}
}

std::vector<PVOID> ms::Scan::AOBScan(std::vector<BYTE>& signature, std::vector<CHAR>& mask, PVOID moduleBase, SIZE_T moduleSize, STOP_CONDITION stopCondition)
{
	MEMORY_BASIC_INFORMATION mbi;
	std::vector<PVOID> addressResults;
	
	uintptr_t* regionStart = reinterpret_cast<uintptr_t*>(moduleBase);
	uintptr_t* regionEnd = IncrementByByte(regionStart, moduleSize);

	for (uintptr_t* region = regionStart; region < regionEnd; region = IncrementByByte(region, mbi.RegionSize))
	{
		if (!VirtualQuery(region, &mbi, sizeof(mbi)) || mbi.State != MEM_COMMIT || mbi.Protect == PAGE_NOACCESS)
			continue;

		ScanBytes(&addressResults, signature, mask, region, mbi.RegionSize, stopCondition);

		if (stopCondition == STOP_CONDITION::NONE)
			continue;
		else if (!addressResults.empty())
			break;
	}

	return addressResults;
}

std::vector<PVOID> ms::Scan::AOBScan(std::vector<BYTE>& signature, std::vector<CHAR>& mask, std::string moduleName, STOP_CONDITION stopCondition)
{
	HMODULE moduleHandle = GetModuleHandleA(moduleName.c_str());
	if (!moduleHandle)
		return {};

	MODULEINFO ModInfo = GetModuleInfo(moduleHandle);

	return AOBScan(signature, mask, ModInfo.lpBaseOfDll, ModInfo.SizeOfImage, stopCondition);
}

std::vector<PVOID> ms::Scan::AOBScan(std::string signatureString, std::string moduleName, STOP_CONDITION stopCondition)
{
	std::vector<CHAR> mask = CreateMask(signatureString);
	std::vector<BYTE> signature = CreateSignature(signatureString);

	return AOBScan(signature, mask, moduleName, stopCondition);
}

std::vector<CHAR> ms::Scan::CreateMask(std::string signatureString)
{
	std::stringstream ss(signatureString);
	std::string str;
	std::vector<CHAR> mask;

	while (ss >> str)
	{
		if (str == "??" || str == "?")
			mask.push_back('?');
		else
			mask.push_back('x');
	}

	return mask;
}

std::vector<BYTE> ms::Scan::CreateSignature(std::string signatureString)
{
	return StringToBytes(ReplaceString(signatureString, "?", "0"));
}