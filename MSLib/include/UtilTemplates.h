#include <string>
#include <vector>
#include <Windows.h>
#include <Psapi.h>
#include "Constants.h"
#include <sstream>
#include "Macros.h"

#ifndef UTILTEMPLATES_H
#define UTILTEMPLATES_H
namespace ms
{
	template<typename DataType>
	DataType IncrementByByte(const DataType address, const LONGLONG offset)
	{
		return reinterpret_cast<DataType>(reinterpret_cast<BYTE*>(address) + offset);
	}

	template<typename DataType>
	DataType GetAddress(const uintptr_t base, const std::vector<uintptr_t> offsets)
	{
		for (unsigned int i = 0; i < offsets.size(); i++)
		{
			if (base < 0xFFFFFF || IsBadReadPtr((uintptr_t*)base, sizeof(uintptr_t)))
			{
				base = 0;
				break;
			}

			if (i == offsets.size() - 1)
				base = (uintptr_t)((BYTE*)base + offsets[i]);
			else
				base = *(uintptr_t*)((BYTE*)base + offsets[i]);
		}

		return (DataType)base;
	}

	template <typename DataType>
	std::string ToString(const DataType value)
	{
		std::stringstream ss;
		ss << value;
		return ss.str();
	}

}

#endif