CXX = g++
CXXFLAGS = -std=c++17 -Wall -Isrc
all:
	$(CXX) $(CXXFLAGS) src/main.cpp \
       src/allocator/MemoryManager.cpp \
       src/buddy/BuddyAllocator.cpp \
       src/cache/CacheLevel.cpp -o memsim

clean:
	rm -f memsim