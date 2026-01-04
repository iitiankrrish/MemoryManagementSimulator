#include "CacheLevel.hpp"
#include <iostream>
using namespace std;
CacheLevel::CacheLevel(size_t csize, size_t bsize, size_t assoc)
    : cachesize(csize), blocksize(bsize), associativity(assoc), hits(0), misses(0)
{
    size_t divisor = blocksize * associativity;
    if (divisor == 0 || csize < divisor)
    {
        sets = 1;
    }
    else
    {
        sets = cachesize / divisor;
    }
    cache.resize(sets);
}

bool CacheLevel::access(size_t address)
{
    size_t block_addr = address / blocksize;
    size_t index = block_addr % sets;
    size_t tag = block_addr / sets;

    auto &set = cache[index];
    for (auto it = set.begin(); it != set.end(); ++it)
    {
        if (it->valid && it->tag == tag)
        {
            hits++;
            return true;
        }
    }
    misses++;
    if (set.size() >= associativity)
    {
        if (!set.empty())
        {
            set.pop_front();
        }
    }
    CacheLine newline;
    newline.valid = true;
    newline.tag = tag;
    set.push_back(newline);

    return false;
}

void CacheLevel::printStats(const char *name) const
{
    size_t total = hits + misses;
    double hit_rate = total ? (double)hits / total : 0.0;

    cout << "--- " << name << " Statistics (FIFO) ---\n";
    cout << "  Hits: " << hits << "\n";
    cout << "  Misses: " << misses << "\n";
    cout << "  Hit rate: " << (hit_rate * 100) << "%\n";
}