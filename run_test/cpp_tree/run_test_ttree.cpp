#include <iostream>
#include <string>
#include <vector>
#include <list>
// #include "timer.h"
#include "run_test_ttree.h"
#include "TTree.h"
#include "base.h"

class TestData
{
public:
    bool operator==(const TestData &ref) const
    {
        return num_ == ref.num_;
    }
    ~TestData()
    {
        num_ = -1;
    }

public:
    int         num_;
    std::string str_;
};

void PrintTreeNode(const TestData& data_ref, int level)
{
    for (int i = 0; i < level; ++i)
    {
        if (i == level - 1)
        {
            std::cout << "|---";
        }
        else
        {
            std::cout << "|   ";
        }
    }
    std::cout << data_ref.str_ << std::endl;
}

void TestTNodeFunction(TTreeNode<TestData>* root)
{
    std::cout << std::endl << "======= Test TTreeNode =======" << std::endl;

    // copy create
    TTreeNode<TestData>* copy_root = TTreeNode<TestData>::Create(*root);

    // print
    std::cout << "preorder:" << std::endl;
    root->PreorderTraversal(0, PrintTreeNode);

    // print
    std::cout << std::endl << "preorder:" << std::endl;
    root->PostorderTraversal(0, PrintTreeNode);

    // find
    TTreeNode<TestData>* node = root->Find(*root->FirstChild()->Get(), true);

    // remove
    root->RemoveChild(node);

    // print
    std::cout << "preorder:" << std::endl;
    root->PreorderTraversal(0, PrintTreeNode);

    // print copy
    std::cout << "copy root preorder:" << std::endl;
    copy_root->PreorderTraversal(0, PrintTreeNode);

    TTreeNode<TestData>::Destroy(root);
    TTreeNode<TestData>::Destroy(copy_root);
}
void TestTTreeFunction(TTreeNode<TestData>* root)
{
    std::cout << std::endl << "======= Test TTree =======" << std::endl;

    TTree<TestData> tree(*root);

    // copy
    TTree<TestData> tree_copy(tree);
    tree_copy.GetRoot()->PreorderTraversal(0, PrintTreeNode);
    tree_copy = tree;
    tree_copy.GetRoot()->PreorderTraversal(0, PrintTreeNode);

    // move
    TTree<TestData> tree_swap(std::move(tree_copy));
    MASSERT(tree_copy.GetRoot() == nullptr);
    tree_swap.GetRoot()->PreorderTraversal(0, PrintTreeNode);
    tree_swap = std::move(tree);
    MASSERT(tree.GetRoot() == nullptr);
    tree_swap.GetRoot()->PreorderTraversal(0, PrintTreeNode);
}

void TestTreeFunction()
{
    TestData data[] = {
        { 0, "/" },
        { 1, "home" },
        { 2, "usr" },
        { 3, "var" },
        { 4, "user" },
        { 5, "muggle" },
        { 6, "bin" },
        { 7, "include" },
        { 8, "local" },
        { 9, "lib" },
        { 10, "sbin" },
        { 11, "src" },
    };

    // init
    TTreeNode<TestData>* root = TTreeNode<TestData>::Create(data[0]);
    TTreeNode<TestData>* home = root->AddChild(TTreeNode<TestData>::Create(data[1]));
    TTreeNode<TestData>* usr = root->AddChild(TTreeNode<TestData>::Create(data[2]));
    root->AddChild(TTreeNode<TestData>::Create(data[3]));
    home->AddChild(TTreeNode<TestData>::Create(data[4]));
    home->AddChild(TTreeNode<TestData>::Create(data[5]));
    usr->AddChild(TTreeNode<TestData>::Create(data[6]));
    usr->AddChild(TTreeNode<TestData>::Create(data[7]));
    TTreeNode<TestData>* local = usr->AddChild(TTreeNode<TestData>::Create(data[8]));
    local->AddChild(TTreeNode<TestData>::Create(data[9]));
    local->AddChild(TTreeNode<TestData>::Create(data[10]));
    local->AddChild(TTreeNode<TestData>::Create(data[11]));

    TTreeNode<TestData>* copy_root = TTreeNode<TestData>::Create(*root);

    TestTNodeFunction(root);
    TestTTreeFunction(copy_root);
}
void TestTreePerformance()
{}

int main()
{
#if MG_PLATFORM_WINDOWS && ! defined(NDEBUG)
    _CrtMemState s1, s2, s3;
    _CrtMemCheckpoint(&s1);
#endif

    TestTreeFunction();
    TestTreePerformance();

#if MG_PLATFORM_WINDOWS && ! defined(NDEBUG)
    _CrtMemCheckpoint(&s2);
    if (_CrtMemDifference(&s3, &s1, &s2))
    {
        _CrtMemDumpStatistics(&s3);
    }
    _CrtDumpMemoryLeaks();
#endif

    return 0;
}