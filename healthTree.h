#ifndef HEALTHTREE_H

#define HEALTHTREE_H

#include <stdio.h>
#include <stdlib.h>
#include <sysexits.h>
#include <sys/stat.h>
#include <string.h>
#include <ctype.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <strings.h>
#include <stdbool.h>

typedef struct healthTree
{
  int id;
  double health;
  struct healthTree *left;
  struct healthTree *right;
  int count;

} healthTree;

enum
{ MAX_HEALTH = 100 };
healthTree *insertTree (healthTree * tree, int id, double health);
void print_tree (healthTree * tree, int lowHealth);
void destroy_healthTree (healthTree * tree);

#endif
