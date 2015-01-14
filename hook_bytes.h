#define THIS1 (0x19 + 2)
#define JMPADDR1 0x5b
#define RETBLOCK 0x60
#define THIS2 (0x7a + 2)

unsigned char data[0xbd] = {
    /* 00000000 push rsp */
    0x54,
    /* 00000001 push r15 */
    0x41, 0x57,
    /* 00000003 push r14 */
    0x41, 0x56,
    /* 00000005 push r13 */
    0x41, 0x55,
    /* 00000007 push r12 */
    0x41, 0x54,
    /* 00000009 push r11 */
    0x41, 0x53,
    /* 0000000b push r10 */
    0x41, 0x52,
    /* 0000000d push r9 */
    0x41, 0x51,
    /* 0000000f push r8 */
    0x41, 0x50,
    /* 00000011 push rdx */
    0x52,
    /* 00000012 push rcx */
    0x51,
    /* 00000013 push rbx */
    0x53,
    /* 00000014 push rax */
    0x50,
    /* 00000015 push rdi */
    0x57,
    /* 00000016 push rsi */
    0x56,
    /* 00000017 push rbp */
    0x55,
    /* 00000018 pushfq */
    0x9c,
    /* 00000019 mov rcx,0x123456789abcdef */
    0x48, 0xb9, 0xef, 0xcd, 0xab, 0x89, 0x67, 0x45, 0x23, 0x01,
    /* 00000023 mov rax,[rcx] */
    0x48, 0x8b, 0x01,
    /* 00000026 mov rdx,rsp */
    0x48, 0x89, 0xe2,
    /* 00000029 lea r8,[rsp+0x88] */
    0x4c, 0x8d, 0x84, 0x24, 0x88, 0x00, 0x00, 0x00,
    /* 00000031 mov rbp,rsp */
    0x48, 0x89, 0xe5,
    /* 00000034 and rsp,byte -0x10 */
    0x48, 0x83, 0xe4, 0xf0,
    /* 00000038 sub rsp,byte +0x20 */
    0x48, 0x83, 0xec, 0x20,
    /* 0000003c call qword [rax+0x8] */
    0xff, 0x50, 0x08,
    /* 0000003f mov rsp,rbp */
    0x48, 0x89, 0xec,
    /* 00000042 popfq */
    0x9d,
    /* 00000043 pop rbp */
    0x5d,
    /* 00000044 pop rsi */
    0x5e,
    /* 00000045 pop rdi */
    0x5f,
    /* 00000046 pop rax */
    0x58,
    /* 00000047 pop rbx */
    0x5b,
    /* 00000048 pop rcx */
    0x59,
    /* 00000049 pop rdx */
    0x5a,
    /* 0000004a pop r8 */
    0x41, 0x58,
    /* 0000004c pop r9 */
    0x41, 0x59,
    /* 0000004e pop r10 */
    0x41, 0x5a,
    /* 00000050 pop r11 */
    0x41, 0x5b,
    /* 00000052 pop r12 */
    0x41, 0x5c,
    /* 00000054 pop r13 */
    0x41, 0x5d,
    /* 00000056 pop r14 */
    0x41, 0x5e,
    /* 00000058 pop r15 */
    0x41, 0x5f,
    /* 0000005a pop rsp */
    0x5c,
    /* 0000005b jmp qword 0x131345 */
    0xe9, 0xe5, 0x12, 0x13, 0x00,
    /* 00000060 push rcx */
    0x51,
    /* 00000061 push rsp */
    0x54,
    /* 00000062 push r15 */
    0x41, 0x57,
    /* 00000064 push r14 */
    0x41, 0x56,
    /* 00000066 push r13 */
    0x41, 0x55,
    /* 00000068 push r12 */
    0x41, 0x54,
    /* 0000006a push r11 */
    0x41, 0x53,
    /* 0000006c push r10 */
    0x41, 0x52,
    /* 0000006e push r9 */
    0x41, 0x51,
    /* 00000070 push r8 */
    0x41, 0x50,
    /* 00000072 push rdx */
    0x52,
    /* 00000073 push rcx */
    0x51,
    /* 00000074 push rbx */
    0x53,
    /* 00000075 push rax */
    0x50,
    /* 00000076 push rdi */
    0x57,
    /* 00000077 push rsi */
    0x56,
    /* 00000078 push rbp */
    0x55,
    /* 00000079 pushfq */
    0x9c,
    /* 0000007a mov rcx,0x123456789abcdef */
    0x48, 0xb9, 0xef, 0xcd, 0xab, 0x89, 0x67, 0x45, 0x23, 0x01,
    /* 00000084 mov rax,[rcx] */
    0x48, 0x8b, 0x01,
    /* 00000087 mov rdx,rsp */
    0x48, 0x89, 0xe2,
    /* 0000008a lea r8,[rsp+0x88] */
    0x4c, 0x8d, 0x84, 0x24, 0x88, 0x00, 0x00, 0x00,
    /* 00000092 mov rbp,rsp */
    0x48, 0x89, 0xe5,
    /* 00000095 and rsp,byte -0x10 */
    0x48, 0x83, 0xe4, 0xf0,
    /* 00000099 sub rsp,byte +0x20 */
    0x48, 0x83, 0xec, 0x20,
    /* 0000009d call qword [rax+0x10] */
    0xff, 0x50, 0x10,
    /* 000000a0 mov rsp,rbp */
    0x48, 0x89, 0xec,
    /* 000000a3 popfq */
    0x9d,
    /* 000000a4 pop rbp */
    0x5d,
    /* 000000a5 pop rsi */
    0x5e,
    /* 000000a6 pop rdi */
    0x5f,
    /* 000000a7 pop rax */
    0x58,
    /* 000000a8 pop rbx */
    0x5b,
    /* 000000a9 pop rcx */
    0x59,
    /* 000000aa pop rdx */
    0x5a,
    /* 000000ab pop r8 */
    0x41, 0x58,
    /* 000000ad pop r9 */
    0x41, 0x59,
    /* 000000af pop r10 */
    0x41, 0x5a,
    /* 000000b1 pop r11 */
    0x41, 0x5b,
    /* 000000b3 pop r12 */
    0x41, 0x5c,
    /* 000000b5 pop r13 */
    0x41, 0x5d,
    /* 000000b7 pop r14 */
    0x41, 0x5e,
    /* 000000b9 pop r15 */
    0x41, 0x5f,
    /* 000000bb pop rsp */
    0x5c,
    /* 000000bc ret */
    0xc3
};