#ifndef ZERG_FUNCTIONS_H
#define ZERG_FUNCTIONS_H

#include "zergmap.h"


extern union PayloadStructs *zerg;
void messFunction(union PayloadStructs *zerg);
void statFunction(union PayloadStructs *zerg);
void commFunction(union PayloadStructs *zerg);
struct vertex* gpsFunction(struct vertex *zergNode, 
					union PayloadStructs *zerg, uint32_t id);


#endif
