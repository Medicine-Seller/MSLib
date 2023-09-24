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
	DWORD dwWriteSize = 0;
	uintptr_t* pulDestination = nullptr;
	uintptr_t* pulReturn = nullptr;
	DWORD dwOffset = 0;

	uintptr_t* pulScannedAddr = nullptr;
	std::vector<BYTE> vOriginalBytes;

	BOOL bAttached = FALSE;
	BOOL Attach(DETOUR_TYPE DETOUR_TYPE = DETOUR_TYPE::JMP_ABSOLUTE);
	VOID Detach();
};

std::vector<BYTE> Detour(uintptr_t* pulDestination, uintptr_t* pulSource, size_t ulSize, DETOUR_TYPE jmpType = DETOUR_TYPE::JMP_ABSOLUTE);