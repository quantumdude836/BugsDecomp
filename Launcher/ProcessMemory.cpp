
#include "stdafx.h"
#include "ProcessMemory.h"


std::optional<ProcessMemory> ProcessMemory::tryCreate(
    HANDLE hProcess,
    size_t size
)
{
    void *mem = VirtualAllocEx(
        hProcess,
        nullptr,
        size,
        MEM_RESERVE | MEM_COMMIT,
        PAGE_READWRITE
    );
    if (!mem)
        return std::nullopt;

    return ProcessMemory(hProcess, mem, size);
}


ProcessMemory::ProcessMemory(HANDLE hProcess, void *memBase, size_t memSize) :
    hProcess(hProcess),
    memBase(memBase),
    memSize(memSize)
{
}

ProcessMemory::ProcessMemory(ProcessMemory &&from) noexcept :
    hProcess(from.hProcess),
    memBase(from.memBase),
    memSize(from.memSize)
{
    from.memBase = nullptr;
}

ProcessMemory &ProcessMemory::operator =(ProcessMemory &&from) noexcept
{
    hProcess = from.hProcess;
    memBase = from.memBase;
    memSize = from.memSize;

    from.memBase = nullptr;

    return *this;
}


size_t ProcessMemory::write(size_t dstOffset, const void *src, size_t size)
{
    SIZE_T numWritten;

    // don't allow offset/size to exceed region
    if (dstOffset + size > memSize)
    {
        if (dstOffset >= memSize)
            return 0;
        size = memSize - dstOffset;
    }

    // write memory
    BOOL res = WriteProcessMemory(
        hProcess,
        reinterpret_cast<char *>(memBase) + dstOffset,
        src,
        size,
        &numWritten
    );
    if (!res)
        return 0;

    return numWritten;
}


ProcessMemory::~ProcessMemory()
{
    if (memBase)
        VirtualFreeEx(hProcess, memBase, 0, MEM_RELEASE);
    // process handle is borrowed; do not release it
}
