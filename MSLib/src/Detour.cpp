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
	0x41, 0x57,										// push r15
	0x49, 0xBF,										// mov r15,...
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // 
	0x41, 0x57,                                     // push r15
	0x4C, 0x8B, 0x7C, 0x24, 0x08,                   // mov r15,[rsp+8]
	0xFF, 0x25, 0x00, 0x00, 0x00, 0x00,				// jmp ...
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // 
	0x41, 0x5F,										// pop r15
};

NTSTATUS ms::Detour::CreateDetour( const PVOID source, const PVOID destination, PVOID* allocatedMemory, const SIZE_T writeSize, std::vector<BYTE>* originalBytes, const Type jumpType)
{
	if (jumpType == Type::JMP_ABSOLUTE && writeSize < 14)
		return STATUS_UNSUCCESSFUL;
	else if (writeSize < 5)
		return STATUS_UNSUCCESSFUL;

	SIZE_T detourSize = ShellCode_Detour.size() + ShellCode_JumpAbsolute.size() + writeSize;
	*allocatedMemory = AllocateMemoryNearAddress(source, detourSize);
	
	PVOID allocation = *allocatedMemory;
	if (!allocation)
		return STATUS_UNSUCCESSFUL;

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

	*reinterpret_cast<uintptr_t*>(detour.data() + 4) = reinterpret_cast<uintptr_t>(allocation) + ShellCode_Detour.size()-2;
	*reinterpret_cast<uintptr_t*>(detour.data() + 25) = reinterpret_cast<uintptr_t>(destination);
	*reinterpret_cast<uintptr_t*>(returnToOriginal.data() + 6) = reinterpret_cast<uintptr_t>(source) + writeSize;

	memcpy(allocation, detour.data(), detour.size());

	PVOID detourOriginalOffset = IncrementByByte(allocation, detour.size());
	memcpy(detourOriginalOffset, source, writeSize);

	PVOID detourReturnOffset = IncrementByByte(allocation, detour.size() + writeSize);
	memcpy(detourReturnOffset, returnToOriginal.data(), returnToOriginal.size());

	return Patch::PatchBytes(source, &jmpToDetour, originalBytes ? originalBytes : NULL);
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
		return STATUS_UNSUCCESSFUL;

	return STATUS_SUCCESS;
}