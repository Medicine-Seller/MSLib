#include "MainHeader.h"

std::vector<BYTE> ms::Detour(uintptr_t* pulDestination, uintptr_t* pulSource, size_t ulSize, DETOUR_TYPE jmpType)
{
#ifdef _DEBUG
	std::cout << COL2("Detour::pulDestination", pulDestination) << std::endl;
	std::cout << COL2("Detour::pulSource", pulSource) << std::endl;
	std::cout << COL2("Detour::ulSize", ulSize) << std::endl;
#endif
	static const BYTE sbRelativeJump[] = "\xE9";
	static const BYTE sbAbsoluteJump[] = "\xFF\x25\x00\x00\x00\x00";

	if (jmpType == JMP_RELATIVE && ulSize < 5)
		return {};
	else if (jmpType == JMP_ABSOLUTE && ulSize < 14)
		return {};

	PushProtectWrite(pulDestination, ulSize);

	std::vector<BYTE> vOriginalBytes(ulSize);
	memcpy(vOriginalBytes.data(), pulDestination, ulSize);
	memset(pulDestination, 0x90, ulSize);

	if (jmpType == JMP_RELATIVE)
	{
		memcpy(pulDestination, sbRelativeJump, 1);
		uintptr_t ulJmpLocation = reinterpret_cast<uintptr_t>(pulSource) - reinterpret_cast<uintptr_t>(pulDestination) - 5;
		*reinterpret_cast<uintptr_t*>(reinterpret_cast<BYTE*>(pulDestination) + 1) = ulJmpLocation;
	}
	else if (jmpType == JMP_ABSOLUTE)
	{
		memcpy(pulDestination, sbAbsoluteJump, 6);
		*reinterpret_cast<uintptr_t*>(reinterpret_cast<BYTE*>(pulDestination) + 6) = reinterpret_cast<uintptr_t>(pulSource);
	}

	PopProtectWrite();

	return vOriginalBytes;
}

BOOL ms::SDetour::Attach(DETOUR_TYPE jmpType)
{
	if (bAttached)
		return TRUE;

	if (!bSigAddrFound)
	{
		std::vector<uintptr_t*> vAddrResults = StringAobScan(szSignature, szModuleName);
		if (vAddrResults.empty())
			return FALSE;

		pulScannedAddr = vAddrResults[0] + dwOffset;
		pulReturn = reinterpret_cast<uintptr_t>(pulScannedAddr) + dwWriteSize;
		bSigAddrFound = TRUE;
	}

	vOriginalBytes = Detour(pulScannedAddr, pulDestination, dwWriteSize, jmpType);
	bAttached = TRUE;

#ifdef ENABLE_LOGGING
	std::cout << COL2("SDetour::Attach::" + szName, pulScannedAddr, szName + " = " + pulDestination) << std::endl;
#endif
	return TRUE;
}

VOID ms::SDetour::Detach()
{
	if (!bAttached || pulScannedAddr == nullptr)
		return;

	Patch(pulScannedAddr, reinterpret_cast<uintptr_t*>(vOriginalBytes.data()), dwWriteSize, FALSE);

	vOriginalBytes.clear();
	bAttached = FALSE;

#ifdef ENABLE_LOGGING
	std::cout << COL2("SDetour::Attach::" + szName, "Detached") << std::endl;
#endif
}