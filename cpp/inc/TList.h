#ifndef __T_LIST_H__
#define __T_LIST_H__

#include <stdlib.h>
#include <stddef.h>
#include <assert.h>
#include "macros.h"

#if ENABLE_C_DATA_STRUCTURE_OPTIMIZATION
#include "memory_pool.h"
#endif

template<typename T>
class TList;

template<typename T>
class TListNode
{
public:
    TListNode()
    {
        next_ = nullptr;
    }
    TListNode(const T& data_ref)
        : data_(data_ref)
    {
        next_ = nullptr;
    }
    TListNode* Next()
    {
        return next_;
    }
    T* Get()
    {
        return &data_;
    }

private:
    T           data_;
    TListNode*  next_;

    friend class TList<T>;
};

template<typename T>
class TList
{
public:
    TList()
    {
        head_ = new TListNode<T>();
        head_->next_ = nullptr;
        count_ = 0;
    }
    TList(const TList<T> &ref)
    {
        copy(ref);
    }
    TList(const TList<T> &&rref) noexcept
        : head_(rref.head_)
        , count_(rref.count_)
    {
        rref.head_ = nullptr;
        rref.count_ = 0;
    }
    ~TList()
    {
        deallocate();
    }

    TList<T>& operator=(const TList<T> &ref)
    {
        if (this != &ref)
        {
            deallocate();
            copy(ref);
        }

        return *this;
    }
    TList<T>& operator=(const TList<T> &&rref) noexcept
    {
        if (this != rref)
        {
            deallocate();

            head_ = rref.head_;
            count_ = rref.count_;

            rref.head_ = nullptr;
            rref.count_ = 0;
        }
        return *this;
    }
    
    bool IsEmpty()
    {
        return head_->next_ == nullptr;
    }
    TListNode<T>* First()
    {
        return head_->next_;
    }
    bool IsLast(TListNode<T> *ptr_node)
    {
        return ptr_node->next_ == nullptr;
    }
    TListNode<T>* Find(const T &ref_data, TListNode<T>* start_node = nullptr)
    {
        TListNode<T> *node = (start_node == nullptr) ? head_->next_ : start_node;
        while (node != nullptr)
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
        TListNode<T> *node = new TListNode<T>(ref_data);
        node->next_ = head_->next_;
        head_->next_ = node;
        ++count_;
    }
    bool FindAndRemove(const T &ref_data)
    {
        TListNode<T> **pp_node = &head_->next_;
        while (*pp_node != nullptr)
        {
            if (*(**pp_node).Get() == ref_data)
            {
                assert(count_ != 0);
                TListNode<T> *p_node = *pp_node;
                *pp_node = (*pp_node)->next_;
                delete(p_node);
                --count_;
                return true;
            }
            else
            {
                pp_node = &(*pp_node)->next_;
            }
        }

        return false;
    }
    void MakeEmpty()
    {
        while (head_->next_ != nullptr)
        {
            TListNode<T>* node = head_->next_;
            head_->next_ = head_->next_->next_;
            delete(node);
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
            while (head_->next_ != nullptr)
            {
                TListNode<T>* node = head_->next_;
                head_->next_ = head_->next_->next_;
                delete(node);
            }

            delete(head_);
            head_ = nullptr;
            count_ = 0;
        }
    }
    void copy(const TList<T> &ref)
    {
        head_ = new TListNode<T>();
        head_->next_ = nullptr;
        count_ = ref.count_;
        
        TListNode<T>* node = ref.head_->next_;
        TListNode<T>* current_node = head_;
        while (node != nullptr)
        {
            TListNode<T>* new_node = new TListNode<T>(node->data_);
            current_node->next_ = new_node;
            current_node = new_node;
            current_node->next_ = nullptr;
            node = node->next_;
        }
    }

private:
    TListNode<T>*   head_;
    size_t          count_;
};

#endif