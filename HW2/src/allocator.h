#ifndef OTUSHOMEWORKS_ALLOCATOR_H
#define OTUSHOMEWORKS_ALLOCATOR_H
#include "pch.h"

std::stringstream err;

template <typename Iterator>
struct Chunk {
    Iterator first , last;

    Chunk () : first(nullptr), last(nullptr) {}

    Chunk(Iterator f, Iterator l) : first(f), last(l){}

    Iterator begin () const {
        return first;
    }

    Iterator end () const {
        return last;
    }

    Iterator begin () {
        return first;
    }

    Iterator end () {
        return last;
    }

    [[nodiscard]] size_t size() const {
        return last - first;
    }
};


template <typename T, std::size_t N = 1>
struct PoolAllocator{
public:
    using value_type = T;

    template<typename U>
    struct rebind{
        using other = PoolAllocator<U, N>;
    };


    T* allocate(size_t n) const{
        static_assert(N > 0);
       // std::cout << __PRETTY_FUNCTION__ << std::endl;
           if (pool.size() == 0) {
               if (n > N) throw std::bad_alloc();
               size_t size = N * sizeof(T);
               auto ptr = std::malloc(size);
               pool.emplace_back(reinterpret_cast<T *>(ptr), reinterpret_cast<T *>(ptr) + size);
            //   std::cerr << "M " << pool.back().begin() << ", " << size << ", " << sizeof(T) << std::endl;
            //   err << "A " << pool.back().begin() << std::endl;
               counter += n;
               mem_s += N;
               return pool.back().begin();
           } else {
               if ((counter += n) <= ((pool.back().size()) / sizeof(T))) {
              //     err << "A " << (chunk_.value()).begin() + (free_pos_ - n) << std::endl;
                   return pool.back().begin() + (counter - n);
               } else {
                   counter = n;
                   mem_s *= 2;
                   while (mem_s < n)
                        mem_s *= 2;
                   size_t size = sizeof(T) * mem_s * n;
                   auto ptr = std::malloc(size);
                   pool.emplace_back(reinterpret_cast<T *>(ptr), reinterpret_cast<T *>(ptr) + size);
             //      std::cerr << "M " << pool.back().begin() << std::endl;
             //      err << "A " << pool.back().begin() << std::endl;
                   return pool.back().begin();
               }
           }

    }

    void deallocate(T* p, size_t n) const{
      //  std::cout << __PRETTY_FUNCTION__ << std::endl;
        bool to_Dealloc = 0;
        for (auto & i : pool)
        {
            if (i.begin() == p)
            {
                to_Dealloc = 1;
                break;
            }
        }
        if (to_Dealloc) {
       //     err << "F " << p << std::endl;
            std::free(p);
        }
    }

    template <typename U, typename ... Args>
    void construct(U* p, Args&&... args) const{
      //  err << "C " << p << std::endl;
      //  std::cout << __PRETTY_FUNCTION__ << std::endl;
        new(p) U(std::forward<Args>(args) ...);
    }

    template <class U>
    void destroy(U* p) const{
     //   std::cout << __PRETTY_FUNCTION__ << std::endl;
     //   err << "D " << p << std::endl;
        p->~U();
        counter -= 1;
    }

private:
    mutable std::vector<Chunk<T*>> pool;
    mutable size_t mem_s = 0;
    mutable size_t counter = 0;
};


#endif //OTUSHOMEWORKS_ALLOCATOR_H
