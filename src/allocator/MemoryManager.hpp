#ifndef MEMORY_MANAGER_H
#define MEMORY_MANAGER_H
#include <list>
#include <string>
#include "Block.hpp"
using namespace std;
enum class AllocatorType
{
    FIRST_FIT,
    BEST_FIT,
    WORST_FIT
};
class MemoryManager
{
private:
    size_t total_memory;
    list<Block> blocks;
    AllocatorType allocator;
    int next_id;
    size_t requests = 0;
    size_t successalloc = 0;
    size_t failedalloc = 0;
    list<Block>::iterator findBlock(size_t size);
    void splitBlock(list<Block>::iterator it, size_t size);
    void coalesce();

public:
    MemoryManager(size_t size);
    void setAllocator(AllocatorType type);
    int mallocMem(size_t size);
    void freeMem(int id);
    void dump() const;
    void stats() const;
};

#endif
