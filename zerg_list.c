#include <stdio.h>
#include <stdlib.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <arpa/inet.h>

#include "zerg_functions.h"
#include "zergmap_functions.h"
#include "zerg_list.h"

vertex *insertVertex(vertex *front, int id, uint64_t lon, uint64_t lat){

	
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
		insert->next=NULL;
		insert->adj = NULL;
		insert->next=front;
		front=insert; // notice how the newest code is considered front now
	// connect to list

	return front;
}

edge * insertEdge(edge *front, int id, int weight){

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
	insert->next=NULL;

	insert->next=front;
	front=insert; // notice how the newest code is considered front now
	return front;
}
