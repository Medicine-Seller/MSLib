#pragma once
#include "Scan.h"

namespace ms
{
	struct SPatch
	{
		std::string szName;								// Name of the structure
		std::string szModuleName;						// Name of module at which the signature will be scanned for
		std::string szSignature;						// Signature/pattern of bytes(hex) to be scanned
		std::string szPatchBytes;						// Bytes(hex) to write
		DWORD dwOffset = 0;								// Offset adjustment after signature found
		std::vector<uintptr_t*> vScannedPatternAddrs;
		std::vector<std::vector<BYTE>> vOriginalBytes;

		BOOL bAttached = false;

		// Description: Apply patch
		// [in] stopCondition - Stop condition for patch
		// Return: True if success
		BOOL Attach(STOP_CONDITION stopCondition = STOP_CONDITION::FIRST_RESULT);

		// Description: Restore patch
		VOID Detach();
	};


	// Description: Patch at destination with bytes
	// [in] pulDestination - Pointer to destination
	// [in] pulSource - Pointer to patch bytes
	// [in] ulSize - Number of bytes to write
	// [in] bKeepOldBytes - Define whether to keep original bytes
	// Return: Return overwrriten bytes if bKeepOldBytes is true, else return empty vector
	std::vector<BYTE> Patch(uintptr_t* pulDestination, uintptr_t* pulSource, size_t ulSize, BOOL bKeepOldBytes);
}
