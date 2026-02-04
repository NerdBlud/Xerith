#include "arena.h"
#include <cstdlib>
#include <algorithm>

namespace xerith {

Arena::Arena(size_t chunk_size) : default_chunk_size(chunk_size) {}

Arena::~Arena() {
    for (auto& chunk : chunks) {
        free(chunk.data);
    }
}

void* Arena::alloc(size_t size) {
    // Ensure 8-byte alignment for performance and safety
    size = (size + 7) & ~7;

    if (chunks.empty() || chunks.back().used + size > chunks.back().size) {
        grow(size);
    }

    auto& last = chunks.back();
    void* ptr = last.data + last.used;
    last.used += size;
    return ptr;
}

void Arena::grow(size_t min_size) {
    size_t size = std::max(min_size, default_chunk_size);
    uint8_t* data = static_cast<uint8_t*>(malloc(size));
    chunks.push_back({data, size, 0});
}

} // namespace xerith