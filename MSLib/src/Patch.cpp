#include "Patch.h"
#include "VirtualProtect.h"
#include "Util.h"
#include "Scan.h"
#include "Logger.h"

NTSTATUS ms::Patch::PatchBytes(const PVOID destination, const std::vector<BYTE>* patchBytes, std::vector<BYTE>* originalBytes)
{
	NTSTATUS status = VirtualProtect::PushProtectWrite(destination, patchBytes->size(), PAGE_EXECUTE_READWRITE);
	if (!SUCCEEDED(status))
		return status;

	if (originalBytes)
	{
		originalBytes->resize(patchBytes->size());
		memcpy(originalBytes->data(), destination, patchBytes->size());
	}

	memcpy(destination, patchBytes->data(), patchBytes->size());

	VirtualProtect::PopProtectWrite();
	return STATUS_SUCCESS;
}

NTSTATUS ms::Patch::PatchBytes(const PVOID destination, const PVOID source, const SIZE_T size, std::vector<BYTE>* originalBytes)
{
	std::vector<BYTE> patchBytes(size, 0x0);
	memcpy(patchBytes.data(), source, size);

	return PatchBytes(destination, &patchBytes, originalBytes);
}

NTSTATUS ms::Patch::Attach(PatchInfo& info)
{
	if (info.IsAttached)
		return STATUS_SUCCESS;

	NTSTATUS status = PatchBytes(info.Destination, &info.WriteBytes, &info.OriginalBytes);
	if (!SUCCEEDED(status))
		return status;

	info.IsAttached = TRUE;
	return STATUS_SUCCESS;
}

NTSTATUS ms::Patch::Attach(PatchInfoAOB& info, const Scan::STOP_CONDITION stopCondition)
{
	if (info.IsAttached)
		return STATUS_SUCCESS;

	if (info.DestinationList.empty())
	{
		MODULEINFO modInfo = GetModuleInfo(reinterpret_cast<HMODULE>(info.Module));

		info.DestinationList = Scan::AOBScan(info.Signature, info.Mask, info.Module, modInfo.SizeOfImage, stopCondition);

		if (info.DestinationList.empty())
			return STATUS_UNSUCCESSFUL;

		for (auto& e : info.DestinationList)
		{
			uintptr_t* address = reinterpret_cast<uintptr_t*>(e);
			e = IncrementByByte(address, info.Offset);
		}
	}

	for (auto& address : info.DestinationList)
	{
		std::vector<BYTE> originalBytes;
		NTSTATUS status = Patch::PatchBytes(address, &info.PatchBytes, &originalBytes);
		if (status != STATUS_SUCCESS)
			return status;

		info.OriginalBytesList.push_back(originalBytes);
	}

	info.IsAttached = TRUE;
	return STATUS_SUCCESS;
}

NTSTATUS ms::Patch::Attach(PatchInfoAOBString& info, const Scan::STOP_CONDITION stopCondition)
{
	if (info.DestinationList.empty())
	{
		info.Mask = Scan::CreateMask(info.SignatureString);
		info.Signature = Scan::CreateSignature(info.SignatureString);
		info.Module = GetModuleHandleA(info.ModuleString.c_str());
		info.PatchBytes = StringToBytes(info.PatchBytesString);
	}

	PatchInfoAOB& patchInfoAOB = info;
	return Attach(patchInfoAOB, stopCondition);
}

NTSTATUS ms::Patch::Detach(PatchInfo& info)
{
	if (!info.IsAttached)
		return STATUS_SUCCESS;

	NTSTATUS status = PatchBytes(info.Destination, &info.OriginalBytes, NULL);
	if (!SUCCEEDED(status))
		return status;

	info.OriginalBytes.clear();
	info.IsAttached = FALSE;
	return STATUS_SUCCESS;
}

NTSTATUS ms::Patch::Detach(PatchAOBBase& info)
{
	if (!info.IsAttached || info.DestinationList.empty())
		return STATUS_SUCCESS;

	for (SIZE_T i = 0; i < info.DestinationList.size(); i++)
	{
		NTSTATUS status = Patch::PatchBytes(info.DestinationList[i], &info.OriginalBytesList[i], NULL);
		if (!SUCCEEDED(status))
			return status;

		info.OriginalBytesList[i].clear();
	}

	info.OriginalBytesList.clear();
	info.IsAttached = FALSE;
	return STATUS_SUCCESS;
}