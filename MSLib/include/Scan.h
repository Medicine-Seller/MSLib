#include <vector>
#include <string>
#include <Windows.h>

#ifndef SCAN_H
#define SCAN_H

namespace ms
{
	enum STOP_CONDITION
	{
		FIRST_RESULT,
		NONE
	};

	// Description: Check if signature matches
	// [in] signature - Vector of bytes signature
	// [in] mask - Vector of char mask
	// [in] address - The address at which the comparison starts
	// Return: True if the signature matches the address bytes
	bool IsSignatureMatch(std::vector<BYTE>& signatureBytes, std::vector<CHAR>& mask, uintptr_t* address);
	
	// Description: Scan for signature
	// [in] addressResult - A pointer to a vector where scanned addresses will be stored
	// [in] signature - Vector bytes signature
	// [in] mask - Vector of char mask
	// [in] region - Pointer to memory region at which the scan will take place
	// [in] moduleSize - The size of the module
	// [in] stopCondition - Defines whether to stop at first scan or not
	VOID Scan(std::vector<uintptr_t*>* addressResult, std::vector<BYTE>& signatureBytes, std::vector<CHAR>& mask, uintptr_t* region, SIZE_T moduleSize, STOP_CONDITION stopCondition);
	
	// Description: Array of byte scan
	// [in] signature - Vector bytes signature
	// [in] mask - Vector of char mask
	// [in] moduleBase - Pointer to module base address
	// [in] moduleSize - Size of module
	// [in][optional] stopCondition - Defines whether to stop at first scan or not
	// Return: Vector of pointers where the signature is found
	std::vector<uintptr_t*> AOBScan(std::vector<BYTE>& signatureBytes, std::vector<CHAR>& mask, uintptr_t* moduleBase, SIZE_T moduleSize, STOP_CONDITION stopCondition = STOP_CONDITION::FIRST_RESULT);
	
	// Description: Module scan for signature
	// [in] signature - Vector bytes signature
	// [in] mask - Vector of char mask
	// [in] moduleName - Module name
	// [in][optional] stopCondition - Defines whether to stop at first scan or not
	// Return: Vector of pointers where the signature is found
	std::vector<uintptr_t*> AOBScanModule(std::vector<BYTE>& signatureBytes, std::vector<CHAR>& mask, std::string moduleName, STOP_CONDITION stopCondition = STOP_CONDITION::FIRST_RESULT);

	// Description: String AOB scan for signature
	// [in] signature - String signature of bytes
	// [in] moduleName - Module name
	// [in][optional] stopCondition - Defines whether to stop at first scan or not
	// Return: Vector of pointers where the signature is found
	std::vector<uintptr_t*> AOBScanString(std::string signature, std::string moduleName, STOP_CONDITION stopCondition = STOP_CONDITION::FIRST_RESULT);
}

#endif