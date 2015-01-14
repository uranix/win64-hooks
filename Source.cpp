#include "hook.h"

int foo(int x, int y, int z) {
    char ret[8];
    for (int i = 0; i < 8; i++)
        ret[i] = x * y + i;
    for (int i = 0; i < 8; i++)
        z += ret[i] * ret[i];
    return z;
}

int bar(int z) {
    int a = 10;
    int b = 41;

    return foo(a, b, z);
}

#include <iostream>

struct HookFoo : public Hook {
    HookFoo(void *p) : Hook(p) { }
    virtual void enter(Context *ctx) override {
        std::cout << "foo(" << ctx->rcx << ", " << ctx->rdx << ", " << ctx->r8 << ")";
    }
    virtual void leave(Context *ctx) override {
        std::cout << " = " << ctx->rax << std::endl;
        ctx->rax = 42;
    }
};

int main() {
    HookFoo m(foo);
    std::cout << "Bar = " << bar(0) << std::endl;
    std::cout << "Bar = " << bar(1) << std::endl;
    std::cout << "Bar = " << bar(2) << std::endl;
    std::cout << "Bar = " << bar(3) << std::endl;
    char z;
    std::cin >> z;
    return 0;
}