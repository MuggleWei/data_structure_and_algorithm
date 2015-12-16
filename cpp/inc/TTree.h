#ifndef __T_TREE_H__
#define __T_TREE_H__

#include <stdlib.h>
#include <stddef.h>
#include <assert.h>
#include <new>
#include <functional>
#include <macros.h>

template<typename T>
class TTree;

template<typename T>
class TTreeNode
{
public:
    // Purpose of use Create function is ensure all of nodes is generate by new TTreeNode<T>
    static TTreeNode<T>* Create()
    {
        return new TTreeNode<T>();
    }
    static TTreeNode<T>* Create(const T &data_ref)
    {
        return new TTreeNode<T>(data_ref);
    }
    static TTreeNode<T>* Create(const TTreeNode<T> &ref)
    {
        return new TTreeNode<T>(ref);
    }
    static void Destroy(TTreeNode<T>* node)
    {
        // ensure node is separated
        assert(node->parent_ == nullptr);
        assert(node->next_sibling_ == nullptr);
        assert(node->prev_sibling_ == nullptr);

        TTreeNode<T> *child = nullptr;
        while (node->first_child_)
        {
            child = node->first_child_;
            child->Separate();
            Destroy(child);
        }

        // note: assume all of nodes is generate by "new TTreeNode<T>".
        delete(node);
    }

    TTreeNode* Parent()
    {
        return parent_;
    }
    TTreeNode* FirstChild()
    {
        return first_child_;
    }
    TTreeNode* NextSibling()
    {
        return next_sibling_;
    }
    TTreeNode* PrevSibling()
    {
        return prev_sibling_;
    }

    T* Get()
    {
        return &data_;
    }
    void Set(const T &data_ref)
    {
        data_ = data_ref;
    }

    TTreeNode<T>* AddChild(TTreeNode<T> *new_node)
    {
        if (first_child_)
        {
            TTreeNode<T>* node = first_child_;
            while (node->next_sibling_)
            {
                node = node->next_sibling_;
            }

            node->next_sibling_ = new_node;
            new_node->prev_sibling_ = node;
            new_node->parent_ = this;
        }
        else
        {
            first_child_ = new_node;
            new_node->parent_ = this;
        }

        return new_node;
    }
    void RemoveChild(TTreeNode<T>* node)
    {
        assert(node->parent_ = this);
        node->Separate();
        Destroy(node);
    }
    TTreeNode<T>* Find(const T &data_ref, bool recursive)
    {
        TTreeNode<T> *child = first_child_;
        while (child)
        {
            if (child->data_ == data_ref)
            {
                return child;
            }
            child = child->next_sibling_;
        }

        if (recursive)
        {
            child = first_child_;
            TTreeNode<T>* ret_node = nullptr;
            while (child)
            {
                ret_node = child->Find(data_ref, true);
                if (ret_node)
                {
                    return ret_node;
                }
                child = child->next_sibling_;
            }
        }

        return nullptr;
    }

    void Separate()
    {
        if (parent_)
        {
            if (parent_->first_child_ == this)
            {
                parent_->first_child_ = next_sibling_;
                if (next_sibling_)
                {
                    next_sibling_->prev_sibling_ = NULL;
                }
            }
            else
            {
                prev_sibling_->next_sibling_ = next_sibling_;
                if (next_sibling_)
                {
                    next_sibling_->prev_sibling_ = prev_sibling_;
                }
            }

            parent_ = nullptr;
            prev_sibling_ = nullptr;
            next_sibling_ = nullptr;
        }
    }

    void PreorderTraversal(int level, std::function<void(const T& data, int)> func)
    {
        func(data_, level);
        TTreeNode<T> *child = first_child_;
        while (child)
        {
            child->PreorderTraversal(level + 1, func);
            child = child->next_sibling_;
        }
    }
    void PostorderTraversal(int level, std::function<void(const T& data, int)> func)
    {
        TTreeNode<T> *child = first_child_;
        while (child)
        {
            child->PostorderTraversal(level + 1, func);
            child = child->next_sibling_;
        }
        func(data_, level);
    }

private:
    TTreeNode()
    {
        parent_ = nullptr;
        first_child_ = nullptr;
        next_sibling_ = nullptr;
        prev_sibling_ = nullptr;
    }
    explicit TTreeNode(const T &data_ref)
        : data_(data_ref)
    {
        parent_ = nullptr;
        first_child_ = nullptr;
        next_sibling_ = nullptr;
        prev_sibling_ = nullptr;
    }
    TTreeNode(const TTreeNode<T> &ref)
    {
        copy(ref);
    }
    TTreeNode& operator=(const TTreeNode<T> &ref) = delete;

    void copy(const TTreeNode<T> &ref)
    {
        parent_ = nullptr;
        first_child_ = nullptr;
        next_sibling_ = nullptr;
        prev_sibling_ = nullptr;

        data_ = ref.data_;
        TTreeNode<T>* node = ref.first_child_;
        while (node)
        {
            TTreeNode<T>* new_node = new TTreeNode<T>(*node);
            this->AddChild(new_node);
            node = node->next_sibling_;
        }
    }

private:
    TTreeNode*  parent_;
    TTreeNode*  first_child_;
    TTreeNode*  next_sibling_;
    TTreeNode*  prev_sibling_;
    T           data_;

    friend class TTree<T>;
};

template<typename T>
class TTree
{
public:
    TTree()
    {
        root_ = nullptr;
    }
    explicit TTree(const T &data_ref)
    {
        root_ = TTreeNode<T>::Create(data_ref);
    }
    explicit TTree(TTreeNode<T> &node_ref)
    {
        assert(node_ref.parent_ == nullptr);
        assert(node_ref.next_sibling_ == nullptr);
        assert(node_ref.prev_sibling_ == nullptr);

        root_ = &node_ref;
    }
    TTree(const TTree<T> &ref)
    {
        copy(ref);
    }
    TTree(TTree<T> &&rref) noexcept
        : root_(rref.root_)
    {
        rref.root_ = nullptr;
    }
    ~TTree()
    {
        deallocate();
    }

    TTree<T>& operator=(const TTree<T> &ref)
    {
        if (this != &ref)
        {
            deallocate();
            copy(ref);
        }
        return *this;
    }
    TTree<T>& operator=(TTree<T> &&rref) noexcept
    {
        if (this != &rref)
        {
            deallocate();
            
            root_ = rref.root_;
            rref.root_ = nullptr;
        }
        return *this;
    }

    void SetRoot(const T &data_ref)
    {
        assert(root_ == nullptr);
        root_ = TTreeNode<T>::Create(data_ref);
    }
    TTreeNode<T>* GetRoot()
    {
        return root_;
    }

private:
    void deallocate()
    {
        if (root_ != nullptr)
        {
            TTreeNode<T>::Destroy(root_);
            root_ = nullptr;
        }
    }
    void copy(const TTree<T> &ref)
    {
        root_ = TTreeNode<T>::Create(*ref.root_);
    }

    TTreeNode<T>*  root_;
};

#endif