#ifndef DIJKSTRA_H
#define DIJKSTRA_H


#include "zerg_list.h"

// The struct for my priority queue in Dijkstra's, it populates edge nodes from
// the adjacency list
struct queue{
	int size;
	int index;
	struct edge **next;
};



struct vertex *findPlace(struct vertex *zergNode, int id);
void populate(struct queue *priq, struct vertex* adja, int total);
edge * pop(struct queue *priq);
void obliviate(struct queue *priq);
struct queue *qInit(struct queue *priq, int vertex);
void insert(struct queue *priq, edge *adjacency, int from, int cost);

#endif
