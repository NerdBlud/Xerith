#ifndef XERITH_ARENA_H
#define XERITH_ARENA_H

#include <vector>
#include <cstddef>
#include <cstdint>

namespace xerith {

class Arena {
public:
    // Default chunk size is 4KB
    explicit Arena(size_t chunk_size = 4096);
    ~Arena();

    // Prevent copying the arena to avoid double-frees
    Arena(const Arena&) = delete;
    Arena& operator=(const Arena&) = delete;

    // The main function: Grab some memory
    void* alloc(size_t size);

    // Template helper to make allocating objects easier
    template <typename T, typename... Args>
    T* construct(Args&&... args) {
        void* mem = alloc(sizeof(T));
        return new (mem) T(std::forward<Args>(args)...);
    }

private:
    struct Chunk {
        uint8_t* data;
        size_t size;
        size_t used;
    };

    void grow(size_t min_size);

    size_t default_chunk_size;
    std::vector<Chunk> chunks;
};

} // namespace xerith

#endif // XERITH_ARENA_H