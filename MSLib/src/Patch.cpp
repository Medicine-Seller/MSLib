#include "Macros.h"
#include "Patch.h"
#include "VirtualProtect.h"
#include "Util.h"

NTSTATUS ms::Patch(uintptr_t* destination, uintptr_t* source, SIZE_T writeSize, std::vector<BYTE> *originalBytes)
{
	NTSTATUS status = PushProtectWrite(destination, writeSize, PAGE_EXECUTE_READWRITE);
	if (status != STATUS_SUCCESS)
		return status;

	if (originalBytes != nullptr)
	{
		originalBytes->resize(writeSize);
		memcpy(originalBytes->data(), destination, writeSize);
	}
	memcpy(destination, source, writeSize);

	PopProtectWrite();

	return STATUS_SUCCESS;
}

NTSTATUS ms::SPatch::Attach(STOP_CONDITION stopCondition)
{
	if (IsAttached)
		return STATUS_SUCCESS;

	std::vector<CHAR> mask = GenerateMask(Signature);
	std::vector<BYTE> signature = StringToBytes(ReplaceString(Signature, "?", "0"));

	if (ScannedAddresses.empty())
	{
		std::vector<uintptr_t*> vAddrResults = AOBScanModule(signature, mask, ModuleName, stopCondition);
		if (vAddrResults.empty())
			return -1;

		for (SIZE_T i = 0; i < vAddrResults.size(); i++)
		{
			ScannedAddresses.push_back(vAddrResults[i]);
			ScannedAddresses[i] = IncrementByByte(ScannedAddresses[i], Offset); 
		}
	}

	std::vector<BYTE> vPatchBytes = StringToBytes(PatchBytes);
	for (auto& addr : ScannedAddresses)
	{
		std::vector<BYTE> vBytesBeforeWrite;
		NTSTATUS status = Patch(addr, reinterpret_cast<uintptr_t*>(vPatchBytes.data()), vPatchBytes.size(), &vBytesBeforeWrite);
		if (status != STATUS_SUCCESS)
			return status;

		OriginalBytes.push_back(vBytesBeforeWrite);
	}

	IsAttached = TRUE;
	return STATUS_SUCCESS;
}

VOID ms::SPatch::Detach()
{
	if (!IsAttached || ScannedAddresses.empty())
		return;

	for (SIZE_T i = 0; i < ScannedAddresses.size(); i++)
	{
		Patch(ScannedAddresses[i], reinterpret_cast<uintptr_t*>(OriginalBytes[i].data()), OriginalBytes[i].size(), nullptr);
		OriginalBytes[i].clear();
	}

	OriginalBytes.clear();
	IsAttached = FALSE;
}
