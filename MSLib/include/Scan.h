#pragma once
#include <vector>
#include <string>
#include <Windows.h>

namespace ms
{
	enum STOP_CONDITION
	{
		FIRST_RESULT,
		NONE
	};

	// Description: Check if signature matches
	// [in] vSig - Vector of bytes signature
	// [in] vMask - Vector of char mask
	// [in] pulScanBeginAddr - The address at which the comparison starts
	// Return: True if the signature matches the address bytes
	bool SigMatched(std::vector<BYTE>& vSig, std::vector<CHAR>& vMask, uintptr_t* pulScanBeginAddr);
	
	// Description: Scan for signature
	// [in] pAddrResults - A pointer to a vector where scanned addresses will be stored
	// [in] vSig - Vector bytes signature
	// [in] vMask - Vector of char mask
	// [in] pulRegion - Pointer to memory region at which the scan will take place
	// [in] ulModuleSize - The size of the module
	// [in] stopCondition - Defines whether to stop at first scan or not
	VOID Scan(std::vector<uintptr_t*>* pAddrResults, std::vector<BYTE>& vSig, std::vector<CHAR>& vMask, uintptr_t* pulRegion, SIZE_T ulModuleSize, STOP_CONDITION stopCondition);
	
	// Description: Array of byte scan
	// [in] vSig - Vector bytes signature
	// [in] vMask - Vector of char mask
	// [in] ulModuleBase - Pointer to module base address
	// [in] ulModuleSize - Size of module
	// [in][optional] stopCondition - Defines whether to stop at first scan or not
	// Return: Vector of pointers where the signature is found
	std::vector<uintptr_t*> AobScan(std::vector<BYTE>& vSig, std::vector<CHAR>& vMask, uintptr_t* ulModuleBase, SIZE_T ulModuleSize, STOP_CONDITION stopCondition = STOP_CONDITION::FIRST_RESULT);
	
	// Description: Module scan for signature
	// [in] vSig - Vector bytes signature
	// [in] vMask - Vector of char mask
	// [in] szModuleName - Module name
	// [in][optional] stopCondition - Defines whether to stop at first scan or not
	// Return: Vector of pointers where the signature is found
	std::vector<uintptr_t*> ModuleAobScan(std::vector<BYTE>& vSig, std::vector<CHAR>& vMask, std::string szModuleName, STOP_CONDITION stopCondition = STOP_CONDITION::FIRST_RESULT);

	// Description: String AOB scan for signature
	// [in] szSig - String signature of bytes
	// [in] szModuleName - Module name
	// [in][optional] stopCondition - Defines whether to stop at first scan or not
	// Return: Vector of pointers where the signature is found
	std::vector<uintptr_t*> StringAobScan(std::string szSig, std::string szModuleName, STOP_CONDITION stopCondition = STOP_CONDITION::FIRST_RESULT);
}
