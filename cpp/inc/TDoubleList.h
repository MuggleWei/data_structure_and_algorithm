#ifndef __T_DOUBLE_LIST_H__
#define __T_DOUBLE_LIST_H__

#include <stdlib.h>
#include <stddef.h>
#include <assert.h>
#include <new>
#include "macros.h"

#if ENABLE_DATA_STRUCTURE_OPTIMIZATION
extern "C"
{
#include "memory_pool.h"
}
#endif

template<typename T>
class TDoubleList;

template<typename T>
class TDoubleListNode
{
public:
    TDoubleListNode()
    {
        next_ = nullptr;
        prev_ = nullptr;
    }
    explicit TDoubleListNode(const T& data_ref)
        : data_(data_ref)
    {
        next_ = nullptr;
        prev_ = nullptr;
    }
    TDoubleListNode* Next()
    {
        return next_;
    }
    TDoubleListNode* Prev()
    {
        return prev_;
    }
    T* Get()
    {
        return &data_;
    }

private:
    T                   data_;
    TDoubleListNode*    next_;
    TDoubleListNode*    prev_;

    friend class TDoubleList < T > ;
};

template<typename T>
class TDoubleList
{
public:
    TDoubleList(size_t hint_pool_size = 8)
    {
#if ENABLE_DATA_STRUCTURE_OPTIMIZATION
        pool_ = (MemoryPool*)malloc(sizeof(MemoryPool));
        MemoryPoolInit(pool_, hint_pool_size, sizeof(TDoubleListNode<T>));
        head_ = new ((void*)MemoryPoolAlloc(pool_)) TDoubleListNode<T>();
        tail_ = new ((void*)MemoryPoolAlloc(pool_)) TDoubleListNode<T>();
#else
        head_ = new TDoubleListNode<T>();
        tail_ = new TDoubleListNode<T>();
#endif
        head_->next_ = tail_;
        head_->prev_ = nullptr;
        tail_->prev_ = head_;
        tail_->next_ = nullptr;
        count_ = 0;
    }
    TDoubleList(const TDoubleList<T> &ref)
    {
        copy(ref);
    }
    TDoubleList(TDoubleList<T> &&rref) noexcept
        : head_(rref.head_)
        , tail_(rref.tail_)
        , count_(rref.count_)
#if ENABLE_DATA_STRUCTURE_OPTIMIZATION
        , pool_(rref.pool_)
#endif
    {
        rref.head_ = nullptr;
        rref.tail_ = nullptr;
        rref.count_ = 0;
#if ENABLE_DATA_STRUCTURE_OPTIMIZATION
        rref.pool_ = nullptr;
#endif
    }
    ~TDoubleList()
    {
        deallocate();
    }

    TDoubleList<T>& operator=(const TDoubleList<T> &ref)
    {
        if (this != &ref)
        {
            deallocate();
            copy(ref);
        }
        return *this;
    }
    TDoubleList<T>& operator=(TDoubleList<T> &&rref) noexcept
    {
        deallocate();

        head_ = rref.head_;
        tail_ = rref.tail_;
        count_ = rref.count_;

        rref.head_ = nullptr;
        rref.tail_ = nullptr;
        rref.count_ = 0;

#if ENABLE_DATA_STRUCTURE_OPTIMIZATION
        pool_ = rref.pool_;
        rref.pool_ = nullptr;
#endif

        return *this;
    }

