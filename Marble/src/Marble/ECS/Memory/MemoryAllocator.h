#pragma once
#include "stdlib.h"

#ifdef _WIN32
/*Windows x64/x86*/
    #ifdef _WIN64
        #include <windows.h>
    #endif
#endif

namespace ECS
{
    /*
    API:
    static constexpr unsigned alignment;
    static constexpr goodSize(size_t);
    Blk allocate(size_t);
    Blk allocateAll(); //stack
    bool expand(Blk&, size_t delta);
    void reallocate(Blk&, size_t);
    bool owns(Blk);
    void deallocate(Blk);
    void deallocateAll();

    Blk alignedAllocate(size_t, unsigned);
    Blk alignedReallocate(size_t, unsigned);

    AlignedMallocator
    */

    using word_t = uintptr_t;
    using byte = unsigned char;

    template <typename T = void>
    struct TypeBlk {
        T* ptr;
        size_t size;
    };

    //template <typename T = void>
    struct MemBlk {
        void* ptr;
        size_t size;
    };

    inline static size_t allign(size_t n){
        return (n + sizeof(word_t) - 1) & ~(sizeof(word_t) - 1);
    }

    template<class Creator>
    class CascadingAllocator;

    template<class A, size_t blockSize>
    class BitmappedBlock;

    template<class A, uint64_t flags>
    class AllocatorWithStats;

    template<class A, class Prefix, class Suffix = void>
    class AffixAllocator;

    // A is the parent allocator
    // Add more optimizations 
    // Add tolerance: min ... max
    // Allocate in batches 
    // add upper bound: no more than top elems
    template<class A, size_t s>
    class FreeList {
        A parent_;
        struct Node {Node* next;} root_;
    public:
        MemBlk allocate(size_t n) {
            if (n == s && root_) {
                MemBlk b = {root_, n};
                root_ = root_.next;
                return b;
            }
            return parent_.allocate(n);
        }
        void deallocate(MemBlk b) {
            if (b.size != s) return parent_.deallocate(b);
            auto p = static_cast<Node*>(b.ptr);
            p.next = root_;
            root_ = p;
        } 
        bool owns(MemBlk b){
            return b.size == s || parent_.owns(b); //Bug? 
        }
    };

    template<size_t s>
    class HeapAllocator {
        byte* data;
        byte* ptr;
    public:
        HeapAllocator() { data = new byte[s]; ptr = data; }
        ~HeapAllocator() { delete[] data; }
        MemBlk allocate(size_t n){
            auto n1 = allign(n);
            if (n1 > (data + s) - ptr)
                return {nullptr, 0};
            MemBlk result = {ptr, n};
            ptr += n1;
            return result;
        }
        void deallocate(MemBlk b) {
            if (b.ptr + allign(b.size) == ptr) ptr = b.ptr; //bug
        } 
        bool owns(MemBlk b){
            return b.ptr >= data && b.ptr < data + s;
        }
        void deallocateAll() {
            ptr = data;
        }
    };

    template<size_t s>
    class StackAllocator {
        byte  data[s];
        byte* ptr;
    public:
        StackAllocator() : ptr(data) {}
        MemBlk allocate(size_t n) {
            auto n1 = allign(n);
            if (n1 > (data + s) - ptr)
                return {nullptr, 0};
            MemBlk result = {ptr, n};
            ptr += n1;
            return result;
        }
        void deallocate(MemBlk b) {
            if (b.ptr + allign(b.size) == ptr) ptr = b.ptr; //bug
        } 
        bool owns(MemBlk b){
            return b.ptr >= data && b.ptr < data + s;
        }
        void deallocateAll() {
            ptr = data;
        }
    };

    template<class Primary, class Fallback>
    class FallbackAllocator
        : private Primary
        , private Fallback {
    public:
        MemBlk allocate(size_t n) {
            MemBlk r = Primary::allocate(n);
            if (!r.ptr) r = Fallback::allocate(n);
            return r;
        }
        void deallocate(MemBlk b){
            if (Primary::owns(b)) Primary::deallocate(b);
            else Fallback::deallocate(b);
        }
        bool owns(MemBlk b){
            return Primary::owns(b) || Fallback::owns(b);
        }
    };
} // namespace name

