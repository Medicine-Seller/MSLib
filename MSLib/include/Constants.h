#include <vector>

#pragma warning(disable: 4005)
#include <ntstatus.h>
#ifndef CONSTANTS_H
#define CONSTANTS_H

namespace ms
{
	struct ConstantString
	{
		long Value;
		const char* StringValue;
	};

#define MS_P(x) {(long)x, #x}
	const std::vector<ConstantString> ConstantsMemoryProtection
	{
		MS_P(PAGE_EXECUTE),
		MS_P(PAGE_EXECUTE_READ),
		MS_P(PAGE_EXECUTE_READWRITE),
		MS_P(PAGE_EXECUTE_WRITECOPY),
		MS_P(PAGE_NOACCESS),
		MS_P(PAGE_READONLY),
		MS_P(PAGE_READWRITE),
		MS_P(PAGE_WRITECOPY),
		MS_P(PAGE_TARGETS_INVALID),
		MS_P(PAGE_TARGETS_NO_UPDATE),
		MS_P(PAGE_GUARD),
		MS_P(PAGE_NOCACHE),
		MS_P(PAGE_WRITECOMBINE)
	};

	const std::vector<ConstantString> ConstantsPage
	{
		MS_P(MEM_COMMIT),
		MS_P(MEM_RESERVE),
		MS_P(MEM_FREE),
		MS_P(MEM_PRIVATE),
		MS_P(MEM_MAPPED),
		MS_P(MEM_IMAGE)
	};

	const std::vector<ConstantString> ConstantsNtStatus
	{
		MS_P(STATUS_SUCCESS),
		MS_P(STATUS_INFO_LENGTH_MISMATCH),
		MS_P(STATUS_ACCESS_VIOLATION),
	};

	const std::vector<ConstantString> ConstantsAccessRights
	{
		MS_P(PROCESS_ALL_ACCESS),
		MS_P(PROCESS_CREATE_PROCESS),
		MS_P(PROCESS_CREATE_THREAD),
		MS_P(PROCESS_DUP_HANDLE),
		MS_P(PROCESS_QUERY_INFORMATION),
		MS_P(PROCESS_QUERY_LIMITED_INFORMATION),
		MS_P(PROCESS_SET_INFORMATION),
		MS_P(PROCESS_SET_QUOTA),
		MS_P(PROCESS_SUSPEND_RESUME),
		MS_P(PROCESS_TERMINATE),
		MS_P(PROCESS_VM_OPERATION),
		MS_P(PROCESS_VM_READ),
		MS_P(PROCESS_VM_WRITE),
		MS_P(SYNCHRONIZE)
	};

#undef MS_P

#define CONSTANT_PAGE ms::ConstantsMemoryProtection
#define CONSTANT_MEM ms::ConstantsPage
#define CONSTANT_STATUS ms::ConstantsNtStatus
#define CONSTANT_ACCESS ms::ConstantsAccessRights
}

#endif