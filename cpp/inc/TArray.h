#ifndef __T_ARRAY_H__
#define __T_ARRAY_H__

#include <stdlib.h>
#include <stddef.h>
#include <assert.h>
#include "macros.h"

template<typename T>
class TArray
{
public:
    explicit TArray(size_t init_capacity = 8)
    {
        capacity_ = init_capacity >= 8 ? init_capacity : 8;
        datas_ = static_cast<T*>(malloc(sizeof(T) * capacity_));
        used_ = 0;
    }
    TArray(const TArray &ref)
    {
        copy(ref);
    }
    TArray(TArray &&rref) noexcept
        : datas_(rref.datas_)
        , capacity_(rref.capacity_)
        , used_(rref.used_)
    {
        rref.datas_ = nullptr;
        rref.capacity_ = 0;
        rref.used_ = 0;
    }
    ~TArray()
    {
        deallocate();
    }

    TArray& operator=(const TArray &ref)
    {
        if (this != &ref)
        {
            deallocate();
            copy(ref);
        }

        return *this;
    }
    TArray& operator=(TArray &&rref) noexcept
    {
        if (this != rref)
        {
             deallocate();

             datas_ = rref.datas_;
             capacity_ = rref.capacity_;
             used_ = rref.used_;

             rref.datas_ = nullptr;
             rref.capacity_ = 0;
             rref.used_ = 0;
        }
        return *this;
    }
    T& operator[](size_t index)
    {
        assert(index < used_);
        return datas_[index];
    }

    bool IsEmpty()
    {
        return used_ == 0;
    }
    bool IsFull()
    {
        return used_ == capacity_;
    }
    const T& Top()
    {
        assert(!IsEmpty());
        return datas_[used_ - 1];
    }
    void Push(const T &ref)
    {
        if (IsFull())
        {
            EnsureSpace(capacity_ * 2);
        }

        new ((void*)(datas_ + used_)) T(ref);
        ++used_;
    }
    void Pop()
    {
        assert(!IsEmpty());
        T *p = &datas_[--used_];
        p->~T();
    }
    const T& Get(size_t index) const
    {
        assert(index < used_);
        return datas_[index];
    }
    void Insert(size_t index, const T &ref)
    {
        assert(index <= used_);
        if (IsFull())
        {
            EnsureSpace(capacity_ * 2);
        }

        T *begin = datas_ + used_;
        T *end = datas_ + index;
        while (begin != end)
        {
            new ((void*)begin) T(std::move(*(begin - 1)));
            --begin;
            begin->~T();
        }
        new ((void*)begin) T(ref);
        ++used_;
    }
    void Remove(size_t index)
    {
        assert(index < used_);

        T *begin = datas_ + index;
        T *end = datas_ + used_ - 1;

        begin->~T();
        while (begin != end)
        {
            new ((void*)begin) T(std::move(*(begin + 1)));
            ++begin;
            begin->~T();
        }
        --used_;
    }
    void EnsureSpace(size_t capacity)
    {
        if (capacity_ < capacity)
        {
            size_t used = used_;
            T *new_data = static_cast<T*>(malloc(sizeof(T) * capacity));
            for (size_t i =0; i<used_; ++i)
            {
                new ((void*)(new_data + i)) T(std::move(datas_[i]));
            }
            deallocate();

            datas_ = new_data;
            capacity_ = capacity;
            used_ = used;            
        }
    }

    size_t Used() const
    {
        return used_;
    }
    size_t Capacity() const
    {
        return capacity_;
    }

private:
    void deallocate()
    {
        if (datas_ != nullptr)
        {
            for (T* p = datas_; p != datas_ + used_; ++p)
            {
                p->~T();
            }
            free(datas_);

            datas_ = nullptr;
            capacity_ = 0;
            used_ = 0;
        }        
    }
    void copy(const TArray &ref)
    {
        capacity_ = ref.capacity_;
        used_ = ref.used_;
        datas_ = static_cast<T*>(malloc(sizeof(T) * capacity_));
        for (size_t i = 0; i < used_; ++i)
        {
            new ((void*)(datas_ + i)) T(ref.datas_[i]);
        }
    }

private:
    T*      datas_;
    size_t  capacity_;
    size_t  used_;
};

#endif