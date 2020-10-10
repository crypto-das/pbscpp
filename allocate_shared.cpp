#include <iostream>
#include <memory>

using NewFunctionPtr = void *(*)(size_t);

class A
{
public:
    static void * operator new(size_t size)
    {
        std::cout << "operator new!" << std::endl;
        return ::operator new(size);
    }
    static void operator delete(void *p, size_t size)
    {
        std::cout << "operator delete!" << std::endl;
        return ::operator delete(p);
    }
};

template<class T>
class MyAlloc
{
public:
    typedef T value_type;
    
    MyAlloc() : parent_type_new_func(operator new) {}
    
    template<class U>
    MyAlloc(const MyAlloc<U> &u_alloc) noexcept
        : parent_type_new_func(MyAlloc<U>::value_type::operator new)
    {}

    T* allocate(size_t n)
    {
        return static_cast<T*>(parent_type_new_func(n * sizeof(T)));
    }
    
    void deallocate(T *p, size_t n)
    {
        ::delete(p);
    }
    
    NewFunctionPtr parent_type_new_func;
};

int main()
{
    MyAlloc<A> alloc;
    auto sp1 = std::make_shared<A>();
    auto sp2 = std::allocate_shared<A>(alloc);
    return 0;
}
