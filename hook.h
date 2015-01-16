#pragma once

#ifdef HOOK_EXPORTS
#define HOOK_API __declspec(dllexport) 
#else
#define HOOK_API __declspec(dllimport) 
#endif

#include <memory>
#include <cstdint>

#define WIN32_LEAN_AND_MEAN
#define VC_EXTRALEAN
#include <Windows.h>

typedef  uint8_t u8;
typedef uint32_t u32;
typedef uint64_t u64;
typedef  int32_t i32;
typedef  int64_t i64;

template<int n>
struct Bytes {
    u8 buf[n];
    Bytes() { }
    Bytes(void *p) {
        memcpy(buf, p, n);
    }
    void write(void *p) {
        memcpy(p, buf, n);
    }
};

struct JmpInstr {
    enum { SIZE = 5 };
    i32 dist;
    JmpInstr(u64 from, u64 to);
    Bytes<SIZE> write(void *memp);
};

struct Context {
    u64 rflags;
    u64 rbp;
    u64 rsi;
    u64 rdi;
    u64 rax;
    u64 rbx;
    u64 rcx;
    u64 rdx;
    u64 r8;
    u64 r9;
    u64 r10;
    u64 r11;
    u64 r12;
    u64 r13;
    u64 r14;
    u64 r15;
    u64 rsp;
};

struct JmpInstr;

struct HookBase {
    std::unique_ptr<CRITICAL_SECTION> cs;
    void *extend;
    u64 oldret;
    void *addr;
    volatile bool exiting;

    std::unique_ptr<JmpInstr> hook2extend;

    Bytes<JmpInstr::SIZE> stolen;
    u32 unprotect() const;
    void protect(u32 flags) const;
    void patchStart();
    void restoreStart();
    void spoofRet(u64 *pret);
    void restoreRet(u64 *pret);
};

struct HOOK_API Hook : protected HookBase {
    Hook(void *addr);
    virtual ~Hook();
    virtual void onEnter(Context *ctx, u64 *pret);
    virtual void onLeave(Context *ctx, u64 *pret);
    virtual void enter(Context *) = 0;
    virtual void leave(Context *) = 0;
};