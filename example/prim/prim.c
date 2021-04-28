/******************************************************************************
 *  @file         prim.c
 *  @author       Muggle Wei
 *  @email        mugglewei@gmail.com
 *  @date         2021-04-27
 *  @copyright    Copyright 2021 Muggle Wei
 *  @license      MIT License
 *  @brief        Prim's algorithm example
 *****************************************************************************/
 
#include "dsaa/dsaa.h"
#include "dsaa/linked_list.h"

struct prim_node
{
	int id;
	bool already_picked; // already picked into tree
};

struct prim_weight
{
	int weight;
	bool already_picked; // already picked into tree
};

struct prim_node* alloc_node()
{
	return (struct prim_node*)malloc(sizeof(struct prim_node));
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

struct prim_weight* alloc_weight(int weight)
{
	struct prim_weight *p = (struct prim_weight*)malloc(sizeof(struct prim_weight));
	p->weight = weight;
	p->already_picked = false;
	return p;
}

int edge_cmp(const void *v1, const void *v2)
{
	int *id1 = (int*)v1;
	int *id2 = (int*)v2;
	return *id2 - *id1;  // NOTE: in this repo, heap is max-heap
}

void free_weight(void *pool, void *data)
{
	free(data);
}

void add_vertex(struct gal *p_graph, int id)
{
	struct prim_node *p_node = alloc_node();
	memset(p_node, 0, sizeof(*p_node));
	p_node->id = id;
	p_node->already_picked = false;

	struct gal_vertex *p_vertex = gal_add_vertex(
		p_graph, &p_node->id, p_node);
	if (p_vertex == NULL)
	{
		printf("failed add node: %d\n", id);
		exit(EXIT_FAILURE);
	}
}

void add_edge(struct gal *p_graph, int id1, int id2, int weight)
{
	if (!gal_add_edge(p_graph, &id1, &id2, alloc_weight(weight)))
	{
		printf("failed add edge %d -> %d\n", id1, id2);
		exit(EXIT_FAILURE);
	}

	if (!gal_add_edge(p_graph, &id2, &id1, alloc_weight(weight)))
	{
		printf("failed add edge %d -> %d\n", id2, id1);
		exit(EXIT_FAILURE);
	}
}

void put_edge_into_heap(struct heap *p_heap, struct gal_vertex *p_vertex)
{
	struct linked_list_node *list_node =
		linked_list_first(&p_vertex->out_degrees);

	while (list_node)
	{
		struct gal_edge *edge = (struct gal_edge*)list_node->data;
		struct prim_weight *weight = (struct prim_weight*)edge->weight;
		if (weight->already_picked)
		{
			list_node = linked_list_next(&p_vertex->out_degrees, list_node);
			continue;
		}

		struct gal_vertex *v1 = (struct gal_vertex*)edge->v1;
		struct gal_vertex *v2 = (struct gal_vertex*)edge->v2;
		struct prim_node *n1 = (struct prim_node*)v1->value;
		struct prim_node *n2 = (struct prim_node*)v2->value;
		if (n1->already_picked && n2->already_picked)
		{
			list_node = linked_list_next(&p_vertex->out_degrees, list_node);
			continue;
		}

		// printf("insert %d(%s) <-> %d(%s)\n",
		//     n1->id, n1->already_picked ? "true" : "false",
		//     n2->id, n2->already_picked ? "true" : "false");
		heap_insert(p_heap, &weight->weight, edge);

		list_node = linked_list_next(&p_vertex->out_degrees, list_node);
	}
}

void prim(struct gal *p_graph, struct linked_list *p_list, int s)
{
	struct heap h;
	heap_init(&h, edge_cmp, 0);

	struct gal_vertex *p_vertex = NULL;

	p_vertex = gal_find_vertex(p_graph, &s);
	if (!p_vertex)
	{
		printf("failed find vertex: %d\n", s);
		exit(EXIT_FAILURE);
	}

	struct prim_node *node = (struct prim_node*)p_vertex->value;
	node->already_picked = true;
	put_edge_into_heap(&h, p_vertex);

	struct heap_node h_node;
	while (!heap_is_empty(&h))
	{
		heap_extract(&h, &h_node);
		struct gal_edge *p_edge = (struct gal_edge*)h_node.value;

		struct prim_weight *weight = (struct prim_weight*)p_edge->weight;
		struct gal_vertex *v1 = (struct gal_vertex*)p_edge->v1;
		struct gal_vertex *v2 = (struct gal_vertex*)p_edge->v2;
		struct prim_node *n1 = (struct prim_node*)v1->value;
		struct prim_node *n2 = (struct prim_node*)v2->value;
		if (n1->already_picked && n2->already_picked)
		{
			continue;
		}

		linked_list_append(p_list, NULL, p_edge);
		printf("pick edge: %d <--%d--> %d\n", n1->id, weight->weight, n2->id);

		p_vertex = (struct gal_vertex*)p_edge->v2;
		node = (struct prim_node*)p_vertex->value;
		node->already_picked = true;
		put_edge_into_heap(&h, p_vertex);
	}

	heap_destroy(&h, NULL, NULL, NULL, NULL);
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
	add_edge(&graph, 1, 3, 4);
	add_edge(&graph, 1, 4, 1);
	add_edge(&graph, 2, 4, 3);
	add_edge(&graph, 2, 5, 10);
	add_edge(&graph, 3, 4, 2);
	add_edge(&graph, 3, 6, 5);
	add_edge(&graph, 4, 5, 7);
	add_edge(&graph, 4, 6, 8);
	add_edge(&graph, 4, 7, 4);
	add_edge(&graph, 5, 7, 6);
	add_edge(&graph, 6, 7, 1);

	struct linked_list list;
	linked_list_init(&list, 0);

	prim(&graph, &list, 1);

	linked_list_destroy(&list, NULL, NULL);

	gal_destroy(&graph, NULL, NULL, free_node, NULL, free_weight, NULL);

	return 0;
}
