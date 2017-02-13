#ifndef ZERG_LIST_H
#define ZERG_LIST_H

#include <stdio.h>
#include <stdlib.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <arpa/inet.h>

typedef struct vertex{
	int id;
	uint64_t lat;
	uint64_t lon;
	struct vertex *next;
	struct edge* adj;
}vertex;

typedef struct edge{
	int id;
	int weight;
	struct edge* next;
}edge;

vertex *insertVertex(vertex *front, int id, uint64_t lon, uint64_t lat);
edge * insertEdge(edge *front, int id, int weight);

#endif
