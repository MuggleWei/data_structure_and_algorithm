#include <iostream>
#include <string>
#include <vector>
#include <list>
#include "timer.h"
#include "performance_test.h"
#include "run_test_tdouble_list.h"
#include "TDoubleList.h"

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

void PrintList(TDoubleList<TestData>& ref)
{
	for (auto node = ref.First(); node != ref.Last(); node = node->Next())
	{
		std::cout << node->Get()->num_ << " ";
	}
	std::cout << std::endl;
}

void TestListInsert(TDoubleList<TestData>& list, int num)
{
	for (int i = 0; i < num; ++i)
	{
		TestData t;
		list.Insert(t);
	}
}
void TestStdListInsert(std::list<TestData>& list, int num)
{
	for (int i = 0; i < num; ++i)
	{
		TestData t;
		list.insert(list.begin(), t);
	}
}

void TestListPerformance_Insert(PerformanceTest& test, int num)
{
	TDoubleList<TestData> list;
	PERFORMANCE_TEST_ADD(test, "cpp double list insert", num, TestListInsert(list, num));
}
void TestListPerformance_Insert_HintPoolSize(PerformanceTest& test, int num)
{
	TDoubleList<TestData> list(num);
	PERFORMANCE_TEST_ADD(test, "cpp double list insert(hint pool size)", num, TestListInsert(list, num));
}
void TestListPerformance_MakeEmpty(PerformanceTest& test, int num)
{
	TDoubleList<TestData> list;
	TestListInsert(list, num);
	PERFORMANCE_TEST_ADD(test, "cpp double list make empty", num, list.MakeEmpty());
}
void TestStdListPerformance_Insert(PerformanceTest& test, int num)
{
	std::list<TestData> list;
	PERFORMANCE_TEST_ADD(test, "std list insert(non pod)", num, TestStdListInsert(list, num));
}
void TestStdListPerformance_MakeEmpty(PerformanceTest& test, int num)
{
	std::list<TestData> list;
	for (int i = 0; i < num; ++i)
	{
		TestData t;
		list.insert(list.begin(), t);
	}
	PERFORMANCE_TEST_ADD(test, "std list make empty(non pod)", num, list.clear());
}

void TestDoubleListFunction()
{
	TDoubleList<TestData> list;

	// insert
	for (int i = 0; i < 8; ++i)
	{
		TestData data = { (int)i, "" };
		list.Insert(data);
	}
	PrintList(list);

	// find
	TestData data = { 5, "5" };
	TDoubleListNode<TestData> *node = list.Find(data);
	if (node)
	{
		std::cout << "find {" << node->Get()->num_ << ", " << node->Get()->str_ << "}" << std::endl;
	}

	// insert node
	TestData insert_node = { 10, "10" };
	list.Insert(insert_node, node);
	PrintList(list);

	// add node
	TestData add_node = { 11, "11" };
	list.Add(add_node, node);
	PrintList(list);

	// remove
	list.Remove(node);
	PrintList(list);

	// copy
	TDoubleList<TestData> copy_list(list);
	PrintList(copy_list);
	copy_list = list;
	PrintList(copy_list);

	// move
	TDoubleList<TestData> swap_list(std::move(copy_list));
	PrintList(swap_list);
	swap_list = std::move(list);
	PrintList(swap_list);
}
void TestDoubleListPerformance()
{
	PerformanceTest test;
	for (int i = 1; i <= 32; ++i)
	{
		int num = i * 102400;
		TestListPerformance_Insert(test, num);
		TestListPerformance_Insert_HintPoolSize(test, num);
		TestListPerformance_MakeEmpty(test, num);
		TestStdListPerformance_Insert(test, num);
		TestStdListPerformance_MakeEmpty(test, num);
	}
	test.WriteCompareToFile("cpp double list performance.txt");
}

int main()
{
#if MG_PLATFORM_WINDOWS && ! defined(NDEBUG)
	_CrtMemState s1, s2, s3;
	_CrtMemCheckpoint(&s1);
#endif

	TestDoubleListFunction();
	TestDoubleListPerformance();

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