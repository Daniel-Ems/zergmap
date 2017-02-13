#ifndef ZERG_FUNCTIONS_H
#define ZERG_FUNCTIONS_H

#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <arpa/inet.h>


extern union PayloadStructs *zerg;
void messFunction(union PayloadStructs *zerg);
void statFunction(union PayloadStructs *zerg);
void commFunction(union PayloadStructs *zerg);
void gpsFunction(union PayloadStructs *zerg);


#endif
