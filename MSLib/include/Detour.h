#include "Scan.h"

#ifndef DETOUR_H
#define DETOUR_H

namespace ms
{
	enum DETOUR_TYPE
	{
		JMP_RELATIVE,
		JMP_ABSOLUTE
	};

	struct DetourStructure
	{
		std::string Name;						// Name of the structure
		std::string ModuleName;				// Name of module at which the signature will be scanned for
		std::string Signature;				// Signature/pattern of bytes(hex) to be scanned
		ULONG WriteSize = 0;					// Write size, minimum for relative jmp is 5, and 14 for absolute
		uintptr_t* Destination = nullptr;	// Pointer to destination
		uintptr_t* ReturnAddress = nullptr;			// Pointer to variable to store return location after a detour
		LONG Offset = 0;						// Offset adjustment after signature found

		uintptr_t* Source = nullptr;
		std::vector<BYTE> OriginalBytes;

		BOOL IsAttached = FALSE;

		// Description: Attach the detour
		// [in] DETOUR_TYPE - Jump type
		// Return: True if success
		NTSTATUS Attach(DETOUR_TYPE DETOUR_TYPE = DETOUR_TYPE::JMP_ABSOLUTE);

		// Description: Detach the detour
		VOID Detach();
	};

	// Description: Create a detour at source
	// [in] destination - Pointer to destination
	// [in] source - Pointer to where the detour will be created
	// [in] writeSize - Overwrite size to create the detour, at least 5 for relative and 14 for absolute jump
	// [in][optional] jumpType - Type of jump
	// Return: Vector of overwritten bytes
	NTSTATUS Detour(uintptr_t* destination, uintptr_t* source, SIZE_T writeSize, std::vector<BYTE>* originalBytes, DETOUR_TYPE jumpType = DETOUR_TYPE::JMP_ABSOLUTE);
}

#endif