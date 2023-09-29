#pragma once
#include "Scan.h"

namespace ms
{
	enum DETOUR_TYPE
	{
		JMP_RELATIVE,
		JMP_ABSOLUTE
	};

	struct SDetour
	{
		std::string szName;						// Name of the structure
		std::string szModuleName;				// Name of module at which the signature will be scanned for
		std::string szSignature;				// Signature/pattern of bytes(hex) to be scanned
		DWORD dwWriteSize = 0;					// Write size, minimum for relative jmp is 5, and 14 for absolute
		uintptr_t* pulDestination = nullptr;	// Pointer to destination
		uintptr_t* pulReturn = nullptr;			// Pointer to variable to store return location after a detour
		DWORD dwOffset = 0;						// Offset adjustment after signature found

		uintptr_t* pulScannedAddr = nullptr;
		std::vector<BYTE> vOriginalBytes;

		BOOL bAttached = FALSE;

		// Description: Attach the detour
		// [in] DETOUR_TYPE - Jump type
		// Return: True if success
		BOOL Attach(DETOUR_TYPE DETOUR_TYPE = DETOUR_TYPE::JMP_ABSOLUTE);

		// Description: Detach the detour
		VOID Detach();
	};

	// Description: Create a detour at source
	// [in] pulDestination - Pointer to destination
	// [in] pulSource - Pointer to where the detour will be created
	// [in] ulSize - Overwrite size to create the detour, at least 5 for relative and 14 for absolute jump
	// [in][optional] jmpType - Type of jump
	// Return: Vector of overwritten bytes
	std::vector<BYTE> Detour(uintptr_t* pulDestination, uintptr_t* pulSource, size_t ulSize, DETOUR_TYPE jmpType = DETOUR_TYPE::JMP_ABSOLUTE);
}