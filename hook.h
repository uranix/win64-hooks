#pragma once

#include "hook_asm.h"

#include <memory>

#include <cstdint>
#include <stdexcept>

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
    JmpInstr(u64 from, u64 to) {
        i64 dist = to - (from + SIZE);
        if (dist > INT32_MAX || dist < INT32_MIN)
            throw std::range_error("jump distance is too far");
        this->dist = static_cast<i32>(dist);
    }
    Bytes<SIZE> write(void *memp) {
        Bytes<SIZE> ret(memp);
        u8 *mem = reinterpret_cast<u8 *>(memp);
        mem[0] = 0xe9;
        memcpy(mem + 1, &dist, 4);
        return ret;
    }
};

class Hook {
public:
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
private:

    void *extend;
    u64 oldret;
    void *addr;

    std::unique_ptr<JmpInstr> hook2extend;

    Bytes<JmpInstr::SIZE> stolen;

public:
    Hook(void *addr);
    virtual ~Hook();
private:
    u32 unprotect() const;
    void protect(u32 flags) const;
    void patchStart();
    void restoreStart();
    void spoofRet(u64 *pret);
    void restoreRet(u64 *pret);
    virtual void onEnter(Context *ctx, u64 *pret);
    virtual void onLeave(Context *ctx, u64 *pret);
public:
    virtual void enter(Context *) = 0;
    virtual void leave(Context *) = 0;
};

