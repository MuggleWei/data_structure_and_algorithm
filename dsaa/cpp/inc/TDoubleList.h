#ifndef __T_DOUBLE_LIST_H__
#define __T_DOUBLE_LIST_H__

#include "base.h"
#include <stddef.h>
#include <new>

#if ENABLE_DSAA_OPTIMIZATION
#include "memory_pool.h"
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
#if ENABLE_DSAA_OPTIMIZATION
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
#if ENABLE_DSAA_OPTIMIZATION
		, pool_(rref.pool_)
#endif
	{
		rref.head_ = nullptr;
		rref.tail_ = nullptr;
		rref.count_ = 0;
#if ENABLE_DSAA_OPTIMIZATION
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

#if ENABLE_DSAA_OPTIMIZATION
		pool_ = rref.pool_;
		rref.pool_ = nullptr;
#endif

		return *this;
	}

	typedef bool (*PtrFunc_Compare)(const T& ref1, const T& ref2);

	bool IsEmpty()
	{
		return count_ == 0;
	}
	bool IsOrdered(PtrFunc_Compare ptr_compare_func)
	{
		TDoubleListNode<T>* p;
		for (p = First(); !IsLast(p); p = p->Next())
		{
			if (!IsLast(p->Next()))
			{
				if (!(*ptr_compare_func)(*p->Get(), *p->Next()->Get()))
				{
					return false;
				}
			}
		}

		return true;
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
	TDoubleListNode<T>* Find(const T &ref_data, TDoubleListNode<T> *start_node = nullptr, PtrFunc_Compare ptr_compare_func = nullptr)
	{
		TDoubleListNode<T> *node = (start_node == nullptr) ? head_->next_ : start_node;
		while (node != tail_)
		{
			bool compare_result;

			if (ptr_compare_func)
			{
				compare_result = (*ptr_compare_func)(node->data_, ref_data);
			}
			else
			{
				compare_result = (node->data_ == ref_data);
			}

			if (compare_result)
			{
				return node;
			}
			node = node->next_;
		}

		return nullptr;
	}
	TDoubleListNode<T>* Insert(const T &ref_data, PtrFunc_Compare ptr_compare_func = nullptr)
	{
		TDoubleListNode<T>* ret = nullptr;

		if (ptr_compare_func == nullptr)
		{
			ret = Insert(ref_data, head_->next_);
		}
		else
		{
			TDoubleListNode<T>* p;
			for (p = First(); !IsLast(p); p = p->Next())
			{
				if (!(*ptr_compare_func)(*p->Get(), ref_data))
				{
					ret = Insert(ref_data, p);
					break;
				}
			}

			if (IsLast(p))
			{
				ret = Add(ref_data);
			}
		}

		return ret;

// #if ENABLE_DSAA_OPTIMIZATION
// 		TDoubleListNode<T>* new_node = new ((void*)MemoryPoolAlloc(pool_)) TDoubleListNode<T>(ref_data);
// #else
// 		TDoubleListNode<T>* new_node = new TDoubleListNode<T>(ref_data);
// #endif
// 		new_node->next_ = head_->next_;
// 		new_node->next_->prev_ = new_node;
// 		head_->next_ = new_node;
// 		new_node->prev_ = head_;
// 		++count_;
	}
	TDoubleListNode<T>* Insert(const T &ref_data, TDoubleListNode<T>* next_node)
	{
#if ENABLE_DSAA_OPTIMIZATION
		TDoubleListNode<T>* new_node = new ((void*)MemoryPoolAlloc(pool_)) TDoubleListNode<T>(ref_data);
#else
		TDoubleListNode<T>* new_node = new TDoubleListNode<T>(ref_data);
#endif
		new_node->next_ = next_node;
		new_node->prev_ = next_node->prev_;
		new_node->next_->prev_ = new_node;
		new_node->prev_->next_ = new_node;
		++count_;

		return new_node;
	}
	TDoubleListNode<T>* Add(const T &ref_data)
	{
		return Add(ref_data, tail_->prev_);

// #if ENABLE_DSAA_OPTIMIZATION
// 		TDoubleListNode<T>* new_node = new ((void*)MemoryPoolAlloc(pool_)) TDoubleListNode<T>(ref_data);
// #else
// 		TDoubleListNode<T>* new_node = new TDoubleListNode<T>(ref_data);
// #endif
// 		new_node->prev_ = tail_->prev_;
// 		tail_->prev_->next_ = new_node;
// 		new_node->next_ = tail_;
// 		tail_->prev_ = new_node;
// 		++count_;
	}
	TDoubleListNode<T>* Add(const T &ref_data, TDoubleListNode<T>* prev_node)
	{
#if ENABLE_DSAA_OPTIMIZATION
		TDoubleListNode<T>* new_node = new ((void*)MemoryPoolAlloc(pool_)) TDoubleListNode<T>(ref_data);
#else
		TDoubleListNode<T>* new_node = new TDoubleListNode<T>(ref_data);
#endif
		new_node->prev_ = prev_node;
		new_node->next_ = prev_node->next_;
		new_node->prev_->next_ = new_node;
		new_node->next_->prev_ = new_node;
		++count_;

		return new_node;
	}
	void Remove(const TDoubleListNode<T> *node)
	{
		node->prev_->next_ = node->next_;
		node->next_->prev_ = node->prev_;
#if ENABLE_DSAA_OPTIMIZATION
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
#if ENABLE_DSAA_OPTIMIZATION
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

#if ENABLE_DSAA_OPTIMIZATION
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

#if ENABLE_DSAA_OPTIMIZATION
			MemoryPoolDestroy(pool_);
			free(pool_);
			pool_ = nullptr;
#endif
		}
	}
	void copy(const TDoubleList<T> &ref)
	{
#if ENABLE_DSAA_OPTIMIZATION
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

#if ENABLE_DSAA_OPTIMIZATION
	MemoryPool*             pool_;
#endif
};

#endif