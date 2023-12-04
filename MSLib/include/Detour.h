#include <Windows.h>
#include <vector>
#include <string>

#ifndef DETOUR_H
#define DETOUR_H

namespace ms
{
	class Detour
	{
	private:

		const static std::vector<BYTE>ShellCode_JumpAbsolute;
		const static std::vector<BYTE>ShellCode_JumpRelative;
		const static std::vector<BYTE>ShellCode_Detour;

	public:

		enum Type
		{
			JMP_RELATIVE,
			JMP_ABSOLUTE
		};

		struct DetourInfo
		{
			std::string Name;
			PVOID Source;
			PVOID Destination;
			PVOID AllocatedMemory;
			SIZE_T WriteSize;
			BOOL IsAttached = FALSE;
			std::vector<BYTE> OriginalBytes;
		};

		struct DetourInfoAOB : DetourInfo
		{
			std::vector<BYTE> Signature;
			std::vector<CHAR> Mask;
			PVOID Module;
			LONG Offset = 0;
		};

		struct DetourInfoAOBString : DetourInfoAOB
		{
			std::string SignatureString;
			std::string ModuleString;
		};

		static NTSTATUS CreateDetour(
			const PVOID source,
			const PVOID destination,
			PVOID* allocatedMemory,
			const SIZE_T writeSize,
			std::vector<BYTE>* originalBytes,
			const Type jumpType = Type::JMP_RELATIVE);

		static NTSTATUS Attach(DetourInfo& info, Type jumpType = Type::JMP_RELATIVE);
		static NTSTATUS Attach(DetourInfoAOB& info, Type jumpType = Type::JMP_RELATIVE);
		static NTSTATUS Attach(DetourInfoAOBString& info, Type jumpType = Type::JMP_RELATIVE);
		static NTSTATUS Detach(DetourInfo& info);
	};
}

#endif