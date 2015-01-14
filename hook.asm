BITS 64

; 88 bytes
%macro pushall 0
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
%endmacro

%macro popall 0
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
%endmacro

jumpblock:
	pushall
	mov rcx, 0x123456789abcdef
	mov rax, [rcx]
	mov rdx, rsp
	lea r8, [rsp + 0x88]
	mov rbp, rsp
	and rsp, byte -0x10
	sub rsp, 0x20
	call [rax + 0x08]
	mov rsp, rbp
	popall
	jmp 0x131345

retblock:
	push rcx
	pushall
	mov rcx, 0x123456789abcdef
	mov rax, [rcx]
	mov rdx, rsp
	lea r8, [rsp + 0x88]
	mov rbp, rsp
	and rsp, byte -0x10
	sub rsp, 0x20
	call [rax + 0x10]
	mov rsp, rbp
	popall
	ret