#ifndef ZERG_LIST_H
#define ZERG_LIST_H


typedef struct vertex{
	int id;
	double lat;
	double lon;
	float alt;
	int edges;
	int remove;
	int visited;
	int from;
	struct vertex *next;
	struct edge* adj;
}vertex;

typedef struct edge{
	int id;
	double weight;
	int remove;
	int visited;
	struct edge* next;
}edge;

void adjAdjust(vertex *zergNode, int adjId, int vertId );
int removeSingle(vertex *zergNode);
void printRemovals(vertex *front);
int ListLength(vertex *front);
vertex *insertVertex(vertex *front, int id, double lon, double lat, float alt);
edge * insertEdge(edge *front, int id, double weight);
double haversine(double lat1, double lon1, double lat2, double lon2);
double pythagorean(float alt1, float alt2, double haversine);
void printAdj(vertex *zergNode);
void printList(vertex *front);
void destroy(vertex *front);

#endif
