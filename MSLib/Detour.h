#pragma once

enum DETOUR_TYPE
{
	JMP_RELATIVE,
	JMP_ABSOLUTE
};

struct SDetour
{
	std::string szName;
	std::string szModuleName;
	std::string szSignature;
	DWORD dwWriteSize;
	uintptr_t* pulDestination;
	uintptr_t& pulReturn;
	DWORD dwOffset;

	uintptr_t* pulScannedAddr;
	std::vector<BYTE> vOriginalBytes;

	BOOL bAttached = FALSE;
	BOOL bSigAddrFound = FALSE;
	BOOL Attach(DETOUR_TYPE DETOUR_TYPE);
	VOID Detach();
};

std::vector<BYTE> Detour(uintptr_t* pulDestination, uintptr_t* pulSource, size_t ulSize, DETOUR_TYPE jmpType = DETOUR_TYPE::JMP_ABSOLUTE);