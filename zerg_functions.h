#ifndef ZERG_FUNCTIONS_H
#define ZERG_FUNCTIONS_H

#include "healthTree.h"
#include "zerg_list.h"
#include "zergmap_functions.h"
#include "zerg_functions.h"
#include "dijkstra.h"

extern union PayloadStructs *zerg;
void messFunction(union PayloadStructs *zerg);
healthTree* statFunction(healthTree *tree, union PayloadStructs *zerg, 
						uint32_t zergId);
void commFunction(union PayloadStructs *zerg);
struct vertex* gpsFunction(struct vertex *zergNode, 
					union PayloadStructs *zerg, uint32_t id);


#endif
