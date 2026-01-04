#ifndef BUDDY_ALLOCATOR_HPP
#define BUDDY_ALLOCATOR_HPP
#include <vector>
#include <set>
#include <cstddef>
#include <map>
using namespace std;
class BuddyAllocator
{
private:
    size_t totsize;
    size_t minblksize;
    int levelmax;
    vector<set<size_t>> freelist;
    map<size_t, size_t> blocksallocated;
    size_t totalused = 0;
    size_t totalrequest = 0;
    int sizeToLevel(size_t size) const;
    size_t levelToSize(int level) const;
    size_t getBuddy(size_t addr, size_t size) const;

public:
    BuddyAllocator(size_t total, size_t min_block);
    size_t alloc(size_t size);
    void freeBlock(size_t addr);
    void stats() const;
    void dump() const;
};

#endif