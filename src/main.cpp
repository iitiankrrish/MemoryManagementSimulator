#include <iostream>
#include <sstream>
#include <string>
#include "allocator/MemoryManager.hpp"
#include "cache/CacheLevel.hpp"
#include "buddy/BuddyAllocator.hpp"
using namespace std;
int main()
{
    MemoryManager *mm = nullptr;
    BuddyAllocator *buddy = nullptr;
    bool use_buddy = false;
    CacheLevel *L1 = nullptr;
    CacheLevel *L2 = nullptr;
    string line;
    cout << "Memory Simulator\n";
    while (true)
    {
        cout << "> ";
        if (!getline(cin, line))
            break;
        stringstream ss(line);
        string cmd;
        ss >> cmd;
        if (cmd == "exit")
        {
            break;
        }
        else if (cmd == "init")
        {
            string type;
            size_t size;
            ss >> type >> size;
            if (type == "memory")
            {
                delete mm;
                delete buddy;
                mm = new MemoryManager(size);
                buddy = nullptr;
                use_buddy = false;
                cout << "Standard Memory Initialized.\n";
            }
            else if (type == "buddy")
            {
                size_t min_block;
                ss >> min_block;
                delete mm;
                delete buddy;
                buddy = new BuddyAllocator(size, min_block);
                mm = nullptr;
                use_buddy = true;
                cout << "Buddy Allocator Initialized.\n";
            }
        }
        else if (cmd == "set")
        {
            string subcmd, policy;
            ss >> subcmd >> policy;
            if (subcmd != "allocator")
            {
                cout << "Usage: set allocator <first_fit|best_fit|worst_fit>\n";
                continue;
            }
            if (use_buddy)
            {
                cout << "Currently in the buddy mode , allocation algorithms are not valid in this mode.\n";
                continue;
            }
            if (!mm)
            {
                cout << "Initialize memory first\n";
                continue;
            }
            if (policy == "first_fit")
                mm->setAllocator(AllocatorType::FIRST_FIT);
            else if (policy == "best_fit")
                mm->setAllocator(AllocatorType::BEST_FIT);
            else if (policy == "worst_fit")
                mm->setAllocator(AllocatorType::WORST_FIT);
            else
            {
                cout << "Unknown allocator\n";
                continue;
            }
            cout << "Allocator set to " << policy << "\n";
        }
        else if (cmd == "malloc")
        {
            size_t size;
            ss >> size;
            if (use_buddy && buddy)
            {
                size_t addr = buddy->alloc(size);
                if (addr != (size_t)-1)
                    cout << "Allocated at address: 0x" << hex << addr << dec << "\n";
                else
                    cout << "Buddy allocation failed.\n";
            }
            else if (mm)
            {
                mm->mallocMem(size);
            }
            else 
            {
                cout << "Error: Initialize memory first (use 'init memory' or 'init buddy')\n";
            }
        }
        else if (cmd == "free")
        {
            if (use_buddy && buddy)
            {
                size_t addr;
                ss >> hex >> addr >> dec;
                buddy->freeBlock(addr);
                cout << "Address 0x" << hex << addr << dec << " freed and merged.\n";
            }
            else if (mm)
            {
                int id;
                ss >> id;
                mm->freeMem(id);
            }
        }
        else if (cmd == "dump")
        {
            if (use_buddy && buddy)
                buddy->dump();
            else if (mm)
                mm->dump();
            else
                cout << "Initialize memory first\n";
        }
        else if (cmd == "stats")
        {
            if (use_buddy && buddy)
                buddy->stats();
            else if (mm)
                mm->stats();
        }
        else if (cmd == "cache")
        {
            string subcmd;
            ss >> subcmd;
            if (subcmd == "init")
            {
                string level;
                size_t cache_size, block_size, associativity;
                if (!(ss >> level >> cache_size >> block_size >> associativity))
                {
                    cout << "Usage: cache init <L1|L2> <cache_size> <block_size> <assoc>\n";
                    continue;
                }
                if (level == "L1")
                {
                    delete L1;
                    L1 = new CacheLevel(cache_size, block_size, associativity);
                    cout << "Initialized L1 cache\n";
                }
                else if (level == "L2")
                {
                    delete L2;
                    L2 = new CacheLevel(cache_size, block_size, associativity);
                    cout << "Initialized L2 cache\n";
                }
                else
                {
                    cout << "Unknown cache level\n";
                }
            }
            else if (subcmd == "access")
            {
                size_t address;
                if (!(ss >> std::hex >> address))
                {
                    std::cout << "Invalid address. Please use hexadecimal (e.g., 0x100)\n";
                    ss.clear();
                    continue;
                }
                ss >> std::dec;

                if (!L1)
                {
                    std::cout << "Initialize cache first\n";
                    continue;
                }

                bool hitL1 = L1->access(address);
                if (hitL1)
                {
                    std::cout << "L1 HIT\n";
                }
                else
                {
                    std::cout << "L1 MISS\n";
                    if (L2)
                    {
                        bool hitL2 = L2->access(address);
                        std::cout << (hitL2 ? "L2 HIT\n" : "L2 MISS\n");
                    }
                }
            }
            else if (subcmd == "stats")
            {
                if (L1)
                    L1->printStats("L1 Cache");
                if (L2)
                    L2->printStats("L2 Cache");
            }
            else if (subcmd == "reset")
            {
                delete L1;
                delete L2;
                L1 = nullptr;
                L2 = nullptr;
                cout << "Cache reset\n";
            }

            else
            {
                cout << "Unknown cache command\n";
            }
        }
        else
        {
            cout << "Unknown command\n";
        }
    }

    delete mm;
    delete buddy;
    delete L1;
    delete L2;
    return 0;
}
