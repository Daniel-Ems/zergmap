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
		insert->next=front;
		front=insert; // notice how the newest code is considered front now
	// connect to list

	return front;
}

edge * insertEdge(edge *front, int id, double weight){

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

	
void destroy(vertex *front)
{
	

	vertex *cursor;
	cursor=front;
	edge *edge;

	while(cursor!=NULL)
	{
		edge = cursor->adj;
		while(edge)
		{
			cursor->adj = cursor->adj->next;
			free(edge);
			edge = cursor->adj;
		}

		front=front->next;
		free(cursor);
		cursor=front;
	}
}

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



void printAdj(vertex *zergNode)
{
	vertex *cursor;
	cursor = zergNode;
	edge *edge;
	while(cursor != NULL)
	{	
		edge = cursor->adj;
		printf("cursor id-> %d\n", cursor->id);
		while(edge !=NULL)
		{
			printf("adj id -> %d\n", edge->id);
			printf("adj haves ->%lf\n", edge->weight);
			edge = edge -> next;
		}
		cursor = cursor->next;

		puts(" ");
	}
}

				
					
	



