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

#include "dijkstra.h"
#include "zerg_list.h"
#include "zergmap_functions.h"
#include "zerg_functions.h"


struct queue *
qInit (struct queue *priq, int vertex)
{
  priq->index = -1;
  priq->size = vertex;
  priq->next = calloc (priq->size, sizeof (edge));
  return priq;
}

// remove inserting in init function
/*
void insert(struct queue *priq, int id, int weight, int from)
{
	priq->index++;
	priq->next[priq->index]->weight = weight;
	priq->next[priq->index]->id = id;
	priq->next[priq->index]->from = from;

	struct path temp = {0};
	int parent = floor((priq->index -1 ) / 2);
	int child = priq->index;

	while (priq->next[parent]->weight > priq->next[child]->weight)
	{	
		temp = priq->next[child];
		priq->next[child] = priq->next[parent];
		priq->next[parent] = temp;
		child = parent;
		parent = floor((parent -1 ) / 2);
	}
}
*/
//attempt to populate priq with the edge nodes from my vertex's

void
insert (struct queue *priq, edge * adjacency, int from, int cost)
{
  bool update = true;
  int i;
  int parent;
  struct edge *temp;
  int tempCost = cost + adjacency->weight;
  if (priq->index >= 0)
    {
      for (i = 0; i < priq->index; i++)
	{
	  if (priq->next[i]->id == adjacency->id)
	    {
	      if (priq->next[i]->total > tempCost)
		{
		  priq->next[i]->total = tempCost;
		  priq->next[i]->from = from;
		  parent = floor ((i - 1) / 2);
		  while (priq->next[i]->total < priq->next[parent]->total)
		    {
		      temp = priq->next[i];
		      priq->next[i] = priq->next[parent];
		      priq->next[parent] = temp;
		      i = parent;
		      parent = floor ((parent - 1) / 2);
		    }
		}
	      update = false;
	    }
	}
    }
  if (update == true)
    {
      priq->index++;
      priq->next[priq->index] = adjacency;
      priq->next[priq->index]->total += tempCost;
      priq->next[priq->index]->from = from;
      struct edge *temp = { 0 };
      if (priq->index > 0)
	{
	  int parent = floor ((priq->index - 1) / 2);
	  int child = priq->index;

	  while (priq->next[parent]->total > priq->next[child]->total)
	    {
	      temp = priq->next[child];
	      priq->next[child] = priq->next[parent];
	      priq->next[parent] = temp;
	      child = parent;
	      parent = floor ((parent - 1) / 2);
	    }
	}
    }
}


edge *
pop (struct queue *priq)
{
  struct edge *popped = { 0 };
  struct edge *temp = { 0 };
  int top = 0;
  popped = priq->next[top];
  if (priq->index == 0)
    {
      priq->next[top] = NULL;
      return popped;
    }
  priq->next[top] = priq->next[priq->index];
  priq->index--;
  int left = 1;
  int right = 2;
  if (priq->index > right)
    {
      while ((priq->next[top]->total > priq->next[left]->total) ||
	     (priq->next[top]->total > priq->next[right]->total))
	{
	  if (priq->next[left]->total < priq->next[right]->total)
	    {
	      temp = priq->next[top];
	      priq->next[top] = priq->next[left];
	      priq->next[left] = temp;
	      top = left;
	    }
	  else
	    {
	      if (right > priq->index)
		{
		  break;
		}
	      temp = priq->next[top];
	      priq->next[top] = priq->next[right];
	      priq->next[right] = temp;
	      top = right;
	    }
	  right = (top * 2) + 2;
	  left = (top * 2) + 1;
	  if (left > priq->index)
	    {
	      break;
	    }
	}
    }
  else
    {
      if (priq->next[left]->total < priq->next[top]->total)
	{
	  temp = priq->next[top];
	  priq->next[top] = priq->next[left];
	  priq->next[left] = temp;
	}
    }

  return popped;
}

void
obliviate (struct queue *priq)
{

  free (priq->next);
  free (priq);
}

vertex *
findPlace (vertex * zergNode, int id)
{
  vertex *find = zergNode;
  while (find != NULL)
    {
      if (find->id == id)
	{
	  return find;
	}
      find = find->next;
    }
  return find;
}