    bool IsEmpty()
    {
        return count_ == 0;
    }
    TDoubleListNode<T>* First()
    {
        return head_->next_;
    }
    TDoubleListNode<T>* Last()
    {
        return tail_;
    }
    bool IsLast(TDoubleListNode<T> *node)
    {
        return node == tail_;
    }
    TDoubleListNode<T>* Find(const T &ref_data, TDoubleListNode<T> *start_node = nullptr)
    {
        TDoubleListNode<T> *node = (start_node == nullptr) ? head_->next_ : start_node;
        while (node != tail_)
        {
            if (node->data_ == ref_data)
            {
                return node;
            }
            node = node->next_;
        }

        return nullptr;
    }
    void Insert(const T &ref_data)
    {
#if ENABLE_DATA_STRUCTURE_OPTIMIZATION
        TDoubleListNode<T>* new_node = new ((void*)MemoryPoolAlloc(pool_)) TDoubleListNode<T>(ref_data);
#else
        TDoubleListNode<T>* new_node = new TDoubleListNode<T>(ref_data);
#endif
        new_node->next_ = head_->next_;
        new_node->next_->prev_ = new_node;
        head_->next_ = new_node;
        new_node->prev_ = head_;
        ++count_;
    }
    void Add(const T &ref_data)
    {
#if ENABLE_DATA_STRUCTURE_OPTIMIZATION
        TDoubleListNode<T>* new_node = new ((void*)MemoryPoolAlloc(pool_)) TDoubleListNode<T>(ref_data);
#else
        TDoubleListNode<T>* new_node = new TDoubleListNode<T>(ref_data);
#endif
        new_node->prev_ = tail_->prev_;
        tail_->prev_->next_ = new_node;
        new_node->next_ = tail_;
        tail_->prev_ = new_node;
        ++count_;
    }
    void Remove(const TDoubleListNode<T> *node)
    {
        node->prev_->next_ = node->next_;
        node->next_->prev_ = node->prev_;
#if ENABLE_DATA_STRUCTURE_OPTIMIZATION
        node->~TDoubleListNode<T>();
        MemoryPoolFree(pool_, (void*)node);
#else
        delete(node);
#endif
        --count_;
    }
    void MakeEmpty()
    {
        TDoubleListNode<T> *node = head_->next_;
        while (node != tail_)
        {
            node->prev_->next_ = node->next_;
            node->next_->prev_ = node->prev_;
#if ENABLE_DATA_STRUCTURE_OPTIMIZATION
            node->~TDoubleListNode<T>();
            MemoryPoolFree(pool_, node);
#else
            delete(node);
#endif
            node = head_->next_;
        }
        count_ = 0;
    }
    size_t Count()
    {
        return count_;
    }

private:
    void deallocate()
    {
        if (head_ != nullptr)
        {
            MakeEmpty();
            
#if ENABLE_DATA_STRUCTURE_OPTIMIZATION
            head_->~TDoubleListNode<T>();
            tail_->~TDoubleListNode<T>();
            MemoryPoolFree(pool_, head_);
            MemoryPoolFree(pool_, tail_);
#else
            delete(head_);
            delete(tail_);
#endif            
            head_ = nullptr;
            tail_ = nullptr;

#if ENABLE_DATA_STRUCTURE_OPTIMIZATION
            MemoryPoolDestroy(pool_);
            free(pool_);
            pool_ = nullptr;
#endif
        }
    }
    void copy(const TDoubleList<T> &ref)
    {
#if ENABLE_DATA_STRUCTURE_OPTIMIZATION
        pool_ = (MemoryPool*)malloc(sizeof(MemoryPool));
        MemoryPoolInit(pool_, ref.pool_->used, sizeof(TDoubleListNode<T>));
        head_ = new ((void*)MemoryPoolAlloc(pool_)) TDoubleListNode<T>();
        tail_ = new ((void*)MemoryPoolAlloc(pool_)) TDoubleListNode<T>();
#else
        head_ = new TDoubleListNode<T>();
        tail_ = new TDoubleListNode<T>();
#endif
        head_->next_ = tail_;
        head_->prev_ = nullptr;
        tail_->prev_ = head_;
        tail_->next_ = nullptr;
        count_ = 0;

        for (TDoubleListNode<T>* node = ref.head_->next_; node != ref.tail_; node = node->next_)
        {
            Add(node->data_);
        }
    }

private:
    TDoubleListNode<T>*     head_;
    TDoubleListNode<T>*     tail_;
    size_t                  count_;

#if ENABLE_DATA_STRUCTURE_OPTIMIZATION
    MemoryPool*             pool_;
#endif
};

#endif