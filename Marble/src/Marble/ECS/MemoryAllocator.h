#pragma once
#include "stdlib.h"

namespace ECS
{
    using byte = unsigned char;

    struct Blk {
        void* ptr;
        size_t size;
    };

    static size_t roundToAligned(size_t n){
        if (n % 2 == 0) return n;
        return n + (n % 2);
    }

    template<class A, size_t blockSize>
    class BitmappedBlock;

    template <class A, ulong flags>
    class AllocatorWithStats;

    template <class A, class Prefix, class Suffix = void>
    class AffixAllocator;

    // A is the parent allocator
    // Add more optimizations 
    template <class A, size_t s>
    class FreeList {
        A parent_;
        struct Node {Node* next;} root_;
    public:
        Blk allocate(size_t n) {
            if (n == s && root_) {
                Blk b = {root_, n};
                root_ = root_.next;
                return b;
            }
            return parent_.allocate(n);
        }
        void deallocate(Blk b) {
            if (b.size != s) return parent_.deallocate(b);
            auto p = (Node*)b.ptr;
            p.next = root_;
            root_ = p;
        } 
        bool owns(Blk b){
            return b.size == s || parent_.owns(b); //Bug? 
        }
    };

    template<size_t s>
    class HeapAllocator {
        byte* data;
        byte* ptr;
        HeapAllocator() {data = new byte[s]; ptr = data;}
    public:
        Blk allocate(size_t n){
            auto n1 = roundToAligned(n);
            if (n1 > (data + s) - ptr)
                return {nullptr, 0};
            Blk result = {ptr, n};
            ptr += n1;
            return result;
        }
        void deallocate(Blk b) {
            if (b.ptr + roundToAligned(b.size) == ptr) ptr = b.ptr; //bug
        } 
        bool owns(Blk b){
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
        StackAllocator() : ptr(data) {}
    public:
        Blk allocate(size_t n) {
            auto n1 = roundToAligned(n);
            if (n1 > (data + s) - ptr)
                return {nullptr, 0};
            Blk result = {ptr, n};
            ptr += n1;
            return result;
        }
        void deallocate(Blk b) {
            if (b.ptr + roundToAligned(b.size) == ptr) ptr = b.ptr; //bug
        } 
        bool owns(Blk b){
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
        Blk allocate(size_t n) {
            Blk r = Primary::allocate(n);
            if (!r.ptr) r = Fallback::allocate(n);
            return r;
        }
        void deallocate(Blk b){
            if (Primary::owns(b)) Primary::deallocate(b);
            else Fallback::deallocate(b);
        }
        bool owns(Blk b){
            return Primary::owns(b) || Fallback::owns(b);
        }
    };
} // namespace name

