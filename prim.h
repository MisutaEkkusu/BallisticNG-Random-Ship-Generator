#ifndef _PRIM_H_
#define _PRIM_H_

#include "SHIPONE.h"

#define MAXCVERT 100 //maximum coincident vertex ammount

typedef enum {UVXY, UVXZ, UVYZ} uvProject_t;


void MeshToObj(mesh_t* mesh, char* filename);


void ShipToObj(shipOne_t ship);
void FreeShip(shipOne_t* ship);
uint8_t MergeMesh(mesh_t m1, mesh_t m2, mesh_t* ret, uint8_t noMerge);


#endif
