#include "SHIPPROTO.h"


void GenProtOneParams(shipOneParams_t* params){
	printf("Generating params!\n");
	degrad=M_PI/180.0f;
	
	params->mainLen=10.0f+((rand()%300)/100.0f);
	
	params->mainWid=0.22f*params->mainLen+((rand()%10)/100.0f);
	
	params->mainHei=0.085f*params->mainLen+((rand()%50)/1000.0f);

	params->noseSlant=30.0f+(rand()%100);
	params->noseWidBot=0.3f*params->mainWid+((rand()%100)/1000.0f);
	params->noseWidTop=0.5f*params->noseWidBot;
	
	params->noseHei=0.17f*params->mainHei+((rand()%80)/1000.0f);
	params->noseLen=params->noseHei/tan(degrad*params->noseSlant);
	
	params->wingSpan=0.60f*params->mainWid-(rand()%80)/200.0f;
	params->wingRoot=2.4f+(rand()%40)/100.0f;
	params->wingTip=(0.03f+(rand()%30)/100.0f)*params->wingRoot;
	params->wingSweep=40.0f+((rand())%40); //Calculate wingtip y as -SPAN/tan(SWEEP)
	params->wingDihedral=180.0f/M_PI*atan(params->mainHei/(params->mainWid*0.5f))+5.0f;
	params->wingPos=-1.3f-(rand()%150)/1000.0f;
	
	params->cockpitWid=params->noseWidTop+(rand()%100)/1000.0f;
	params->cockpitLen=params->mainLen*0.23f-(rand()%100)/1000.0f;
	params->cockpitHei=0.33f*params->cockpitWid*tan(degrad*params->wingDihedral);
	params->cockpitPos=-0.3f-(rand()%300)/1000.0f;
	params->cockpitDropFact=0.15f+(rand()%50)/1000.0f;
	
	params->tailWid=0.4f*params->mainWid+(rand()%300)/1000.0f;
	params->tailHei=0.5f*params->mainHei;
	params->exhLength=0.0f;
	params->exhRadius=0.22f;
	
	params->aBrakeLen=params->wingRoot*0.22f-(rand()%100)/1000.0f;
	params->aBrakeEdgeOffset=0.2f;
	
	params->isLongWing=rand()%3; //GENERATE FEISAR WINGS
	params->hasEngCover = rand()%2;
	
	params->isDualEngine=rand()%3; //Generate Twin Engine
	params->engCoverType=rand()%2;
	printf("Done!\n");
}


void MakeProtoOne(shipOne_t* ship, shipOneParams_t params){
	
	uint8_t i = 0;
	
	ProtOneNose(ship, params);
	ProtOneMid(ship, params);
	ProtOneTail(ship, params);
	ProtOneWings(ship, params);
	
	for(i=0;i<ship->nose.ntris;i++)	
		ship->nose.polys[i].polysubgrp = NOSE;
	for(i=0;i<ship->mid.ntris;i++)	
		ship->mid.polys[i].polysubgrp = MID;
	for(i=0;i<ship->tail.ntris;i++)	
		ship->tail.polys[i].polysubgrp = TAIL;	
	for(i=0;i<ship->wings.ntris;i++)	
		ship->wings.polys[i].polysubgrp = WINGS;
}

