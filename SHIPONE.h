#ifndef _SHIPONE_H_
#define _SHIPONE_H_

#include "shipgenmaths.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <time.h>
#include <math.h>

typedef enum {STDWING, LERX1, LERX2, LERX3} ShipOneWingType_t;

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
	
	ShipOneWingType_t isLongWing;
	
	uint8_t isDualEngine;
	
}shipOneParams_t;

shipOneParams_t shipParams;


void GenShipOneParams(shipOneParams_t* params);
void MakeShipOne(shipOne_t* ship, shipOneParams_t params);
void ShipOneMonoNose(shipOne_t* ship, shipOneParams_t params);
void ShipOneMid(shipOne_t* ship, shipOneParams_t params);
void ShipOneWings(shipOne_t* ship, shipOneParams_t params);
void ShipOneTwinTail(shipOne_t* ship, shipOneParams_t params);
void ShipOneMonoTail(shipOne_t* ship, shipOneParams_t params);

#endif
