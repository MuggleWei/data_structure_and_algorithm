#include "gtest/gtest.h"
#include "dsaa/dsaa.h"
#include "test_utils/test_utils.h"

#define TEST_GRAPH_ADJ_LIST_LEN 64

struct TestEdgeInfo
{
	const char *v1_key;
	const char *v2_key;
	const char *v1_value;
	const char *v2_value;
	int        weight;
};

static const char *cities[] = {
	"PEK",
	"SHA",
	"CAN",
	"SZX"
};

static const char *infos[] = {
	"bei jing",
	"shang hai",
	"guang zhou",
	"shen zhen",
};

static TestEdgeInfo s_edge_infos[] = {
	{ "PEK", "SHA", "bei jing", "shang hai", 1207 },
	{ "SHA", "SZX", "shang hai", "shen zhen", 1435 },
	{ "CAN", "SZX", "guang zhou", "shen zhen", 130 },
	{ "PEK", "CAN", "bei jing", "guang zhou", 2122 },
};

class GraphAdjListFixture : public ::testing::Test
{
public:
	void SetUp()
	{
		muggle_debug_memory_leak_start(&mem_state_);

		bool ret;

		ret = gal_init(&gal_, test_utils_cmp_str, TEST_GRAPH_ADJ_LIST_LEN);
		ASSERT_TRUE(ret);

		for (size_t i = 0; i < sizeof(cities) / sizeof(cities[0]); i++)
		{
			char *key = test_utils_.allocateString();
			memset(key, 0, TEST_UTILS_STR_SIZE);
			strncpy(key, cities[i], TEST_UTILS_STR_SIZE - 1);

			char *value = test_utils_.allocateString();
			memset(value, 0, TEST_UTILS_STR_SIZE);
			strncpy(value, infos[i], TEST_UTILS_STR_SIZE - 1);

			struct gal_vertex *vertex = gal_add_vertex(&gal_, key, value);
			ASSERT_TRUE(vertex != NULL);
			ASSERT_STREQ((char*)vertex->key, key);
			ASSERT_STREQ((char*)vertex->value, value);
		}

		for (size_t i = 0; i < sizeof(cities) / sizeof(cities[0]); i++)
		{
			struct gal_vertex *vertex = gal_add_vertex(&gal_, (void*)cities[i], (void*)infos[i]);
			ASSERT_TRUE(vertex == NULL);
		}

		for (size_t i = 0; i < sizeof(s_edge_infos) / sizeof(s_edge_infos[0]); i++)
		{
			int *p_distance = test_utils_.allocateInteger();
			*p_distance = s_edge_infos[i].weight;
			struct gal_edge *edge = gal_add_edge(&gal_, (void*)s_edge_infos[i].v1_key, (void*)s_edge_infos[i].v2_key, p_distance);
			ASSERT_TRUE(edge != NULL);
		}

		for (size_t i = 0; i < sizeof(s_edge_infos) / sizeof(s_edge_infos[0]); i++)
		{
			int distance = s_edge_infos[i].weight;
			struct gal_edge *edge = gal_add_edge(&gal_, (void*)s_edge_infos[i].v1_key, (void*)s_edge_infos[i].v2_key, &distance);
			ASSERT_FALSE(edge != NULL);
		}
	}

	void TearDown()
	{
		gal_destroy(&gal_,
			test_utils_free_str, &test_utils_,
			test_utils_free_str, &test_utils_,
			test_utils_free_int, &test_utils_);

		muggle_debug_memory_leak_end(&mem_state_);
	}
protected:
	struct gal gal_;

	TestUtils test_utils_;
	muggle_debug_memory_state mem_state_;
};

TEST_F(GraphAdjListFixture, find_vertex)
{
	struct gal_vertex *vertex = NULL;

	vertex = gal_find_vertex(&gal_, (void*)"PEK");
	ASSERT_TRUE(vertex != NULL);
	ASSERT_STREQ((const char*)vertex->value, "bei jing");

	vertex = gal_find_vertex(&gal_, (void*)"SHA");
	ASSERT_TRUE(vertex != NULL);
	ASSERT_STREQ((const char*)vertex->value, "shang hai");

	vertex = gal_find_vertex(&gal_, (void*)"not exists");
	ASSERT_TRUE(vertex == NULL);
}

TEST_F(GraphAdjListFixture, find_edge)
{
	struct gal_edge *edge = NULL;

	for (size_t i = 0; i < sizeof(s_edge_infos) / sizeof(s_edge_infos[0]); i++)
	{
		edge = gal_find_edge(&gal_, (void*)s_edge_infos[i].v1_key, (void*)s_edge_infos[i].v2_key);
		ASSERT_TRUE(edge != NULL);
		ASSERT_TRUE(edge->v1 != NULL);
		ASSERT_TRUE(edge->v2 != NULL);
		ASSERT_TRUE(edge->weight != NULL);
		ASSERT_STREQ((const char *)edge->v1->key, s_edge_infos[i].v1_key);
		ASSERT_STREQ((const char *)edge->v2->key, s_edge_infos[i].v2_key);
		ASSERT_STREQ((const char *)edge->v1->value, s_edge_infos[i].v1_value);
		ASSERT_STREQ((const char *)edge->v2->value, s_edge_infos[i].v2_value);
		ASSERT_EQ(*(int*)edge->weight, s_edge_infos[i].weight);
	}
}

