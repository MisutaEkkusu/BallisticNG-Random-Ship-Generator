#ifndef _PRIM_H_
#define _PRIM_H_

#include "shipgenmaths.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <time.h>
#include <math.h>

#define MAXCVERT 100 //maximum coincident vertex ammount

typedef enum {UVXY, UVXZ, UVYZ} uvProject_t;

typedef struct shipOne{
	
	vert_t pos;
	
	mesh_t nose;
	mesh_t mid;
	mesh_t tail;
	mesh_t wings;
	
	mesh_t obj;
	
}shipOne_t;

shipOne_t ship;

typedef struct shipOneParams{
	
	float mainLen;
	float mainWid;
	float mainHei; 
	
	float noseLen;
	float noseHei;
	float noseWidBot;
	float noseWidTop;
	float noseSlant; 
	
	float tailWid;
	float tailHei; 
	float exhRadius;
	float exhLength;
	
	float wingSpan;
	float wingRoot;
	float wingDihedral;
	float wingSweep;
	float wingPos; 
	float wingTip;
	
	float cockpitPos;
	float cockpitLen;
	float cockpitHei;
	float cockpitWid; 
	float cockpitDropFact;
	
	float aBrakeLen;
	float aBrakeEdgeOffset;
	
	uint8_t isLongWing;
	uint8_t isDualEngine;
	
}shipOneParams_t;

shipOneParams_t shipParams;

void MeshToObj(mesh_t* mesh, char* filename);

void GenShipOneParams(shipOneParams_t* params);
void MakeShipOne(shipOne_t* ship, shipOneParams_t params);
void ShipOneTwinTail(shipOne_t* ship, shipOneParams_t params);
void ShipOneMonoTail(shipOne_t* ship, shipOneParams_t params);
void ShipToObj(shipOne_t ship);
void FreeShip(shipOne_t* ship);
uint8_t MergeMesh(mesh_t m1, mesh_t m2, mesh_t* ret, uint8_t noMerge);


#endif
