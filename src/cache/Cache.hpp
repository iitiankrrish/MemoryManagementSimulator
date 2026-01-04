#ifndef CACHE_HPP
#define CACHE_HPP

#include <cstddef>

struct CacheLine {
    bool valid;
    size_t tag;

    CacheLine() : valid(false), tag(0) {}
};

#endif
