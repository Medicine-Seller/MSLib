EXTERNDEF SaveRegisters:PROC
EXTERNDEF ClearRegisters:PROC
EXTERNDEF RestoreRegisters:PROC
EXTERNDEF x64Registers:QWORD

.data
	Temp QWORD 0
    x64Registers QWORD 48*8 dup (0)
.code

SaveRegisters PROC
    mov Temp,rax
    pushfq                      ;Push flags on stack
    pop rax                     ;Pop flags off stack into rax
    mov x64Registers[0*8],rax         ;Store flags
    mov rax,Temp

    mov x64Registers[1*8],rsp    
    add x64Registers[1*8],8           ;Remove Caller Return

    mov x64Registers[2*8],rax         ;Save registers
    mov x64Registers[3*8],rbx
    mov x64Registers[4*8],rcx
    mov x64Registers[5*8],rdx
    mov x64Registers[6*8],rsi
    mov x64Registers[7*8],rdi
    mov x64Registers[8*8],rbp
    mov x64Registers[9*8],r8
    mov x64Registers[10*8],r9
    mov x64Registers[11*8],r10
    mov x64Registers[12*8],r11
    mov x64Registers[13*8],r12
    mov x64Registers[14*8],r13
    mov x64Registers[15*8],r14
    mov x64Registers[16*8],r15
    
    lea rax,[x64Registers+17*8]
    movaps [rax+0*16],xmm0      ;Save SSE registers
    movaps [rax+1*16],xmm1
    movaps [rax+2*16],xmm2
    movaps [rax+3*16],xmm3
    movaps [rax+4*16],xmm4
    movaps [rax+5*16],xmm5
    movaps [rax+6*16],xmm6
    movaps [rax+7*16],xmm7
    movaps [rax+8*16],xmm8
    movaps [rax+9*16],xmm9
    movaps [rax+10*16],xmm10
    movaps [rax+11*16],xmm11
    movaps [rax+12*16],xmm12
    movaps [rax+13*16],xmm13
    movaps [rax+14*16],xmm14
    movaps [rax+15*16],xmm15

    mov rax,Temp

    ret
SaveRegisters ENDP

RestoreRegisters PROC
    lea rax,[x64Registers+17*8]
    movaps xmm15,[rax+15*16]
    movaps xmm14,[rax+14*16]
    movaps xmm13,[rax+13*16]
    movaps xmm12,[rax+12*16]
    movaps xmm11,[rax+11*16]
    movaps xmm10,[rax+10*16]
    movaps xmm9,[rax+9*16]
    movaps xmm8,[rax+8*16]
    movaps xmm7,[rax+7*16]
    movaps xmm6,[rax+6*16]
    movaps xmm5,[rax+5*16]
    movaps xmm4,[rax+4*16]
    movaps xmm3,[rax+3*16]
    movaps xmm2,[rax+2*16]
    movaps xmm1,[rax+1*16]
    movaps xmm0,[rax+0*16]

    mov r15,x64Registers[16*8]        ;Restore registers
    mov r14,x64Registers[15*8]
    mov r13,x64Registers[14*8]
    mov r12,x64Registers[13*8]
    mov r11,x64Registers[12*8]
    mov r10,x64Registers[11*8]
    mov r9,x64Registers[10*8]
    mov r8,x64Registers[9*8]
    mov rbp,x64Registers[8*8]
    mov rdi,x64Registers[7*8]
    mov rsi,x64Registers[6*8]
    mov rdx,x64Registers[5*8]
    mov rcx,x64Registers[4*8]
    mov rbx,x64Registers[3*8]
    mov rax,x64Registers[2*8]

    mov rsp,x64Registers[1*8] 
    sub rsp,8                   ;Add Caller Return

    mov Temp,rax
    mov rax,x64Registers[0*8]         ;Move flags
    push rax                    ;Push flags to stack
    popfq                       ;Restore flags
    mov rax,Temp

    ret
RestoreRegisters ENDP

ClearRegisters PROC
    mov rax,0
    mov rbx,0
    mov rcx,0
    mov rdx,0
    mov rdi,0
    mov r8,0
    mov r9,0
    mov r10,0
    mov r11,0
    mov r12,0
    mov r13,0
    mov r14,0
    mov r15,0
    mov rdi,0

    xorps xmm0,xmm0
    xorps xmm1,xmm1
    xorps xmm2,xmm2
    xorps xmm3,xmm3
    xorps xmm4,xmm4
    xorps xmm5,xmm5
    xorps xmm6,xmm6
    xorps xmm7,xmm7
    xorps xmm8,xmm8
    xorps xmm9,xmm9
    xorps xmm10,xmm10
    xorps xmm11,xmm11
    xorps xmm12,xmm12
    xorps xmm13,xmm13
    xorps xmm14,xmm14
    xorps xmm15,xmm15

    ;mov rbp,0
    mov rsi,0
    ;mov rsp,0

    push rax                    ;Push 0 to stack
    popfq                       ;Clear flags

    ret
ClearRegisters ENDP

end