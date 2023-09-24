#pragma once

std::string ReplaceString(std::string szStr, std::string szFind, std::string szReplace);
std::vector<BYTE> StringToBytes(std::string szBytes);
std::vector<CHAR> GenerateMask(std::string szSig);

template<typename DataType>
DataType* IncrementByByte(DataType* pulAddress, intptr_t ulOffset)
{
	return reinterpret_cast<DataType*>(reinterpret_cast<BYTE*>(pulAddress) + ulOffset);
}

template<typename DataType>
DataType GetAddress(uintptr_t base, std::vector<uintptr_t> offsets)
{
	for (unsigned int i = 0; i < offsets.size(); i++)
	{
		if (base < 0xFFFFFF || IsBadReadPtr((uintptr_t*)base, sizeof(uintptr_t)))
		{
			base = 0;
			break;
		}

		if (i == offsets.size() - 1) //If last offset, don't dereference
			base = (uintptr_t)((BYTE*)base + offsets[i]);
		else
			base = *(uintptr_t*)((BYTE*)base + offsets[i]);
	}

	return (DataType)base;
}
