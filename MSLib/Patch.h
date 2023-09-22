#pragma once

struct SPatch
{
	std::string szName;
	std::string szModuleName;
	std::string szSignature;
	std::string szPatchBytes;
	DWORD dwOffset;
	std::vector<uintptr_t*> vScannedPatternAddrs;
	std::vector<std::vector<BYTE>> vOriginalBytes;

	BOOL bAttached = false;
	BOOL bPatternFound = false;
	BOOL Attach(STOP_CONDITION stopCondition);
	VOID Detach();
};

std::vector<BYTE> Patch(uintptr_t* pulDestination, uintptr_t* pulSource, size_t ulSize, BOOL bKeepOldBytes);