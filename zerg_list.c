#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <arpa/inet.h>
#define _GNU_SOURCE
#include <sysexits.h>
#include <sys/stat.h>
#include <ctype.h>
#include <math.h>

#include "zerg_list.h"


//All of these functions were taken from the Linked List section of DSAI
vertex *insertVertex(vertex *front, int id, double lon, double lat, float alt){

	
		vertex *insert=(vertex*)malloc(sizeof(vertex));
		if(!insert)
		{
			printf("Memory Error\n");
			return NULL;
		}

		// insert data into new node
		insert->id=id;
		insert->lat=lat;
		insert->lon=lon;
		insert->alt = alt;
		insert->next=NULL;
		insert->adj = NULL;
		insert->total = 0;
		insert->edges = 0;
		insert->visited = 0;
		insert->remove = 0;
		insert->from = 0;
		insert->next=front;
		//front=insert; // notice how the newest code is considered front now
	// connect to list

	return insert;
}

edge * insertEdge(edge *front, int id, double weight, vertex *parent){

	// create new node
	edge *insert=(edge *)malloc(sizeof(edge));
	if(!insert)
	{
		printf("Memory Error\n");
		return NULL;
	}

	// insert data into new node
	insert->id=id;
	insert->weight = weight;
	insert->remove = 0;
	insert->visited = 0;
	insert->total = 0;
	insert->from = 0;
	insert->next=NULL;
	insert->parent = parent;

	insert->next=front;
	//front=insert; // notice how the newest code is considered front now
	return insert;
}

	
void destroy(vertex *front)
{
	

	vertex *cursor;
	cursor=front;
	edge *fuck;
	edge *temp;
	

	while(cursor!=NULL)
	{
		fuck = cursor->adj;

		while(fuck)
		{
			temp = fuck;
			fuck= fuck->next;

			free(temp);
		}

		front=cursor;
		cursor=front->next;

		free(front);
	}
}

// debugging function for printing the contents of zergNodes
void printList(vertex *front)
{
	vertex *cursor;
	cursor = front;
	while(cursor != NULL)
	{
		printf("id -> %d\n", cursor ->id);
		printf("lat -> %lf\n", cursor->lat);
		printf("lon -> %lf\n", cursor->lon);
		printf("alt -> %f\n", cursor ->alt);
		printf("edges ->%d\n", cursor->edges);
		cursor = cursor->next;
		puts(" ");
	}
}

//http://rosettacode.org/wiki/Haversine_formula#C
double haversine(double lat1, double lon1, double lat2, double lon2)
{
	double R = 6371;
	double TO_RAD = (3.1415926536 / 180);
	double dx, dy, dz;
	lon1 -= lon2;
	lon1 *= TO_RAD;
	lat1 *= TO_RAD; 
	lat2 *= TO_RAD;
 
	dz = sin(lat1) - sin(lat2);
	dx = cos(lon1) * cos(lat1) - cos(lat2);
	dy = sin(lon1) * cos(lat1);
	return (asin(sqrt(dx * dx + dy * dy + dz * dz) / 2) * 2 * R) * 1000;
}

double pythagorean(float alt1, float alt2, double haversine)
{
	double aSqr, bSqr, cSqr;
	aSqr = pow((alt1+alt2),2);
	bSqr = pow(haversine, 2);
	cSqr = aSqr+bSqr;
	return sqrt(cSqr);
}

//Debugging function used to  print the contents of adjacency nodes. 
void printAdj(vertex *zergNode)
{
	vertex *cursor;
	cursor = zergNode;
	edge *edge ;
	while(cursor != NULL)
	{	
		edge = cursor->adj;
		printf("cursor id-> %d\n", cursor->id);
		printf("cursor edges->%d\n", cursor->edges);
		while(edge !=NULL)
		{
			printf("adj id -> %d\n", edge->id);
			printf("adj haves ->%d\n", (int)edge->weight);
			printf("adj->parent->edges->%d\n", edge->parent->edges);
			edge = edge -> next;
		}
		cursor = cursor->next;

		puts(" ");
	}
}

int ListLength(vertex *front){
	int count = 0;
	vertex *cursor=front;
	while(cursor!=NULL)
	{
		count++;
		cursor=cursor->next;
	}
	return count;
}

int removeSingle(vertex *zergNode)
{
	vertex *cursor = zergNode;
	int removals = 0;
	if(cursor == NULL)
	{
		return removals;
	}
	removals += removeSingle(cursor->next);
	if(cursor->edges == 1)
	{
		cursor->remove = 1;
		adjAdjust(cursor->adj->parent, cursor->id);
		removals++;
	}
	if(cursor->edges == 0)
	{
		cursor->remove = 1;
		removals++;
	}
	return removals;

}

bool sameConnections(vertex *zergNode)
{
	bool goodGraph = true;
	vertex *cursor = zergNode;
	int graphConn;
	while(cursor->remove != 0)
	{
		cursor = cursor->next;
	}
	graphConn = cursor->edges;
	cursor = zergNode;
	while(cursor)
	{
		if(cursor->edges != graphConn && cursor->remove == 0)
		{
			goodGraph = false;
			return goodGraph;
		}
		cursor = cursor->next;
	}
	return goodGraph;
}
	
void printRemovals(vertex *front)
{
	vertex *cursor;
	cursor = front;
	while(cursor != NULL)
	{
		if(cursor->remove == 1)
		{
			printf("Remove Zerg #%d\n", cursor->id);
		}
		cursor = cursor->next;
		
	}
}

void adjAdjust(vertex *zergNode, int adjId)
{
	vertex *cursor;
	edge *adjacencyIndex;

	cursor = zergNode;
	adjacencyIndex = cursor->adj;
	cursor->edges--;

	while(adjacencyIndex)
	{
		if(adjacencyIndex->id == adjId)
		{
			adjacencyIndex->remove += 1;
			return;
		}
		adjacencyIndex = adjacencyIndex->next;
	}	
}

//This function sets the cost to the greates number of adjancies present between
//two adjacent nodes.
void gainWeight(vertex *zergNode)
{
	vertex *placeHolder = zergNode;	
	edge *temp;
	
	while(placeHolder)
	{
		temp = placeHolder->adj;
		while(temp)
		{
			//setting haves to edge weight in order to force some form of path
			//during dijkstra's
			if(placeHolder->edges < temp->parent->edges)
			{
				temp->weight = temp->parent->edges;
			}
			else
			{
				temp->weight = placeHolder->edges;
			}
			temp = temp->next;
		}
	placeHolder = placeHolder->next;
	}
}



				
					
	



