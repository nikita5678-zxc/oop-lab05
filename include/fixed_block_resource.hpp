#pragma once

#include <iostream>

#include <memory_resource>
#include <list>

class FixedBlockMemoryResource: public std::pmr::memory_resource {
public:
    explicit FixedBlockMemoryResource(std::size_t size);
    ~FixedBlockMemoryResource();

protected:
    void * do_allocate(size_t bytes, size_t alignment) override;
    void do_deallocate(void* ptr, size_t bytes, size_t alignment);
    bool do_is_equal(const std::pmr::memory_resource& other) const noexcept override;

private:
    struct AllocationInfo {
        void* ptr;
        size_t size;
        size_t alignment;
    };

    char *buffer;
    size_t size;
    std::list<AllocationInfo> free_blocks;
};