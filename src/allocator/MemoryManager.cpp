#include "MemoryManager.hpp"
#include <iostream>
#include <iomanip>
using namespace std;
MemoryManager::MemoryManager(size_t size)
    : total_memory(size), allocator(AllocatorType::FIRST_FIT), next_id(1)
{
    blocks.emplace_back(0, size);
}
list<Block>::iterator MemoryManager::findBlock(size_t size)
{
    list<Block>::iterator best = blocks.end();
    for (auto it = blocks.begin(); it != blocks.end(); ++it)
    {
        if (!it->free || it->size < size)
            continue;
        if (allocator == AllocatorType::FIRST_FIT)
            return it;
        if (allocator == AllocatorType::BEST_FIT)
        {
            if (best == blocks.end() || it->size < best->size)
                best = it;
        }
        if (allocator == AllocatorType::WORST_FIT)
        {
            if (best == blocks.end() || it->size > best->size)
                best = it;
        }
    }
    return best;
}
void MemoryManager::splitBlock(list<Block>::iterator it, size_t size)
{
    if (it->size == size)
        return;
    Block newBlock(it->start + size, it->size - size);
    it->size = size;
    blocks.insert(next(it), newBlock);
}
int MemoryManager::mallocMem(size_t size)
{
    requests++;
    auto it = findBlock(size);
    if (it == blocks.end())
    {
        failedalloc++;
        cout << "Allocation failed\n";
        return -1;
    }
    successalloc++;
    splitBlock(it, size);
    it->free = false;
    it->id = next_id++;
    it->requested_size = size;
    cout << "Allocated block id=" << it->id
              << " at address=0x" << hex << it->start << dec << "\n";
    return it->id;
}
void MemoryManager::freeMem(int id)
{
    for (auto &b : blocks)
    {
        if (b.id == id)
        {
            b.free = true;
            b.id = -1;
            coalesce();
            cout << "Block " << id << " freed\n";
            return;
        }
    }
    cout << "Invalid block id\n";
}
void MemoryManager::coalesce()
{
    for (auto it = blocks.begin(); it != blocks.end();)
    {
        auto next_it = next(it); 
        if (next_it != blocks.end() && it->free && next_it->free)
        {
            it->size += next_it->size;
            blocks.erase(next_it);
        }
        else
        {
            ++it;
        }
    }
}
void MemoryManager::dump() const
{
    for (const auto &b : blocks)
    {
        cout << "[0x" << hex << b.start
                  << " - 0x" << (b.start + b.size - 1) << "] "
                  << (b.free ? "FREE" : "USED") << "\n";
    }
    cout << dec;
}
void MemoryManager::stats() const
{
    size_t used = 0, free = 0;
    size_t largest_free = 0;
    size_t total_internal_frag = 0;
    for (const auto &b : blocks)
    {
        if (b.free)
        {
            free += b.size;
            largest_free = max(largest_free, b.size);
        }
        else
        {
            used += b.size;
            total_internal_frag += (b.size - b.requested_size);
        }
    }
    double external_frag = 0.0;
    if (free > 0)
    {
        external_frag = 1.0 - (double)largest_free / free;
    }
    cout << "Total memory: " << total_memory << "\n";
    cout << "Used memory: " << used << "\n";
    cout << "Free memory: " << free << "\n";
    cout << "Internal fragmentation: " << total_internal_frag << " bytes\n";
    cout << "External fragmentation: " << external_frag * 100 << "%\n";
    cout << "Allocation requests: " << requests << "\n";
    cout << "Successful allocations: " << successalloc << "\n";
    cout << "Failed allocations: " << failedalloc << "\n";
    double success_rate = requests
                              ? (double)successalloc / requests
                              : 0.0;
    cout << "Allocation success rate: "
              << success_rate * 100 << "%\n";
}
void MemoryManager::setAllocator(AllocatorType type)
{
    allocator = type;
}
