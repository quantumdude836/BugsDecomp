
// Wrapper class around memory allocated via VirtualAllocEx.

#pragma once


class ProcessMemory
{
public:
    /// <summary>
    /// Tries to allocate some memory in a given process. The memory is
    /// allocated in reserve+commit mode with read/write permissions. The
    /// process handle is borrowed.
    /// </summary>
    /// <param name="hProcess">Handle to process to allocate memory in</param>
    /// <param name="size">Size, in bytes, to allocate</param>
    /// <returns>Allocated memory, if successful</returns>
    static std::optional<ProcessMemory> tryCreate(HANDLE hProcess, size_t size);

private:
    /// <summary>
    /// Creates a new instance of process memory.
    /// </summary>
    /// <param name="hProcess">Handle to process containing the memory</param>
    /// <param name="memBase">Base address of memory region</param>
    /// <param name="memSize">Size, in bytes, of memory region</param>
    ProcessMemory(HANDLE hProcess, void *memBase, size_t memSize);
public:
    ProcessMemory(const ProcessMemory &) = delete;
    /// <summary>
    /// Move constructor.
    /// </summary>
    /// <param name="from">Instance to move from</param>
    ProcessMemory(ProcessMemory &&from) noexcept;
    ProcessMemory &operator =(const ProcessMemory &) = delete;
    /// <summary>
    /// Move assignment operator.
    /// </summary>
    /// <param name="from">Instance to move from</param>
    /// <returns>Reference to this</returns>
    ProcessMemory &operator =(ProcessMemory &&from) noexcept;

public:
    /// <summary>
    /// Returns the base address of the memory.
    /// </summary>
    /// <returns>Base address of the memory, in remote process space</returns>
    void *baseAddress() const { return memBase; }

public:
    /// <summary>
    /// Tries to write some data into the process.
    /// </summary>
    /// <param name="dstOffset">
    /// Destination offset, in bytes, into the memory region
    /// </param>
    /// <param name="src">Pointer to source data</param>
    /// <param name="size">Number of bytes to write</param>
    /// <returns>Number of bytes successfully written</returns>
    size_t write(size_t dstOffset, const void *src, size_t size);

public:
    /// <summary>
    /// Destructor. Frees memory region.
    /// </summary>
    ~ProcessMemory();

private:
    // process handle
    HANDLE hProcess;
    // base address, in target process address space
    void *memBase;
    // size of memory, in bytes
    size_t memSize;
};
