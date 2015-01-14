.CODE

Lorigin::

thisValue dq ?;
origEntry dq ?;

Lcode::

; 88 bytes
pushall macro
    push rsp
    push r15
    push r14
    push r13
    push r12
    push r11
    push r10
    push r9
    push r8
    push rdx
    push rcx
    push rbx
    push rax
    push rdi
    push rsi
    push rbp
    pushfq
endm

popall macro
    popfq
    pop rbp
    pop rsi
    pop rdi
    pop rax
    pop rbx
    pop rcx
    pop rdx
    pop r8
    pop r9
    pop r10
    pop r11
    pop r12
    pop r13
    pop r14
    pop r15
    pop rsp
endm

Ljumpblock::
    pushall
Lthis1::
    mov rcx, [thisValue]
    mov rax, [rcx]
    mov rdx, rsp
    lea r8, [rsp + 88h]
    mov rbp, rsp
    and rsp, byte -10h
    sub rsp, 20h
    call qword ptr [rax + 08h]
    mov rsp, rbp
    popall
Ljmpback::
    jmp qword ptr [origEntry]

Lretblock::
    push rcx
    pushall
Lthis2::
    mov rcx, [thisValue]
    mov rax, [rcx]
    mov rdx, rsp
    lea r8, [rsp + 88h]
    mov rbp, rsp
    and rsp, byte -10h
    sub rsp, 20h
    call qword ptr [rax + 10h]
    mov rsp, rbp
    popall
    ret
Lend::

.DATA

public headerPtr
public allSize
public entryOffs
public leaveOffs

headerPtr dq Lorigin
allSize   dd Lend - Lorigin
entryOffs dd Ljumpblock - Lorigin
leaveOffs dd Lretblock - Lorigin

end