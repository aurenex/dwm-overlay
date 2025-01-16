#include "vtablehook/include/vtablehook.h"

int unprotect(void* region)
{
    MEMORY_BASIC_INFORMATION mbi;
    VirtualQuery((LPCVOID)region, &mbi, sizeof(mbi));
    VirtualProtect(mbi.BaseAddress, mbi.RegionSize, PAGE_READWRITE, &mbi.Protect);
    return mbi.Protect;
}

void protect(void* region, int protection)
{
    MEMORY_BASIC_INFORMATION mbi;
    VirtualQuery((LPCVOID)region, &mbi, sizeof(mbi));
    VirtualProtect(mbi.BaseAddress, mbi.RegionSize, protection, &mbi.Protect);
}

/*
* instance: pointer to an instance of a class
* hook: function to overwrite with
* offset: 0 = method 1, 1 = method 2 etc...
* return: original function
*/

void* vtable::hook(void* instance, void* hook, int offset)
{
    intptr_t vtable = *((intptr_t*)instance);
    intptr_t entry = vtable + sizeof(intptr_t) * offset;
    intptr_t original = *((intptr_t*)entry);

    int original_protection = unprotect((void*)entry);
    *((intptr_t*)entry) = (intptr_t)hook;
    protect((void*)entry, original_protection);

    return (void*)original;
}