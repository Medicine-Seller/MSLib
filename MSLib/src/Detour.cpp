#include "Detour.h"
#include "Patch.h"
#include "Scan.h"
#include "Util.h"
#include "Logger.h"

const std::vector<BYTE>ms::Detour::ShellCode_JumpAbsolute
{
	0xFF,0x25,0x00,0x00,0x00,0x00,                  // jmp ...
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, //
};

const std::vector<BYTE>ms::Detour::ShellCode_JumpRelative
{
	0xE9, 0x00, 0x00, 0x00, 0x00					// jmp ...
};

const std::vector<BYTE>ms::Detour::ShellCode_Detour
{
	0x50,											// push rax
	0x48, 0xB8,										// mov rax,...
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // 
	0x50,                                           // push rax
	0x48, 0x8B, 0x44, 0x24, 0x08,                   // mov rax,[rsp+8]
	0xFF, 0x25, 0x00, 0x00, 0x00, 0x00,				// jmp ...
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // 
	0x58,											// pop rax
};

NTSTATUS ms::Detour::CreateDetour(
	PVOID source,
	PVOID destination,
	PVOID* allocatedMemory,
	SIZE_T writeSize,
	std::vector<BYTE>* originalBytes,
	Type jumpType)
{
	if (jumpType == Type::JMP_ABSOLUTE && writeSize < 14)
		return STATUS_INTERNAL_ERROR;
	else if (writeSize < 5)
		return STATUS_INTERNAL_ERROR;

	SIZE_T detourSize = ShellCode_Detour.size() + ShellCode_JumpAbsolute.size() + writeSize;
	*allocatedMemory = AllocateMemoryNearAddress(source, detourSize);
	PVOID allocation = *allocatedMemory;

	if (!allocation)
		return STATUS_INTERNAL_ERROR;

	std::vector<BYTE>jmpToDetour(writeSize, 0x90);
	if (jumpType == Type::JMP_ABSOLUTE)
	{
		memcpy(jmpToDetour.data(), ShellCode_JumpAbsolute.data(), ShellCode_JumpAbsolute.size());
		uintptr_t address = reinterpret_cast<uintptr_t>(allocation);
		*reinterpret_cast<uintptr_t*>(jmpToDetour.data() + 6) = address;
	}
	else
	{
		memcpy(jmpToDetour.data(), ShellCode_JumpRelative.data(), ShellCode_JumpRelative.size());
		uintptr_t address = reinterpret_cast<uintptr_t>(allocation) - reinterpret_cast<uintptr_t>(source) - 5;
		*reinterpret_cast<int32_t*>(jmpToDetour.data() + 1) = static_cast<int32_t>(address);
	}

	std::vector<BYTE>detour = ShellCode_Detour;
	std::vector<BYTE>returnToOriginal = ShellCode_JumpAbsolute;

	*reinterpret_cast<uintptr_t*>(detour.data() + 3) = reinterpret_cast<uintptr_t>(allocation) + ShellCode_Detour.size() - 1;
	*reinterpret_cast<uintptr_t*>(detour.data() + 23) = reinterpret_cast<uintptr_t>(destination);
	*reinterpret_cast<uintptr_t*>(returnToOriginal.data() + 6) = reinterpret_cast<uintptr_t>(source) + writeSize;

	memcpy(allocation, detour.data(), detour.size());

	void* detourReturnAddress = reinterpret_cast<void*>(reinterpret_cast<uintptr_t>(allocation) + detour.size());
	memcpy(detourReturnAddress, returnToOriginal.data(), returnToOriginal.size());

	NTSTATUS status = Patch::PatchBytes(source, &jmpToDetour, originalBytes ? originalBytes : NULL);
	return status;
}

NTSTATUS ms::Detour::Attach(DetourInfo& info, Type jumpType)
{
	if (info.IsAttached)
		return STATUS_SUCCESS;

	NTSTATUS status = CreateDetour(info.Source, info.Destination, &info.AllocatedMemory, info.WriteSize, &info.OriginalBytes, jumpType);
	if (SUCCEEDED(status))
		info.IsAttached = TRUE;

	return status;
}

NTSTATUS ms::Detour::Attach(DetourInfoAOB& info, Type jumpType)
{
	if (!info.Source)
	{
		MODULEINFO modInfo = GetModuleInfo(reinterpret_cast<HMODULE>(info.Module));

		std::vector<PVOID> addressResults = Scan::AOBScan(info.Signature, info.Mask, info.Module, modInfo.SizeOfImage, Scan::STOP_CONDITION::FIRST_RESULT);
		if (addressResults.empty())
			return STATUS_SUCCESS;

		uintptr_t* adjustedAddress = reinterpret_cast<uintptr_t*>(IncrementByByte(addressResults[0], info.Offset));
		info.Source = reinterpret_cast<PVOID>(adjustedAddress);
	}

	DetourInfo& detourInfo = info;
	return Attach(detourInfo, jumpType);
}

NTSTATUS ms::Detour::Attach(DetourInfoAOBString& info, Type jumpType)
{
	if (!info.Source)
	{
		info.Mask = Scan::CreateMask(info.SignatureString);
		info.Signature = Scan::CreateSignature(info.SignatureString);
		info.Module = GetModuleHandleA(info.ModuleString.c_str());
	}

	DetourInfoAOB& detourInfoAob = info;
	return Attach(detourInfoAob, jumpType);
}

NTSTATUS ms::Detour::Detach(DetourInfo& info)
{
	if (!info.IsAttached)
		return STATUS_SUCCESS;

	NTSTATUS status = Patch::PatchBytes(info.Source, &info.OriginalBytes, nullptr);
	if (SUCCEEDED(status))
		info.IsAttached = FALSE;

	if (!VirtualFree(info.AllocatedMemory, 0, MEM_RELEASE))
		return STATUS_INTERNAL_ERROR;

	return STATUS_SUCCESS;
}