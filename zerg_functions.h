#ifndef ZERG_FUNCTIONS_H
#define ZERG_FUNCTIONS_H

#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <arpa/inet.h>
#include "zerg_list.h"


extern union PayloadStructs *zerg;
void messFunction(union PayloadStructs *zerg);
void statFunction(union PayloadStructs *zerg);
void commFunction(union PayloadStructs *zerg);
vertex* gpsFunction(vertex *zergNode, union PayloadStructs *zerg, uint32_t id);


#endif
