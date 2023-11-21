#include "Scan.h"

#ifndef PATCH_H
#define PATCH_H

namespace ms
{
	struct SPatch
	{
		std::string Name;								// Name of the structure
		std::string ModuleName;						// Name of module at which the signature will be scanned for
		std::string Signature;						// Signature/pattern of bytes(hex) to be scanned
		std::string PatchBytes;						// Bytes(hex) to write
		LONG Offset = 0;								// Offset adjustment after signature found
		std::vector<uintptr_t*> ScannedAddresses;
		std::vector<std::vector<BYTE>> OriginalBytes;

		BOOL IsAttached = false;

		// Description: Apply patch
		// [in] stopCondition - Stop condition for patch
		// Return: True if success
		NTSTATUS Attach(STOP_CONDITION stopCondition = STOP_CONDITION::FIRST_RESULT);

		// Description: Restore patch
		VOID Detach();
	};


	// Description: Patch at destination with bytes
	// [in] destination - Pointer to destination
	// [in] source - Pointer to patch bytes
	// [in] writeSize - Number of bytes to write
	// [in] bKeepOldBytes - Define whether to keep original bytes
	// Return: Return overwrriten bytes if bKeepOldBytes is true, else return empty vector
	NTSTATUS Patch(uintptr_t* destination, uintptr_t* source, SIZE_T writeSize, std::vector<BYTE>* originalBytes);
}

#endif