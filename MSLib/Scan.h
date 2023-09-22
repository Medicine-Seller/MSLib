#pragma once

enum STOP_CONDITION
{
	FIRST_RESULT,
	NONE
};

bool SigMatched(std::vector<BYTE>& vSig, std::vector<CHAR>& vMask, uintptr_t* pulScanBeginAddr);
VOID Scan(std::vector<uintptr_t*>* pAddrResults, std::vector<BYTE>& vSig, std::vector<CHAR>& vMask, uintptr_t* pulRegion, SIZE_T ulModuleSize, STOP_CONDITION stopCondition);
std::vector<uintptr_t*> AobScan(std::vector<BYTE>& vSig, std::vector<CHAR>& vMask, uintptr_t* ulModuleBase, SIZE_T ulModuleSize, STOP_CONDITION stopCondition = STOP_CONDITION::FIRST_RESULT);
std::vector<uintptr_t*> ModuleAobScan(std::vector<BYTE>& vSig, std::vector<CHAR>& vMask, std::string szModuleName, STOP_CONDITION stopCondition = STOP_CONDITION::FIRST_RESULT);
std::vector<uintptr_t*> StringAobScan(std::string szSig, std::string szModuleName, STOP_CONDITION stopCondition = STOP_CONDITION::FIRST_RESULT);

