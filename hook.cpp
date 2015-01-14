#include "hook.h"
#include "hook_asm.h"

#include <Windows.h>
#include <cstdlib>

JmpInstr::JmpInstr(u64 from, u64 to) {
    i64 dist = to - (from + SIZE);
    if (dist > INT32_MAX || dist < INT32_MIN)
        throw STATUS_INTEGER_OVERFLOW;
    this->dist = static_cast<i32>(dist);
}
Bytes<JmpInstr::SIZE> JmpInstr::write(void *memp) {
    Bytes<JmpInstr::SIZE> ret(memp);
    u8 *mem = reinterpret_cast<u8 *>(memp);
    mem[0] = 0xe9;
    memcpy(mem + 1, &dist, 4);
    return ret;
}

u32 HookBase::unprotect() const {
    DWORD oldflags, flags;
    flags = PAGE_EXECUTE_READWRITE;
    VirtualProtect(addr, JmpInstr::SIZE, flags, &oldflags);
    return oldflags;
}

void HookBase::protect(u32 flags) const {
    DWORD oldflags;
    VirtualProtect(addr, JmpInstr::SIZE, flags, &oldflags);
}

static LPVOID GetNextFreePage(LPVOID addr) {
    MEMORY_BASIC_INFORMATION mbi;
    u8 *allocat = static_cast<u8 *>(addr);
    u64 gran = 0x10000;
    do {
        u64 p = reinterpret_cast<u64>(allocat);
        p--;
        p |= (gran - 1);
        p++;
        allocat = reinterpret_cast<u8 *>(p);
        size_t ret = VirtualQuery(allocat, &mbi, sizeof(mbi));
        if (ret == 0)
            throw STATUS_NO_MEMORY;
        if (mbi.State == MEM_FREE)
            break;
        allocat = static_cast<u8 *>(mbi.BaseAddress) + mbi.RegionSize;
    } while (true);
    return allocat;
}

Hook::Hook(void *addr) {
    this->addr = addr;
    try {
        extend = VirtualAlloc(GetNextFreePage(addr), 0x1000, MEM_RESERVE | MEM_COMMIT, PAGE_EXECUTE_READWRITE);
        if (!extend) {
            throw GetLastError();
        }
        memcpy(extend, headerPtr, allSize);
        static_cast<Header *>(extend)->origEntry = addr;
        static_cast<Header *>(extend)->thisValue = this;
        hook2extend = std::unique_ptr<JmpInstr>(new JmpInstr(reinterpret_cast<u64>(addr), reinterpret_cast<u64>(extend) + entryOffs));
        patchStart();
    } catch (...) {
        abort();
    }
}

Hook::~Hook() {
    restoreStart();
    VirtualFree(extend, 0, MEM_RELEASE);
}

void HookBase::patchStart() {
    u32 fl = unprotect();
    stolen = hook2extend->write(addr);
    protect(fl);
}

void HookBase::restoreStart() {
    u32 fl = unprotect();
    stolen.write(addr);
    protect(fl);
}

void HookBase::spoofRet(u64 *pret) {
    oldret = *pret;
    *pret = reinterpret_cast<u64>(extend) + leaveOffs;
}

void HookBase::restoreRet(u64 *pret) {
    *pret = oldret;
}

void Hook::onEnter(Context *ctx, u64 *pret) {
    restoreStart();
    enter(ctx);
    spoofRet(pret);
}

void Hook::onLeave(Context *ctx, u64 *pret) {
    restoreRet(pret);
    leave(ctx);
    patchStart();
}


/*

a: call foo
b:

foo:
...
ret: ret

Orig flow: a -> foo -> ret -> b
Hooked flow: a -> foo (jmp extend) -> extend(save ret to b and patch) -> foo(orig) -> ret(to patched) -> retcode(ret to b) -> b

*/
