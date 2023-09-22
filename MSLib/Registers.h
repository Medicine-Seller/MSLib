#pragma once

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
};

extern "C" Registersx64 registersx64;
