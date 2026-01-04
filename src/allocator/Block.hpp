#ifndef BLOCK_H
#define BLOCK_H
#include <cstddef>
struct Block
{
    size_t start;
    size_t size;
    size_t requested_size;
    bool free;
    int id;
    Block(size_t s, size_t sz)
        : start(s), size(sz), requested_size(0), free(true), id(-1) {}
};
#endif
