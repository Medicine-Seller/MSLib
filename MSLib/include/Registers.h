#include <Windows.h>

#ifndef REGISTERS_H
#define REGISTERS_H

namespace ms
{
	struct Registersx64
	{
		uintptr_t flags;
		uintptr_t rsp;
		uintptr_t rax;
		uintptr_t rbx;
		uintptr_t rcx;
		uintptr_t rdx;
		uintptr_t rsi;
		uintptr_t rdi;
		uintptr_t rbp;
		uintptr_t r8;
		uintptr_t r9;
		uintptr_t r10;
		uintptr_t r11;
		uintptr_t r12;
		uintptr_t r13;
		uintptr_t r14;
		uintptr_t r15;
		uintptr_t xmm0_lower;
		uintptr_t xmm0_upper;
		uintptr_t xmm1_lower;
		uintptr_t xmm1_upper;
		uintptr_t xmm2_lower;
		uintptr_t xmm2_upper;
		uintptr_t xmm3_lower;
		uintptr_t xmm3_upper;
		uintptr_t xmm4_lower;
		uintptr_t xmm4_upper;
		uintptr_t xmm5_lower;
		uintptr_t xmm5_upper;
		uintptr_t xmm6_lower;
		uintptr_t xmm6_upper;
		uintptr_t xmm7_lower;
		uintptr_t xmm7_upper;
		uintptr_t xmm8_lower;
		uintptr_t xmm8_upper;
		uintptr_t xmm9_lower;
		uintptr_t xmm9_upper;
		uintptr_t xmm10_lower;
		uintptr_t xmm10_upper;
		uintptr_t xmm11_lower;
		uintptr_t xmm11_upper;
		uintptr_t xmm12_lower;
		uintptr_t xmm12_upper;
		uintptr_t xmm13_lower;
		uintptr_t xmm13_upper;
		uintptr_t xmm14_lower;
		uintptr_t xmm14_upper;
		uintptr_t xmm15_lower;
		uintptr_t xmm15_upper;
		uintptr_t xmm16_lower;
		uintptr_t xmm16_upper;
	};
}

#endif