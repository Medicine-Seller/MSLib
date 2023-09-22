EXTERN registersx64:QWORD

.data
	Temp QWORD 0

.code

SaveRegisters PROC
    mov Temp,rax
    pushfq                      ;Push flags on stack
    pop rax                     ;Pop flags off stack
    mov registersx64[0*8],rax   ;Store flags
    mov rax,Temp

    mov registersx64[1*8],rsp    
    add registersx64[1*8],8     ;Remove Caller Return

    mov registersx64[2*8],rax
    mov registersx64[3*8],rbx
    mov registersx64[4*8],rcx
    mov registersx64[5*8],rdx
    mov registersx64[6*8],rsi
    mov registersx64[7*8],rdi
    mov registersx64[8*8],rbp
    mov registersx64[9*8],r8
    mov registersx64[10*8],r9
    mov registersx64[11*8],r10
    mov registersx64[12*8],r11
    mov registersx64[13*8],r12
    mov registersx64[14*8],r13
    mov registersx64[15*8],r14
    mov registersx64[16*8],r15

    ret
SaveRegisters ENDP

RestoreRegisters PROC
    mov rax,registersx64[2*8]
    mov rbx,registersx64[3*8]
    mov rcx,registersx64[4*8]
    mov rdx,registersx64[5*8]
    mov rsi,registersx64[6*8]
    mov rdi,registersx64[7*8]
    mov rbp,registersx64[8*8]
    mov r8,registersx64[9*8]
    mov r9,registersx64[10*8]
    mov r10,registersx64[11*8]
    mov r11,registersx64[12*8]
    mov r12,registersx64[13*8]
    mov r13,registersx64[14*8]
    mov r14,registersx64[15*8]
    mov r15,registersx64[16*8]

    mov rsp,registersx64[1*8] 
    sub rsp,8                   ;Add Caller Return

    mov Temp,rax
    mov rax,registersx64[0*8]   ;Move flags
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

    ;mov rbp,0
    mov rsi,0
    ;mov rsp,0

    push rax                    ;Push 0 to stack
    popfq                       ;Clear flags

    ret
ClearRegisters ENDP

end