TEST_F(GraphAdjListFixture, find_out_edge)
{
	struct linked_list_node *node = NULL;
	struct linked_list *list = NULL;
	struct gal_edge *edge = NULL;
	struct gal_vertex *vertex = NULL;

	list = gal_find_out_edge(&gal_, (void*)"PEK");
	ASSERT_TRUE(list != NULL);
	ASSERT_EQ((int)linked_list_size(list), 2);

	node = linked_list_first(list);
	ASSERT_TRUE(node != NULL);

	edge = (struct gal_edge*)node->data;
	ASSERT_TRUE(edge != NULL);

	vertex = (struct gal_vertex*)edge->v1;
	ASSERT_STREQ((const char *)vertex->key, "PEK");
	ASSERT_STREQ((const char *)vertex->value, "bei jing");

	vertex = (struct gal_vertex*)edge->v2;
	ASSERT_TRUE(vertex != NULL);
	if (strncmp((const char *)vertex->key, "SHA", TEST_UTILS_STR_SIZE) == 0)
	{
		ASSERT_STREQ((const char *)vertex->value, "shang hai");

		node = linked_list_next(list, node);
		ASSERT_TRUE(node != NULL);

		edge = (struct gal_edge*)node->data;
		ASSERT_TRUE(edge != NULL);

		vertex = (struct gal_vertex*)edge->v2;
		ASSERT_TRUE(vertex != NULL);
		ASSERT_STREQ((const char *)vertex->key, "CAN");
		ASSERT_STREQ((const char *)vertex->value, "guang zhou");
	}
	else if (strncmp((const char *)vertex->key, "CAN", TEST_UTILS_STR_SIZE) == 0)
	{
		ASSERT_STREQ((const char *)vertex->value, "guang zhou");

		node = linked_list_next(list, node);
		ASSERT_TRUE(node != NULL);

		edge = (struct gal_edge*)node->data;
		ASSERT_TRUE(edge != NULL);

		vertex = (struct gal_vertex*)edge->v2;
		ASSERT_TRUE(vertex != NULL);
		ASSERT_STREQ((const char *)vertex->key, "SHA");
		ASSERT_STREQ((const char *)vertex->value, "shang hai");
	}
	else
	{
		ASSERT_TRUE(false);
	}

	node = linked_list_next(list, node);
	ASSERT_TRUE(node == NULL);
}

TEST_F(GraphAdjListFixture, find_in_edge)
{
	struct linked_list_node *node = NULL;
	struct linked_list *list = NULL;
	struct gal_edge *edge = NULL;
	struct gal_vertex *vertex = NULL;

	list = gal_find_in_edge(&gal_, (void*)"SZX");
	ASSERT_TRUE(list != NULL);
	ASSERT_EQ((int)linked_list_size(list), 2);

	node = linked_list_first(list);
	ASSERT_TRUE(node != NULL);

	edge = (struct gal_edge*)node->data;
	ASSERT_TRUE(edge != NULL);

	vertex = (struct gal_vertex*)edge->v2;
	ASSERT_STREQ((const char *)vertex->key, "SZX");
	ASSERT_STREQ((const char *)vertex->value, "shen zhen");

	vertex = (struct gal_vertex*)edge->v1;
	ASSERT_TRUE(vertex != NULL);
	if (strncmp((const char *)vertex->key, "SHA", TEST_UTILS_STR_SIZE) == 0)
	{
		ASSERT_STREQ((const char *)vertex->value, "shang hai");

		node = linked_list_next(list, node);
		ASSERT_TRUE(node != NULL);

		edge = (struct gal_edge*)node->data;
		ASSERT_TRUE(edge != NULL);

		vertex = (struct gal_vertex*)edge->v1;
		ASSERT_TRUE(vertex != NULL);
		ASSERT_STREQ((const char *)vertex->key, "CAN");
		ASSERT_STREQ((const char *)vertex->value, "guang zhou");
	}
	else if (strncmp((const char *)vertex->key, "CAN", TEST_UTILS_STR_SIZE) == 0)
	{
		ASSERT_STREQ((const char *)vertex->value, "guang zhou");

		node = linked_list_next(list, node);
		ASSERT_TRUE(node != NULL);

		edge = (struct gal_edge*)node->data;
		ASSERT_TRUE(edge != NULL);

		vertex = (struct gal_vertex*)edge->v1;
		ASSERT_TRUE(vertex != NULL);
		ASSERT_STREQ((const char *)vertex->key, "SHA");
		ASSERT_STREQ((const char *)vertex->value, "shang hai");
	}
	else
	{
		ASSERT_TRUE(false);
	}

	node = linked_list_next(list, node);
	ASSERT_TRUE(node == NULL);
}

TEST_F(GraphAdjListFixture, remove_edge)
{
	struct gal_edge *edge = NULL;

	gal_remove_edge(&gal_, (void*)"PEK", (void*)"CAN", test_utils_free_int, &test_utils_);
	edge = gal_find_edge(&gal_, (void*)"PEK", (void*)"CAN");
	ASSERT_TRUE(edge == NULL);

	edge = gal_find_edge(&gal_, (void*)"PEK", (void*)"SHA");
	ASSERT_TRUE(edge != NULL);
}

TEST_F(GraphAdjListFixture, remove_vertex)
{
	struct gal_vertex *vertex = NULL;
	struct gal_edge *edge = NULL;

	gal_remove_vertex(&gal_, (void*)"PEK",
		test_utils_free_str, &test_utils_,
		test_utils_free_str, &test_utils_,
		test_utils_free_int, &test_utils_);

	vertex = gal_find_vertex(&gal_, (void*)"PEK");
	ASSERT_TRUE(vertex == NULL);

	edge = gal_find_edge(&gal_, (void*)"PEK", (void*)"CAN");
	ASSERT_TRUE(edge == NULL);

	edge = gal_find_edge(&gal_, (void*)"PEK", (void*)"SHA");
	ASSERT_TRUE(edge == NULL);
}
