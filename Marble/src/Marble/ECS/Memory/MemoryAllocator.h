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
    struct DebugInfo {
        std::string allocatorName;
        size_t initByteSize = 0;
        uint32_t allocations = 0;
        size_t totalBytesAllocated = 0;
        size_t maxBytesAllocated = 0;
        uint32_t deallocations = 0;
    };

    #define ECS_MEM_DEBUG 0 
    //MBL_ASSERT(x, ...) { if (!(x)) { MBL_ERROR("Assertion Failed: {0}", __VA_ARGS__); MBL_DEBUGBREAK(); } }
    #ifdef ECS_MEM_DEBUG
        #if defined(__GNUC__) || (defined(__MWERKS__) && (__MWERKS__ >= 0x3000)) || (defined(__ICC) && (__ICC >= 600)) || defined(__ghs__)
            #define ECS_FUNC_SIG __PRETTY_FUNCTION__
        #elif defined(__DMC__) && (__DMC__ >= 0x810)
            #define ECS_FUNC_SIG __PRETTY_FUNCTION__
        #elif defined(__FUNCSIG__)
            #define ECS_FUNC_SIG __FUNCSIG__
        #elif (defined(__INTEL_COMPILER) && (__INTEL_COMPILER >= 600)) || (defined(__IBMCPP__) && (__IBMCPP__ >= 500))
            #define ECS_FUNC_SIG __FUNCTION__
        #elif defined(__BORLANDC__) && (__BORLANDC__ >= 0x550)
            #define ECS_FUNC_SIG __FUNC__
        #elif defined(__STDC_VERSION__) && (__STDC_VERSION__ >= 199901)
            #define ECS_FUNC_SIG __func__
        #elif defined(__cplusplus) && (__cplusplus >= 201103)
            #define ECS_FUNC_SIG __func__
        #else
            #define ECS_FUNC_SIG "ECS_FUNC_SIG unknown!"
        #endif

        //#define MBL_PROFILE_BEGIN_SESSION(name, filepath) ::Marble::Instrumentor::Get().BeginSession(name, filepath)
        //#define MBL_PROFILE_END_SESSION() ::Marble::Instrumentor::Get().EndSession()
        //#define MBL_PROFILE_SCOPE(name) ::Marble::InstrumentationTimer timer##__LINE__(name);
        //#define MBL_PROFILE_FUNCTION() MBL_PROFILE_SCOPE (MBL_FUMBL_SIG) //void __cdecl Marble::Instrumentor::Stop(void)

        #define DEBUG_MEM_INFO(name, init_size)          DebugInfo debugInfo = {name, init_size};
        #define DEBUG_PRINT_MEM_INFO()  { \
                                          std::cout << "<<" << debugInfo.allocatorName << ">>" << std::endl; \
                                          std::cout << "Initial Bytes allocated: " << debugInfo.initByteSize << std::endl; \
                                          std::cout << "Allocations: " << debugInfo.allocations << std::endl; \
                                          std::cout << "deallocations: " << debugInfo.deallocations << std::endl; \
                                          std::cout << "Maximum Bytes Allocated: " << debugInfo.maxBytesAllocated << std::endl; \
                                          std::cout << "Total Bytes Allocated: " << debugInfo.totalBytesAllocated << std::endl; \
                                        }
        #define DEBUG_ADD_ALLOC(x) { debugInfo.totalBytesAllocated += x; debugInfo.allocations++; }
        #define DEBUG_ADD_DEALLOC() debugIfno.deallocations++;
    #else
        #define DEBUG_MEM_INFO(name, init_size)
        #define DEBUG_PRINT_MEM_INFO()
        #define DEBUG_ADD_ALLOC(x) 
        #define DEBUG_ADD_DEALLOC()
    #endif // ECS_MEM_DEBUG

    


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

        DEBUG_MEM_INFO(ECS_FUNC_SIG, s);
    public:
        HeapAllocator() { data = new byte[s];}
        ~HeapAllocator() { delete[] data; DEBUG_PRINT_MEM_INFO(); }
        MemBlk allocate(size_t n){
            auto n1 = allign(n);
            if (n1 > (data + s) - ptr)
                return {nullptr, 0};
            MemBlk result = {ptr, n};
            DEBUG_ADD_ALLOC(n1);
            ptr += n1;
            return result;
        }
        void deallocate(MemBlk b) {
            DEBUG_ADD_DEALLOC();
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

