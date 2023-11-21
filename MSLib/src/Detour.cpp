#include "Macros.h"
#include "Detour.h"
#include "VirtualProtect.h"
#include "Patch.h"
#include "Util.h"


NTSTATUS ms::Detour(uintptr_t* destination, uintptr_t* source, SIZE_T writeSize, std::vector<BYTE>* originalBytes, DETOUR_TYPE jumpType)
{
	static const BYTE relativeJump[] = "\xE9";
	static const BYTE absoluteJump[] = "\xFF\x25\x00\x00\x00\x00";

	if (jumpType == JMP_RELATIVE && writeSize < 5)
		return -1;
	else if (jumpType == JMP_ABSOLUTE && writeSize < 14)
		return -1;

	std::vector<BYTE> jumpShellCode(writeSize, 0x90);

	if (jumpType == JMP_RELATIVE)
	{
		memcpy(jumpShellCode.data(), relativeJump, 1);
		uintptr_t jumpLocation = reinterpret_cast<uintptr_t>(source) - reinterpret_cast<uintptr_t>(jumpShellCode.data()) - 5;
		*reinterpret_cast<uintptr_t*>(jumpShellCode.data() + 1) = jumpLocation;
	}
	else if (jumpType == JMP_ABSOLUTE)
	{
		memcpy(jumpShellCode.data(), absoluteJump, 6);
		*reinterpret_cast<uintptr_t*>(jumpShellCode.data() + 6) = reinterpret_cast<uintptr_t>(source);
	}
	else
	{
		return -1;
	}

	NTSTATUS status = PushProtectWrite(destination, writeSize, PAGE_EXECUTE_READWRITE);
	if (status != STATUS_SUCCESS)
		return status;

	status = Patch(destination, reinterpret_cast<uintptr_t*>(jumpShellCode.data()), jumpShellCode.size(), originalBytes);
	if (status != STATUS_SUCCESS)
		return status;

	PopProtectWrite();

	return STATUS_SUCCESS;
}

NTSTATUS ms::DetourStructure::Attach(DETOUR_TYPE jumpType)
{
	if (IsAttached)
		return STATUS_SUCCESS;

	if (Source == nullptr)
	{
		std::vector<uintptr_t*> vAddrResults = AOBScanString(Signature, ModuleName);
		if (vAddrResults.empty())
			return -1;

		Source = IncrementByByte(vAddrResults[0], Offset);
		*ReturnAddress = reinterpret_cast<uintptr_t>(IncrementByByte(Source, WriteSize));
	}

	NTSTATUS status = Detour(Source, Destination, WriteSize, &OriginalBytes, jumpType);
	if (status != STATUS_SUCCESS)
		return status;

	IsAttached = TRUE;
	return STATUS_SUCCESS;
}

VOID ms::DetourStructure::Detach()
{
	if (!IsAttached || Source == nullptr)
		return;

	Patch(Source, reinterpret_cast<uintptr_t*>(OriginalBytes.data()), WriteSize, nullptr);

	OriginalBytes.clear();
	IsAttached = FALSE;
}