void ProtOneNose(shipOne_t* ship, shipOneParams_t params){
	
	float noseLenFactor = 0.5f;
	float noseWidFactorF = 0.1f; //tip width
	float noseWidFactorB = 0.18f; //max bottom width
	
	ship->nose.nverts = 8;
	ship->nose.ntris = 8;
	
	ship->nose.verts = malloc(ship->nose.nverts*sizeof(vert_t));
	ship->nose.polys = malloc(ship->nose.ntris*sizeof(poly_t));
	
	if(ship->nose.verts == NULL || ship->nose.polys == NULL){
		fprintf(stderr,"E!\n");
		exit(-1);
	}
	
	ship->nose.verts[0].x = 0.5f*noseWidFactorF*params.mainWid; ship->nose.verts[0].y = 0.5f*params.mainLen; ship->nose.verts[0].z = 0.0f;
	ship->nose.verts[1] = MirrorVert(ship->nose.verts[0],'x');
	
	ship->nose.verts[2] = TranslateVert(ship->nose.verts[0],0.0f,-0.25*params.mainLen,0.45f*params.mainHei);
	ship->nose.verts[3] = MirrorVert(ship->nose.verts[2],'x');
	
	ship->nose.verts[4] = TranslateVert(ship->nose.verts[0],(noseWidFactorB)*params.mainWid,-0.25*params.mainLen,0.0f);
	ship->nose.verts[5] = MirrorVert(ship->nose.verts[4],'x');
	
	ship->nose.verts[6] = TranslateVert(VLerp(params.cockpitDropFact,ship->nose.verts[2],ship->nose.verts[4]),(ship->nose.verts[4].x-ship->nose.verts[0].x)*params.cockpitDropFact,-params.cockpitDropFact,-(ship->nose.verts[2].z-ship->nose.verts[0].z)*params.cockpitDropFact);
	ship->nose.verts[7] = MirrorVert(ship->nose.verts[6],'x');
	
	Triangulate(&ship->nose.polys[0],&ship->nose.polys[1],0,1,3,2,-1,DIFFUSE2);
	Triangulate(&ship->nose.polys[2],&ship->nose.polys[3],4,0,2,6,-1,DIFFUSE1);
	Triangulate(&ship->nose.polys[4],&ship->nose.polys[5],5,1,3,7,1,DIFFUSE1);
	Triangulate(&ship->nose.polys[6],&ship->nose.polys[7],0,1,5,4,1,BOTTOM);
	
}

void ProtOneMid(shipOne_t* ship, shipOneParams_t params){
	
	uint8_t i = 0;
	
	ship->mid.nverts = 12;
	ship->mid.ntris = 12;
	
	ship->mid.verts = malloc(ship->mid.nverts*sizeof(vert_t));
	ship->mid.polys = malloc(ship->mid.ntris*sizeof(poly_t));
	
	if(ship->mid.verts == NULL || ship->mid.polys == NULL){
		fprintf(stderr,"E!\n");
		exit(-1);
	}
	
	for(i=0;i<6;i++)
		ship->mid.verts[i] = ship->nose.verts[i+2];
		
	ship->mid.verts[6] = TranslateVert(ship->mid.verts[0],0.0f,-ship->mid.verts[0].y,-ship->mid.verts[0].y*(ship->nose.verts[2].z-ship->nose.verts[0].z)/(ship->nose.verts[2].y-ship->nose.verts[0].y));
	
	ship->mid.verts[8] = TranslateVert(ship->mid.verts[2],params.mainWid*0.10f,-0.75f*ship->mid.verts[2].y,0.0f);
	
	ship->mid.verts[10] = VLerp(2.35f*params.cockpitDropFact,ship->mid.verts[6],ship->mid.verts[8]);
	
	for(i=7;i<12;i+=2)
		ship->mid.verts[i] = MirrorVert(ship->mid.verts[i-1],'x');
		
	Triangulate(&ship->mid.polys[0],&ship->mid.polys[1],0,1,7,6,-1,GLASS);
	Triangulate(&ship->mid.polys[2],&ship->mid.polys[3],0,4,10,6,1,GLASS);
	Triangulate(&ship->mid.polys[4],&ship->mid.polys[5],1,5,11,7,-1,GLASS);
	
	Triangulate(&ship->mid.polys[6],&ship->mid.polys[7],4,2,8,10,1,DIFFUSE1);
	Triangulate(&ship->mid.polys[8],&ship->mid.polys[9],5,3,9,11,-1,DIFFUSE1);
	
	Triangulate(&ship->mid.polys[10],&ship->mid.polys[11],2,3,9,8,1,BOTTOM);
	
}

