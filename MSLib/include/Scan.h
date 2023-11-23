#include <vector>
#include <string>
#include <Windows.h>

#ifndef SCAN_H
#define SCAN_H

namespace ms
{
	class Scan
	{
	private:

	public:

		enum STOP_CONDITION
		{
			FIRST_RESULT,
			NONE
		};

		static std::vector<BYTE> CreateSignature(std::string signatureString);
		static BOOL IsSignatureMatch(PVOID address, std::vector<BYTE>& signature, std::vector<CHAR>& mask);
		static VOID ScanBytes(std::vector<PVOID>* addressResults, std::vector<BYTE>& signatureBytes, std::vector<CHAR>& mask, PVOID region, SIZE_T moduleSize, STOP_CONDITION stopCondition);
		static std::vector<PVOID> AOBScan(std::vector<BYTE>& signature, std::vector<CHAR>& mask, PVOID moduleBase, SIZE_T moduleSize, STOP_CONDITION stopCondition = STOP_CONDITION::FIRST_RESULT);
		static std::vector<PVOID> AOBScan(std::vector<BYTE>& signature, std::vector<CHAR>& mask, std::string moduleName, STOP_CONDITION stopCondition = STOP_CONDITION::FIRST_RESULT);
		static std::vector<PVOID> AOBScan(std::string signatureString, std::string moduleName, STOP_CONDITION stopCondition = STOP_CONDITION::FIRST_RESULT);
		static std::vector<CHAR> CreateMask(std::string signatureString);
	};

}

#endif