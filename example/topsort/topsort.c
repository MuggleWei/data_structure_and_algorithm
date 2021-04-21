#include <stdio.h>
#include <stdlib.h>
#include "dsaa/dsaa.h"
#include "dsaa/linked_list.h"

/**
 * @brief allocate course
 *
 * @param main_id  course main id
 * @param sub_id   course sub id
 *
 * @return pointer to course
 */
char* alloc_course(int main_id, int sub_id)
{
	char *buf = (char*)malloc(16);
	snprintf(buf, sizeof(buf), "%d.%d", main_id, sub_id);
	return buf;
}

/**
 * @brief compare two course
 *
 * @param v1  course 1
 * @param v2  course 2
 *
 * @return
 *     1: v1 > v2
 *     0: v1 == v2
 *     -1: v1 < v2
 */
int course_cmp(const void *v1, const void *v2)
{
	return strcmp((const char*)v1, (const char*)v2);
}

/**
 * @brief free course
 *
 * @param pool  always NULL
 * @param data  pointer to course
 */
void free_course(void *pool, void *data)
{
	free(data);
}

/**
 * @brief add course into graph
 *
 * @param p_graph  pointer to graph
 * @param main_id  course main id
 * @param sub_id   couse sub id
 */
void add_course(struct gal *p_graph, int main_id, int sub_id)
{
	struct gal_vertex *p_vertex = gal_add_vertex(
			p_graph, alloc_course(main_id, sub_id), NULL);
	if (p_vertex == NULL)
	{
		printf("failed add course %d.%d\n", main_id, sub_id);
		exit(EXIT_FAILURE);
	}
}

/**
 * @brief add course dependency relationship
 *
 * @param p_graph  pointer to graph
 * @param c1_main  main id of previous course
 * @param c1_sub   sub id of previous course
 * @param c2_main  main id of post course
 * @param c2_sub   sub id of post course
 */
void add_course_depend(struct gal *p_graph,
	int c1_main, int c1_sub, int c2_main, int c2_sub)
{
	char c1[16], c2[16];
	snprintf(c1, sizeof(c1), "%d.%d", c1_main, c1_sub);
	snprintf(c2, sizeof(c2), "%d.%d", c2_main, c2_sub);
	if (!gal_add_edge(p_graph, c1, c2, NULL))
	{
		printf("failed add course dependency %s -> %s\n", c1, c2);
		exit(EXIT_FAILURE);
	}
}

/**
 * @brief topological sort
 *
 * @param p_graph  pointer to graph
 */
void top_sort(struct gal *p_graph)
{
	struct avl_tree_node *node = avl_tree_iter_begin(&p_graph->verteices);
	while (node)
	{
		struct gal_vertex *vertex = (struct gal_vertex*)node->value;
		uint32_t in_degree = linked_list_size(&vertex->in_degrees);
		if (in_degree == 0)
		{
			printf("%s | ", (char*)vertex->key);
			gal_remove_vertex(p_graph, vertex->key,
				free_course, NULL, NULL, NULL, NULL, NULL);
			node = avl_tree_iter_begin(&p_graph->verteices);
			continue;
		}
		else
		{
			node = avl_tree_iter_next(node);
		}
	}

	printf("\n");

	node = avl_tree_iter_begin(&p_graph->verteices);
	if (node)
	{
		printf("failed top sort, graph has a cycle\n");
		exit(EXIT_FAILURE);
	}
	else
	{
		printf("top sort completed\n");
	}
}

int main()
{
	struct gal graph;
	if (!gal_init(&graph, course_cmp, 0))
	{
		printf("failed gal_init\n");
		exit(EXIT_FAILURE);
	}

	char *course = NULL;

	add_course(&graph, 1, 1);
	add_course(&graph, 1, 2);
	add_course(&graph, 1, 3);
	add_course(&graph, 2, 1);
	add_course(&graph, 2, 2);
	add_course(&graph, 2, 3);

	add_course_depend(&graph, 1, 1, 1, 2);
	add_course_depend(&graph, 1, 1, 1, 3);
	add_course_depend(&graph, 1, 2, 2, 1);
	add_course_depend(&graph, 1, 3, 2, 1);
	add_course_depend(&graph, 1, 2, 2, 2);
	add_course_depend(&graph, 1, 3, 2, 2);
	add_course_depend(&graph, 2, 1, 2, 3);
	add_course_depend(&graph, 2, 2, 2, 3);

	top_sort(&graph);

	gal_destroy(&graph, free_course, NULL, NULL, NULL, NULL, NULL);

	return 0;
}