void ProtOneTail(shipOne_t* ship, shipOneParams_t params){
	
	uint8_t i = 0;
	vert_t VORIGIN = {0,0,0};
	
	ship->tail.nverts = 14;
	ship->tail.ntris = 20;
	
	ship->tail.verts = malloc(ship->tail.nverts*sizeof(vert_t));
	ship->tail.polys = malloc(ship->tail.ntris*sizeof(poly_t));
	
	if(ship->tail.verts == NULL || ship->tail.polys == NULL){
		fprintf(stderr,"E!\n");
		exit(-1);
	}
	
	for(i=0;i<6;i++)
		ship->tail.verts[i] = ship->mid.verts[i+6];
		
	ship->tail.verts[6] = TranslateVert(ship->tail.verts[2],0.4f*params.wingSpan,-1.25f*ship->tail.verts[2].y,-0.2f*params.wingSpan*sin(degrad*params.wingDihedral));
	
	ship->tail.verts[8] = TranslateVert(ship->tail.verts[4],0.2f*params.wingSpan,-1.05f*ship->tail.verts[2].y,-0.1f*params.wingSpan*sin(degrad*params.wingDihedral));
	
	ship->tail.verts[10] = TranslateVert(VORIGIN,0.25f*params.mainWid,-0.24f*params.mainLen,0.7f*(ship->tail.verts[0].z-ship->tail.verts[8].z));
	
	ship->tail.verts[12] = TranslateVert(VORIGIN,0.15f*params.mainWid,-0.26f*params.mainLen,1.3f*(ship->tail.verts[0].z-ship->tail.verts[8].z));
	
	for(i=7;i<14;i+=2)
		ship->tail.verts[i] = MirrorVert(ship->tail.verts[i-1],'x');
		
	Triangulate(&ship->tail.polys[0],&ship->tail.polys[1],0,1,13,12,-1,MECHANICS);
	Triangulate(&ship->tail.polys[2],&ship->tail.polys[3],2,3,11,10,1,BOTTOM);
	
	Triangulate(&ship->tail.polys[4],&ship->tail.polys[5],4,0,12,8,-1,DIFFUSE1);
	Triangulate(&ship->tail.polys[6],&ship->tail.polys[7],5,1,13,9,1,DIFFUSE1);
	
	Triangulate(&ship->tail.polys[8],&ship->tail.polys[9],6,8,12,10,-1,DIFFUSE2);
	Triangulate(&ship->tail.polys[10],&ship->tail.polys[11],7,9,13,11,1,DIFFUSE2);
	
	CreateTriangle(&ship->tail.polys[12],4,8,2,-1,DIFFUSE1);
	CreateTriangle(&ship->tail.polys[13],2,8,6,-1,EXHAUST);
	
	CreateTriangle(&ship->tail.polys[14],5,9,3,1,DIFFUSE1);
	CreateTriangle(&ship->tail.polys[15],3,9,7,1,EXHAUST);
	
	CreateTriangle(&ship->tail.polys[16],2,10,6,1,BOTTOM);
	CreateTriangle(&ship->tail.polys[17],3,11,7,-1,BOTTOM);
	
	Triangulate(&ship->tail.polys[18],&ship->tail.polys[19],12,13,11,10,-1,EXHAUST_INNER);
}

void ProtOneWings(shipOne_t* ship, shipOneParams_t params){
	
	uint8_t i=0;
	
	ship->wings.nverts = 8;
	ship->wings.ntris = 4;
	
	ship->wings.verts = malloc(ship->wings.nverts*sizeof(vert_t));
	ship->wings.polys = malloc(ship->wings.ntris*sizeof(poly_t));
	
	if(ship->wings.verts == NULL || ship->wings.polys == NULL){
		fprintf(stderr,"E!\n");
		exit(-1);
	}
	
	
	ship->wings.verts[0] = ship->tail.verts[6];
	ship->wings.verts[2] = ship->tail.verts[10];
	
	ship->wings.verts[4] = TranslateVert(ship->wings.verts[0],0.5f*params.wingSpan,0.15f*params.wingRoot,-0.5f*params.wingSpan*sin(degrad*params.wingDihedral));
	ship->wings.verts[6] = TranslateVert(ship->wings.verts[4],0.0f,-0.5f,0.0f);
	
	ship->wings.verts[1] = MirrorVert(ship->wings.verts[0],'x');	
	ship->wings.verts[3] = MirrorVert(ship->wings.verts[2],'x');
	ship->wings.verts[5] = MirrorVert(ship->wings.verts[4],'x');
	ship->wings.verts[7] = MirrorVert(ship->wings.verts[6],'x');
	
	Triangulate(&ship->wings.polys[0],&ship->wings.polys[1],0,2,6,4,-1,DIFFUSE1);
	Triangulate(&ship->wings.polys[2],&ship->wings.polys[3],1,3,7,5,1,DIFFUSE1);	
	
}
