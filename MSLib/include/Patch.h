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
			LONG Offset;
			BOOL IsAttached = FALSE;
			std::vector<PVOID> DestinationList;
			std::vector<std::vector<BYTE>> OriginalBytesList;
		};

	public:

		struct PatchInfo
		{
			PVOID Destination;
			std::vector<BYTE> WriteBytes;
			BOOL IsAttached = FALSE;
			std::vector<BYTE> OriginalBytes;
		};

		struct PatchInfoAOB : PatchAOBBase
		{
			std::vector<BYTE> Signature;
			std::vector<CHAR> Mask;
			PVOID Module;
		};

		struct PatchInfoAOBString : PatchInfoAOB
		{
			std::string SignatureString;
			std::string ModuleString;
			std::string PatchBytesString;
		};

		static NTSTATUS Attach(PatchInfo& info);
		static NTSTATUS Attach(PatchInfoAOB& info, Scan::STOP_CONDITION patchType = Scan::STOP_CONDITION::FIRST_RESULT);
		static NTSTATUS Attach(PatchInfoAOBString& info, Scan::STOP_CONDITION patchType = Scan::STOP_CONDITION::FIRST_RESULT);
		static NTSTATUS Detach(PatchInfo& info);
		static NTSTATUS Detach(PatchAOBBase& info);
		static NTSTATUS PatchBytes(PVOID destination, std::vector<BYTE>* patchBytes, std::vector<BYTE>* originalBytes);
	};
}

#endif