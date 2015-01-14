#pragma once

#include <cstdint>

typedef struct Header{
    void *thisValue;
    void *origEntry;
} header_t;

extern "C" header_t *headerPtr;

extern "C" uint32_t allSize;
extern "C" uint32_t entryOffs;
extern "C" uint32_t leaveOffs;