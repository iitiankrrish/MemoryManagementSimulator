#include "BuddyAllocator.hpp"
#include <iostream>
#include <cmath>
#include <algorithm>
using namespace std;
BuddyAllocator::BuddyAllocator(size_t total, size_t min_block)
    : totsize(total), minblksize(min_block)
{
    levelmax = log2(totsize / minblksize);
    freelist.resize(levelmax + 1);
    freelist[0].insert(0);
}

int BuddyAllocator::sizeToLevel(size_t size) const
{
    int level = 0;
    size_t block = minblksize;
    while (block < size)
    {
        block <<= 1;
        level++;
    }
    int target = levelmax - level;
    return (target < 0) ? 0 : target;
}

size_t BuddyAllocator::levelToSize(int level) const
{
    return totsize >> level;
}

size_t BuddyAllocator::getBuddy(size_t addr, size_t size) const
{
    return addr ^ size;
}

size_t BuddyAllocator::alloc(size_t size)
{
    if (size > totsize)
        return static_cast<size_t>(-1);
    int level = sizeToLevel(size);
    size_t actual_block_size = levelToSize(level);
    for (int i = level; i >= 0; i--)
    {
        if (!freelist[i].empty())
        {
            size_t addr = *freelist[i].begin();
            freelist[i].erase(addr);
            while (i < level)
            {
                i++;
                size_t split_size = levelToSize(i);
                freelist[i].insert(addr + split_size);
            }
            blocksallocated[addr] = size;
            totalused += actual_block_size;
            totalrequest += size;
            return addr;
        }
    }
    return static_cast<size_t>(-1);
}

void BuddyAllocator::freeBlock(size_t addr)
{
    auto it_map = blocksallocated.find(addr);
    if (it_map == blocksallocated.end())
    {
        cout << "Error: Address not allocated.\n";
        return;
    }
    size_t requested_size = it_map->second;
    int level = sizeToLevel(requested_size);
    size_t block_size = levelToSize(level);
    totalused -= block_size;
    totalrequest -= requested_size;
    blocksallocated.erase(it_map);
    while (level > 0)
    {
        size_t current_block_size = levelToSize(level);
        size_t buddy = getBuddy(addr, current_block_size);
        auto &list = freelist[level];
        auto it_buddy = list.find(buddy);
        if (it_buddy != list.end())
        {
            list.erase(it_buddy);
            addr = min(addr, buddy);
            level--;
        }
        else
        {
            break;
        }
    }
    freelist[level].insert(addr);
}
void BuddyAllocator::dump() const
{
    for (int i = 0; i <= levelmax; i++)
    {
        cout << "Level " << i << " ("
                  << levelToSize(i) << " bytes): ";
        for (auto addr : freelist[i])
            cout << addr << " ";
        cout << "\n";
    }
}
void BuddyAllocator::stats() const
{
    size_t internal_frag = totalused - totalrequest;
    size_t total_free = totsize - totalused;
    size_t largest_free = 0;
    for (int i = 0; i <= levelmax; i++)
    {
        if (!freelist[i].empty())
        {
            largest_free = levelToSize(i);
            break; 
        }
    }
    double external_frag = 0.0;
    if (total_free > 0)
    {
        external_frag = 1.0 - (double)largest_free / total_free;
    }
    cout << "--- Buddy Allocator Stats ---\n";
    cout << "Total Memory: " << totsize << "\n";
    cout << "Internal Fragmentation: " << internal_frag << " bytes\n";
    cout << "External Fragmentation: " << external_frag * 100 << "%\n";
    cout << "Utilization: " << ((double)totalrequest / totsize) * 100 << "%\n";
}