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

		static std::vector<BYTE> CreateSignature(const std::string signatureString);
		static BOOL IsSignatureMatch(const PVOID address, const std::vector<BYTE>& signature, const std::vector<CHAR>& mask);
		static VOID ScanBytes(std::vector<PVOID>* addressResults, const std::vector<BYTE>& signature, const std::vector<CHAR>& mask, const PVOID region, const SIZE_T moduleSize, const STOP_CONDITION stopCondition);
		static std::vector<PVOID> AOBScan(const std::vector<BYTE>& signature, const std::vector<CHAR>& mask, const PVOID moduleBase, const SIZE_T moduleSize, const STOP_CONDITION stopCondition = STOP_CONDITION::FIRST_RESULT);
		static std::vector<PVOID> AOBScan(const std::vector<BYTE>& signature, const std::vector<CHAR>& mask, const std::string moduleName, const STOP_CONDITION stopCondition = STOP_CONDITION::FIRST_RESULT);
		static std::vector<PVOID> AOBScan(const std::string signatureString, const std::string moduleName, const STOP_CONDITION stopCondition = STOP_CONDITION::FIRST_RESULT);
		static std::vector<CHAR> CreateMask(const std::string signatureString);
	};

}

#endif