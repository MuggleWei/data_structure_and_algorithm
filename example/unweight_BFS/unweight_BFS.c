/******************************************************************************
 *  @file         unweight_BFS.c
 *  @author       Muggle Wei
 *  @email        mugglewei@gmail.com
 *  @date         2021-04-23
 *  @copyright    Copyright 2021 Muggle Wei
 *  @license      MIT License
 *  @brief        example for unweighted graph BFS(breadth-first search)
 *****************************************************************************/

#include "dsaa/avl_tree_iter.h"
#include "dsaa/dsaa.h"
#include "dsaa/queue.h"

struct unweight_node
{
	int  id;       // node id
	bool known;    // node is already known
	int  distance; // distance from start node
	int  path;     // previous node in path
};

struct unweight_node* alloc_node()
{
	return (struct unweight_node*)malloc(sizeof(struct unweight_node));
}

int cmp_node(const void *v1, const void *v2)
{
	int *id1 = (int*)v1;
	int *id2 = (int*)v2;
	return *id1 - *id2;
}

void free_node(void *pool, void *data)
{
	free(data);
}

void add_vertex(struct gal *p_graph, int id)
{
	struct unweight_node *p_node = alloc_node();
	memset(p_node, 0, sizeof(*p_node));
	p_node->id = id;

	struct gal_vertex *p_vertex = gal_add_vertex(
		p_graph, &p_node->id, p_node);
	if (p_vertex == NULL)
	{
		printf("failed add node: %d\n", id);
		exit(EXIT_FAILURE);
	}
}

void add_edge(struct gal *p_graph, int id1, int id2)
{
	if (!gal_add_edge(p_graph, &id1, &id2, NULL))
	{
		printf("failed add edge %d -> %d\n", id1, id2);
		exit(EXIT_FAILURE);
	}
}

/**
 * @brief run breadth-first search from s
 *
 * @param p_graph  pointer to graph
 * @param s        start node key
 */
void unweight_BFS(struct gal *p_graph, int s)
{
	struct queue q;
	queue_init(&q, 0);

	struct gal_vertex *p_vertex = NULL;

	p_vertex = gal_find_vertex(p_graph, &s);
	if (!p_vertex)
	{
		printf("failed find vertex: %d\n", s);
		exit(EXIT_FAILURE);
	}
	struct unweight_node *node = (struct unweight_node*)p_vertex->value;
	node->known = true;
	node->distance = 0;
	node->path = s;

	queue_enqueue(&q, p_vertex);
	while (!queue_is_empty(&q))
	{
		struct queue_node *q_node = (struct queue_node*)queue_front(&q);
		p_vertex = (struct gal_vertex*)q_node->data;
		queue_dequeue(&q, NULL, NULL);

		struct unweight_node *cur_node =
			(struct unweight_node*)p_vertex->value;

		struct linked_list_node *list_node = linked_list_first(
			&p_vertex->out_degrees);
		while (list_node)
		{
			struct gal_edge *edge = (struct gal_edge*)list_node->data;
			struct gal_vertex *tmp_vertex = edge->v2;
			struct unweight_node *node =
				(struct unweight_node*)tmp_vertex->value;
			if (!node->known)
			{
				node->known = true;
				node->distance = cur_node->distance + 1;
				node->path = cur_node->id;
				queue_enqueue(&q, tmp_vertex);
				printf("find path %d -> %d(distance=%d)\n",
					cur_node->id, node->id, node->distance);
			}

			list_node = linked_list_next(&p_vertex->out_degrees, list_node);
		}
	}

	queue_destroy(&q, NULL, NULL);
}

/**
 * @brief output breadth-first search result
 *
 * @param p_graph  pointer to graph
 * @param s        start vertex id
 */
void output_BFS_result(struct gal *p_graph, int s)
{
	printf("--------------------\n");
	printf("unweighted BFS result:\n");
	struct avl_tree_node *it = NULL;
	for (it = avl_tree_iter_begin(&p_graph->verteices); it; it = avl_tree_iter_next(it))
	{
		struct gal_vertex *vertex = (struct gal_vertex*)it->value;
		struct unweight_node *p = (struct unweight_node*)vertex->value;
		if (p->known == false)
		{
			printf("%d -> %d, without path\n", s, p->id);
			continue;
		}
		printf("%d -> %d, distance=%d, path: ", s, p->id, p->distance);
		while (true)
		{
			p = (struct unweight_node*)vertex->value;
			int id = p->id;
			if (id == s)
			{
				printf("%d\n", s);
				break;
			}
			else
			{
				printf("%d <- ", id);
				vertex = gal_find_vertex(p_graph, &p->path);
			}
		}
	}
}

int main()
{
	struct gal graph;
	if (!gal_init(&graph, cmp_node, 0))
	{
		printf("failed gal_init\n");
		exit(EXIT_FAILURE);
	}

	for (int i = 1; i <= 7; i++)
	{
		add_vertex(&graph, i);
	}

	add_edge(&graph, 1, 2);
	add_edge(&graph, 1, 4);
	add_edge(&graph, 2, 4);
	add_edge(&graph, 2, 5);
	add_edge(&graph, 3, 1);
	add_edge(&graph, 3, 6);
	add_edge(&graph, 4, 3);
	add_edge(&graph, 4, 5);
	add_edge(&graph, 4, 6);
	add_edge(&graph, 4, 7);
	add_edge(&graph, 5, 7);
	add_edge(&graph, 7, 6);

	int start_id = 3;
	unweight_BFS(&graph, start_id);
	output_BFS_result(&graph, start_id);

	gal_destroy(&graph, NULL, NULL, free_node, NULL, NULL, NULL);

	return 0;
}
