/*
 *  reads filename and inserts its contents in place of the include statement
 */
#include "process_file.h"

void PrintDoubleList(DoubleList *p_list)
{
	// open file
	FILE *fp = fopen("main.c.cache", "w+");
	if (!fp)
	{
		MLOG("can't open file main.c.cache!\n");
		MASSERT(0);
		return;
	}

	DoubleListNode *p_node = DoubleListFirst(p_list);
	while (p_node)
	{
		char **p_data = (char**)GET_DOUBLE_LIST_NODE_DATA_ADDRESS(*p_node);
		MLOG("%s\n", *p_data);
		fwrite(*p_data, 1, strlen(*p_data), fp);
		fwrite("\n", 1, 1, fp);
		p_node = DoubleListNext(p_list, p_node);
	}
	MLOG("\n");

	fclose(fp);
}

int main()
{
#if MG_PLATFORM_WINDOWS && ! defined(NDEBUG)
	_CrtMemState s1, s2, s3;
	_CrtMemCheckpoint(&s1);
#endif

	DoubleList lines;
	DoubleList binarys;
	Array file_stack, file_records;
	size_t index;

	DoubleListInit(&lines, sizeof(char*), 8);
	DoubleListInit(&binarys, sizeof(char*), 256);
	ArrayInit(&file_stack, 8, sizeof(char*));
	ArrayInit(&file_records, 8, sizeof(char*));

	process_file("res/main.c", false, &file_stack, &file_records, &lines, &binarys);

	// output result
	PrintDoubleList(&lines);

	// free file stack
	MASSERT(ArrayIsEmpty(&file_stack));
	if (!ArrayIsEmpty(&file_stack))
	{
		for (index = 0; index < ArrayUsed(&file_stack); ++index)
		{
			char** p_address = (char**)ArrayGet(&file_stack, index);
			free(*p_address);
		}
		MLOG("It's something wrong!");
	}

	// free file records
	for (index = 0; index < ArrayUsed(&file_records); ++index)
	{
		char** p_address = (char**)ArrayGet(&file_records, index);
		free(*p_address);
	}

	// free binarys
	DoubleListNode* p_node = DoubleListFirst(&binarys);
	while (p_node)
	{
		char** p_data = (char**)GET_DOUBLE_LIST_NODE_DATA_ADDRESS(*p_node);
		free(*p_data);
		p_node = DoubleListNext(&binarys, p_node);
	}

	ArrayDestroy(&file_stack);
	ArrayDestroy(&file_records);
	DoubleListDestroy(&lines);
	DoubleListDestroy(&binarys);

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