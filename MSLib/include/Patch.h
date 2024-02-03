#include <Windows.h>
#include <vector>
#include <string>
#include "Scan.h"

#ifndef PATCH_H
#define PATCH_H

namespace ms
{
	class Patch
	{
	private:

		struct PatchAOBBase
		{
			std::vector<BYTE> PatchBytes;
			LONG Offset = 0;
			BOOL IsAttached = FALSE;
			std::vector<PVOID> DestinationList;
			std::vector<std::vector<BYTE>> OriginalBytesList;
		};

	public:

		struct PatchInfo
		{
			PVOID Destination = 0;
			std::vector<BYTE> WriteBytes;
			BOOL IsAttached = FALSE;
			std::vector<BYTE> OriginalBytes;
		};

		struct PatchInfoAOB : PatchAOBBase
		{
			std::vector<BYTE> Signature;
			std::vector<CHAR> Mask;
			PVOID Module = 0;
		};

		struct PatchInfoAOBString : PatchInfoAOB
		{
			std::string SignatureString;
			std::string ModuleString;
			std::string PatchBytesString;
		};

		static NTSTATUS Attach(PatchInfo& info);
		static NTSTATUS Attach(PatchInfoAOB& info, const Scan::STOP_CONDITION patchType = Scan::STOP_CONDITION::FIRST_RESULT);
		static NTSTATUS Attach(PatchInfoAOBString& info, const Scan::STOP_CONDITION patchType = Scan::STOP_CONDITION::FIRST_RESULT);
		static NTSTATUS Detach(PatchInfo& info);
		static NTSTATUS Detach(PatchAOBBase& info);
		static NTSTATUS PatchBytes(const PVOID destination, const std::vector<BYTE>* patchBytes, std::vector<BYTE>* originalBytes);
		static NTSTATUS PatchBytes(const PVOID destination, const PVOID source, const SIZE_T size, std::vector<BYTE>* originalBytes);
	};
}

#endif