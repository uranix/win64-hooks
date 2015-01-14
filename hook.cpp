#include "hook.h"

#include <Windows.h>
#include <iostream>
#include <cstdlib>
#include <string>

#include "hook_bytes.h"

u32 Hook::unprotect() const {
    DWORD oldflags, flags;
    flags = PAGE_EXECUTE_READWRITE;
    VirtualProtect(addr, JmpInstr::SIZE, flags, &oldflags);
    return oldflags;
}

void Hook::protect(u32 flags) const {
    DWORD oldflags;
    VirtualProtect(addr, JmpInstr::SIZE, flags, &oldflags);
}

Hook::Hook(void *addr) : addr(addr) {
    try {
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
                throw std::out_of_range("Could not find suitable page");
            if (mbi.State == MEM_FREE)
                break;
            allocat = static_cast<u8 *>(mbi.BaseAddress) + mbi.RegionSize;
        } while (true);
        extend = (u8 *)VirtualAlloc(allocat, 0x1000, MEM_RESERVE | MEM_COMMIT, PAGE_EXECUTE_READWRITE);
        if (!extend) {
            throw std::out_of_range(std::string("VirtualAlloc failed: ") + std::to_string(GetLastError()));
        }
        memcpy(extend, data, sizeof(data));
        *reinterpret_cast<void **>(extend + THIS1) = this;
        *reinterpret_cast<void **>(extend + THIS2) = this;
        hook2extend = std::unique_ptr<JmpInstr>(new JmpInstr(reinterpret_cast<u64>(addr), reinterpret_cast<u64>(extend)));
        JmpInstr extend2hook(reinterpret_cast<u64>(extend + JMPADDR1), reinterpret_cast<u64>(addr));
        extend2hook.write(extend + JMPADDR1);
        patchStart();
    } catch (std::exception &e) {
        std::cerr << e.what() << std::endl;
        abort();
    }
}

Hook::~Hook() {
    restoreStart();
    VirtualFree(extend, 0, MEM_RELEASE);
}

void Hook::patchStart() {
    u32 fl = unprotect();
    stolen = hook2extend->write(addr);
    protect(fl);
}

void Hook::restoreStart() {
    u32 fl = unprotect();
    stolen.write(addr);
    protect(fl);
}

void Hook::spoofRet(u64 *pret) {
    oldret = *pret;
    *pret = reinterpret_cast<u64>(extend + RETBLOCK);
}

void Hook::restoreRet(u64 *pret) {
    *pret = oldret;
}

void Hook::onEnter(Hook::Context *ctx, u64 *pret) {
    restoreStart();
    enter(ctx);
    spoofRet(pret);
}

void Hook::onLeave(Hook::Context *ctx, u64 *pret) {
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
