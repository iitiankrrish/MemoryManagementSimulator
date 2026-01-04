#ifndef CACHE_LEVEL_HPP
#define CACHE_LEVEL_HPP

#include <vector>
#include <deque>
#include <cstddef>
#include "Cache.hpp"
using namespace std;
enum class ReplacementPolicy {
    FIFO,
    LRU
};

class CacheLevel {
private:
    size_t cachesize;
    size_t blocksize;
    size_t associativity;
    size_t sets;

    vector<deque<CacheLine>> cache;

public:
    size_t hits;
    size_t misses;

    CacheLevel(size_t cachesize,
               size_t blocksize,
               size_t associativity);

    bool access(size_t address);
    void printStats(const char* name) const;
};

#endif
