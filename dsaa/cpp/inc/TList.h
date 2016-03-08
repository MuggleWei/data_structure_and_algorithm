#ifndef __T_LIST_H__
#define __T_LIST_H__

#include "base.h"
#include <new>

#if ENABLE_DSAA_OPTIMIZATION
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
	explicit TListNode(const T& data_ref)
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

	friend class TList < T > ;
};

template<typename T>
class TList
{
public:
	TList(size_t hint_pool_size = 8)
	{
#if ENABLE_DSAA_OPTIMIZATION
		pool_ = (MemoryPool*)malloc(sizeof(MemoryPool));
		MemoryPoolInit(pool_, hint_pool_size, sizeof(TListNode<T>));
		head_ = new ((void*)MemoryPoolAlloc(pool_)) TListNode<T>();
#else
		head_ = new TListNode<T>();
#endif
		head_->next_ = nullptr;
		count_ = 0;
	}
	TList(const TList<T> &ref)
	{
		copy(ref);
	}
	TList(TList<T> &&rref) noexcept
		: head_(rref.head_)
		, count_(rref.count_)
#if ENABLE_DSAA_OPTIMIZATION
		, pool_(rref.pool_)
#endif
	{
		rref.head_ = nullptr;
		rref.count_ = 0;
#if ENABLE_DSAA_OPTIMIZATION
		rref.pool_ = nullptr;
#endif
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
	TList<T>& operator=(TList<T> &&rref) noexcept
	{
		if (this != &rref)
		{
			deallocate();

			head_ = rref.head_;
			count_ = rref.count_;

			rref.head_ = nullptr;
			rref.count_ = 0;

#if ENABLE_DSAA_OPTIMIZATION
			pool_ = rref.pool_;
			rref.pool_ = nullptr;
#endif
		}
		return *this;
	}

	typedef bool (*PtrFunc_Compare)(const T& ref1, const T& ref2);

	bool IsEmpty()
	{
		return head_->next_ == nullptr;
	}
	bool IsOrdered(PtrFunc_Compare ptr_compare_func)
	{
		TListNode<T>* p;
		for (p = First(); p; p = p->next_)
		{
			if (p->next_)
			{
				if (!(*ptr_compare_func)(p->data_, p->next_->data_))
				{
					return false;
				}
			}
		}

		return true;
	}
	TListNode<T>* First()
	{
		return head_->next_;
	}
	bool IsLast(TListNode<T> *ptr_node)
	{
		return ptr_node->next_ == nullptr;
	}
	TListNode<T>* Find(const T &ref_data, TListNode<T>* start_node = nullptr, PtrFunc_Compare ptr_compare_func = nullptr)
	{
		TListNode<T> *node = (start_node == nullptr) ? head_->next_ : start_node;
		while (node != nullptr)
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
	TListNode<T>* Insert(const T &ref_data, PtrFunc_Compare ptr_compare_func = nullptr)
	{
		TListNode<T>* ret = nullptr;

		if (ptr_compare_func == nullptr)
		{
			ret = Add(ref_data, head_);
		}
		else
		{
			TListNode<T>* p;
			TListNode<T>* prev = head_;
			for (p = First(); p; p = p->next_)
			{
				if (!(*ptr_compare_func)(p->data_, ref_data))
				{
					ret = Add(ref_data, prev);
					break;
				}

				prev = p;
			}

			if (p == nullptr)
			{
				ret = Add(ref_data, prev);
			}
		}

		return ret;
	}
	TListNode<T>* Add(const T &ref_data, TListNode<T>* prev_node)
	{
#if ENABLE_DSAA_OPTIMIZATION
		TListNode<T> *node = new ((void*)MemoryPoolAlloc(pool_)) TListNode<T>(ref_data);
#else
		TListNode<T> *node = new TListNode<T>(ref_data);
#endif
		node->next_ = prev_node->next_;
		prev_node->next_ = node;
		++count_;

		return node;
	}
	bool FindAndRemove(const T &ref_data, PtrFunc_Compare ptr_compare_func = nullptr)
	{
		TListNode<T> **pp_node = &head_->next_;
		while (*pp_node != nullptr)
		{
			bool compare_result = false;
			if (ptr_compare_func)
			{
				compare_result = (*ptr_compare_func)((**pp_node).data_, ref_data);
			}
			else
			{
				compare_result = ((**pp_node).data_ == ref_data);
			}

			if (compare_result)
			{
				MASSERT(count_ != 0);
				TListNode<T> *p_node = *pp_node;
				*pp_node = (*pp_node)->next_;
#if ENABLE_DSAA_OPTIMIZATION
				p_node->~TListNode<T>();
				MemoryPoolFree(pool_, p_node);
#else
				delete(p_node);
#endif
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
	bool RemoveNext(TListNode<T>* node = nullptr)
	{
		if (node == nullptr)
		{
			node = head_;
		}

		TListNode<T> *next_node = node->next_;
		if (next_node)
		{
			node->next_ = next_node->next_;
#if ENABLE_DSAA_OPTIMIZATION
			next_node->~TListNode<T>();
			MemoryPoolFree(pool_, next_node);
#else
			delete(next_node);
#endif

			return true;
		}

		return false;
	}
	void MakeEmpty()
	{
		while (head_->next_ != nullptr)
		{
			TListNode<T>* node = head_->next_;
			head_->next_ = head_->next_->next_;
#if ENABLE_DSAA_OPTIMIZATION
			node->~TListNode<T>();
			MemoryPoolFree(pool_, node);
#else
			delete(node);
#endif
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
			head_->~TListNode<T>();
			MemoryPoolFree(pool_, head_);
#else
			delete(head_);
#endif
			head_ = nullptr;
			count_ = 0;
#if ENABLE_DSAA_OPTIMIZATION
			MemoryPoolDestroy(pool_);
			free(pool_);
			pool_ = nullptr;
#endif
		}
	}
	void copy(const TList<T> &ref)
	{
#if ENABLE_DSAA_OPTIMIZATION
		pool_ = (MemoryPool*)malloc(sizeof(MemoryPool));
		MemoryPoolInit(pool_, ref.pool_->used, sizeof(TListNode<T>));
		head_ = new ((void*)MemoryPoolAlloc(pool_)) TListNode<T>();
#else
		head_ = new TListNode<T>();
#endif
		head_->next_ = nullptr;
		count_ = ref.count_;

		TListNode<T>* node = ref.head_->next_;
		TListNode<T>* current_node = head_;
		while (node != nullptr)
		{
#if ENABLE_DSAA_OPTIMIZATION
			TListNode<T>* new_node = new ((void*)MemoryPoolAlloc(pool_)) TListNode<T>(node->data_);
#else
			TListNode<T>* new_node = new TListNode<T>(node->data_);
#endif
			current_node->next_ = new_node;
			current_node = new_node;
			current_node->next_ = nullptr;
			node = node->next_;
		}
	}

private:
	TListNode<T>*   head_;
	size_t          count_;

#if ENABLE_DSAA_OPTIMIZATION
	MemoryPool*     pool_;
#endif
};

#endif