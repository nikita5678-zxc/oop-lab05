#include "../include/fixed_block_resource.hpp"
#include <new>
#include <algorithm> 
#include <cstddef>

FixedBlockMemoryResource::FixedBlockMemoryResource(std::size_t size)
    : buffer(static_cast<char*>(std::malloc(size)))
    , size(size)
    , free_blocks(this)
{
    if (!buffer) {
        throw std::bad_alloc{};
    }
    free_blocks.push_back(AllocationInfo{
        buffer, size, alignof(std::max_align_t)
    });
}

FixedBlockMemoryResource::~FixedBlockMemoryResource() {
    std::free(buffer);
}

void* FixedBlockMemoryResource::do_allocate(std::size_t bytes, std::size_t alignment) {
    if (bytes == 0) return nullptr;

    for (auto it = free_blocks.begin(); it != free_blocks.end(); ++it) {
        AllocationInfo block = *it;

        void* candidate = block.ptr;     
        std::size_t space = block.size; 

        void* aligned_ptr = std::align(alignment, bytes, candidate, space);

        if (aligned_ptr != nullptr) {
            free_blocks.erase(it);

            char* used_start = static_cast<char*>(aligned_ptr);
            char* used_end   = used_start + bytes;
            char* block_end  = static_cast<char*>(block.ptr) + block.size;

            if (used_start > static_cast<char*>(block.ptr)) {
                free_blocks.push_back(AllocationInfo{
                    block.ptr,
                    static_cast<size_t>(used_start - static_cast<char*>(block.ptr)),
                    alignof(std::max_align_t)
                });
            }

            if (used_end < block_end) {
                free_blocks.push_back(AllocationInfo{
                    used_end,
                    static_cast<size_t>(block_end - used_end),
                    alignof(std::max_align_t)
                });
            }

            return aligned_ptr;
        }
    }

    throw std::bad_alloc{};
}

void FixedBlockMemoryResource::do_deallocate(void* ptr, std::size_t bytes, std::size_t alignment) {
    if (!ptr || bytes == 0) {
        return;
    }
    free_blocks.push_back(AllocationInfo{ptr, bytes, alignment});
}

bool FixedBlockMemoryResource::do_is_equal(const std::pmr::memory_resource& other) const noexcept {
    return this == &other;
}