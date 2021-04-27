/******************************************************************************
 *  @file         dijkstra.c
 *  @author       Muggle Wei
 *  @email        mugglewei@gmail.com
 *  @date         2021-04-23
 *  @copyright    Copyright 2021 Muggle Wei
 *  @license      MIT License
 *  @brief        example of Dijkstra's algorithm
 *****************************************************************************/
 
#include "dsaa/dsaa.h"
#include "dsaa/graph_adj_list.h"
#include "dsaa/heap.h"

struct weight_node
{
	int  id;       // node id
	bool known;    // node is already known
	int  distance; // distance from start node
	int  path;     // previous node in path
};

struct weight
{
	int distance;
};

struct weight_node* alloc_node()
{
	return (struct weight_node*)malloc(sizeof(struct weight_node));
}

int cmp_node(const void *v1, const void *v2)
{
	int *id1 = (int*)v1;
	int *id2 = (int*)v2;
	return *id1 - *id2;
}

int heap_cmp_node(const void *v1, const void *v2)
{
	int *id1 = (int*)v1;
	int *id2 = (int*)v2;
	return *id2 - *id1;  // NOTE: in this repo, heap is max-heap
}

void free_node(void *pool, void *data)
{
	free(data);
}

struct weight* alloc_weight(int distance)
{
	struct weight *weight = (struct weight*)malloc(sizeof(struct weight));
	weight->distance = distance;
	return weight;
}

void free_weight(void *pool, void *data)
{
	free(data);
}

void add_vertex(struct gal *p_graph, int id)
{
	struct weight_node *p_node = alloc_node();
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

void add_edge(struct gal *p_graph, int id1, int id2, int distance)
{
	if (distance < 0)
	{
		printf("negative distance is invalid\n");
		exit(EXIT_FAILURE);
	}

	struct weight *weight = alloc_weight(distance);
	if (!gal_add_edge(p_graph, &id1, &id2, weight))
	{
		printf("failed add edge %d -> %d\n", id1, id2);
		exit(EXIT_FAILURE);
	}
}

void dijkstra(struct gal *p_graph, int s)
{
	struct heap h;
	heap_init(&h, heap_cmp_node, 0);

	struct gal_vertex *p_vertex = NULL;

	p_vertex = gal_find_vertex(p_graph, &s);
	if (!p_vertex)
	{
		printf("failed find vertex: %d\n", s);
		exit(EXIT_FAILURE);
	}
	struct weight_node *node = (struct weight_node*)p_vertex->value;
	node->known = true;
	node->distance = 0;
	node->path = s;

	struct heap_node h_node;
	heap_insert(&h, &node->distance, p_vertex);
	while (!heap_is_empty(&h))
	{
		heap_extract(&h, &h_node);
		p_vertex = (struct gal_vertex*)h_node.value;
		struct weight_node *cur_node = (struct weight_node*)p_vertex->value;

		struct linked_list_node *list_node = linked_list_first(
			&p_vertex->out_degrees);
		while (list_node)
		{
			struct gal_edge *edge = (struct gal_edge*)list_node->data;
			struct gal_vertex *tmp_vertex = edge->v2;
			struct weight_node *node =
				(struct weight_node*)tmp_vertex->value;
			int distance = ((struct weight*)edge->weight)->distance;
			if (!node->known)
			{
				node->known = true;
				node->distance = cur_node->distance + distance;
				node->path = cur_node->id;
				heap_insert(&h, &node->distance, tmp_vertex);
				printf("new path %d -> %d(distance=%d)\n",
					cur_node->id, node->id, node->distance);
			}
			else if (node->distance > cur_node->distance + distance)
			{
				printf("update path %d -> %d(distance=%d), "
					"old path %d -> %d(distance=%d)\n",
					cur_node->id, node->id, cur_node->distance + distance,
					node->path, node->id, node->distance);
				node->known = true;
				node->distance = cur_node->distance + distance;
				node->path = cur_node->id;
				heap_insert(&h, &node->distance, tmp_vertex);
			}
			else
			{
				printf("ignore path %d -> %d(distance=%d), "
					"already exists path %d -> %d(distance=%d)\n",
					cur_node->id, node->id, cur_node->distance + distance,
					node->path, node->id, node->distance);
			}

			list_node = linked_list_next(&p_vertex->out_degrees, list_node);
		}
	}

	heap_destroy(&h, NULL, NULL, NULL, NULL);
}

/**
 * @brief output Dijkstra's algorithm result
 *
 * @param p_graph  pointer to graph
 * @param s        start vertex id
 */
void output_dijkstra_result(struct gal *p_graph, int s)
{
	printf("--------------------\n");
	printf("weighted Dijkstra's algorithm result:\n");
	struct avl_tree_node *it = NULL;
	for (it = avl_tree_iter_begin(&p_graph->verteices); it; it = avl_tree_iter_next(it))
	{
		struct gal_vertex *vertex = (struct gal_vertex*)it->value;
		struct weight_node *p = (struct weight_node*)vertex->value;
		if (p->known == false)
		{
			printf("%d -> %d, without path\n", s, p->id);
			continue;
		}
		printf("%d -> %d, distance=%d, path: ", s, p->id, p->distance);
		while (true)
		{
			p = (struct weight_node*)vertex->value;
			int id = p->id;
			if (id == s)
			{
				printf("%d\n", s);
				break;
			}
			else
			{
				struct gal_edge *edge = gal_find_edge(p_graph, &p->path, &p->id);
				struct weight *weight = (struct weight*)edge->weight;
				printf("%d <--(%d)-- ", id, weight->distance);
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

	add_edge(&graph, 1, 2, 2);
	add_edge(&graph, 1, 4, 1);
	add_edge(&graph, 2, 4, 3);
	add_edge(&graph, 2, 5, 10);
	add_edge(&graph, 3, 1, 4);
	add_edge(&graph, 3, 6, 5);
	add_edge(&graph, 4, 3, 2);
	add_edge(&graph, 4, 5, 2);
	add_edge(&graph, 4, 6, 8);
	add_edge(&graph, 4, 7, 4);
	add_edge(&graph, 5, 7, 6);
	add_edge(&graph, 7, 6, 1);

	int start_id = 1;
	dijkstra(&graph, start_id);
	output_dijkstra_result(&graph, start_id);

	gal_destroy(&graph, NULL, NULL, free_node, NULL, free_weight, NULL);

	return 0;
}
