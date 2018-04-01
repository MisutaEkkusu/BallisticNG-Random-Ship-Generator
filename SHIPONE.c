#include "SHIPONE.h"


void GenShipOneParams(shipOneParams_t* params){
	printf("Generating params!\n");
	degrad=M_PI/180.0f;
	
	params->mainLen=10.0f+((rand()%100)/100.0f);
	
	params->mainWid=0.22f*params->mainLen+((rand()%10)/100.0f);
	
	params->mainHei=0.085f*params->mainLen+((rand()%50)/1000.0f);
	
	params->noseSlant=30.0f+(rand()%100);
	params->noseWidBot=0.3f*params->mainWid+((rand()%100)/1000.0f);
	params->noseWidTop=0.5f*params->noseWidBot;
	
	params->noseHei=0.17f*params->mainHei+((rand()%80)/1000.0f);
	params->noseLen=params->noseHei/tan(degrad*params->noseSlant);
	
	params->wingSpan=0.65f*params->mainWid-(rand()%100)/200.0f;
	params->wingRoot=2.4f+(rand()%40)/100.0f;
	params->wingTip=(0.05f+(rand()%10)/100.0f)*params->wingRoot;
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
	
	params->isDualEngine=rand()%3; //Generate Twin Engine
	printf("Done!\n");

}


void MakeShipOne(shipOne_t* ship, shipOneParams_t params){
	uint32_t i = 0;
	printf("Generating ship mesh...\n");
	ShipOneMonoNose(ship, params);
	ShipOneMid(ship, params);
	ShipOneWings(ship,params);

	if(params.isDualEngine!=0)
		ShipOneTwinTail(ship,params);
	else
		ShipOneMonoTail(ship,params);
		
	
	for(i=0;i<ship->nose.ntris;i++)	
		ship->nose.polys[i].polysubgrp = NOSE;
	for(i=0;i<ship->mid.ntris;i++)	
		ship->mid.polys[i].polysubgrp = MID;
	for(i=0;i<ship->tail.ntris;i++)	
		ship->tail.polys[i].polysubgrp = TAIL;	
	for(i=0;i<ship->wings.ntris;i++)	
		ship->wings.polys[i].polysubgrp = WINGS;	
		
	printf("Done!\n");
}


void ShipOneMonoNose(shipOne_t* ship, shipOneParams_t params){
	
	float tgtWidth=lerp((0.5f*params.mainLen)/(0.5f*params.mainLen-params.wingPos),0.5f*params.noseWidBot,0.5f*params.mainWid);
	
	printf("Building nose: Arrowhead\n");
	
	//NOSE--------------------------------------------------------------
	ship->nose.ntris=10;
	ship->nose.nverts=8;
	
	ship->nose.verts=malloc(ship->nose.nverts*sizeof(vert_t));
	ship->nose.polys=malloc(ship->nose.ntris*sizeof(poly_t));
	
	if(ship->nose.verts == NULL || ship->nose.polys == NULL ){
		fprintf(stderr,"Failed to allocate ship nose memory.\n");
		exit(-1);
	}
	//VERTS	
	ship->nose.verts[0].x=0.5f*params.noseWidBot; ship->nose.verts[0].y=params.mainLen*0.5f; ship->nose.verts[0].z=0.0f;
	ship->nose.verts[1].x=0.5f*params.noseWidTop; ship->nose.verts[1].y=params.mainLen*0.5f-params.noseLen; ship->nose.verts[1].z=params.noseHei;
	
	ship->nose.verts[2].x=-0.5f*params.noseWidTop; ship->nose.verts[2].y=params.mainLen*0.5f-params.noseLen; ship->nose.verts[2].z=params.noseHei;
	ship->nose.verts[3].x=-0.5f*params.noseWidBot; ship->nose.verts[3].y=params.mainLen*0.5f; ship->nose.verts[3].z=0.0f;
	
	ship->nose.verts[4].x=tgtWidth; ship->nose.verts[4].y=0.0f; ship->nose.verts[4].z=0.0f;
	ship->nose.verts[5].x=0.5f*params.cockpitWid; ship->nose.verts[5].y=params.cockpitPos; ship->nose.verts[5].z=params.mainHei;
	
	ship->nose.verts[6].x=-0.5f*params.cockpitWid; ship->nose.verts[6].y=params.cockpitPos; ship->nose.verts[6].z=params.mainHei;
	ship->nose.verts[7].x=-tgtWidth; ship->nose.verts[7].y=0.0f; ship->nose.verts[7].z=0.0f;
	
	//TRIANGLES
	//NOSE LEFT
	Triangulate(&ship->nose.polys[0],&ship->nose.polys[1],0,4,5,1,1,DIFFUSE1);
	//NOSE TOP
	Triangulate(&ship->nose.polys[2],&ship->nose.polys[3],1,5,6,2,1,DIFFUSE2);
	//NOSE RIGHT
	Triangulate(&ship->nose.polys[4],&ship->nose.polys[5],6,7,3,2,1,DIFFUSE1);
	//NOSE FRONT
	Triangulate(&ship->nose.polys[6],&ship->nose.polys[7],0,1,2,3,1,INTAKE);
	//NOSE BOTTOM
	Triangulate(&ship->nose.polys[8],&ship->nose.polys[9],0,4,7,3,-1,BOTTOM);
	
	
}

void ShipOneMid(shipOne_t* ship, shipOneParams_t params){
	float tgtWidth=lerp((0.5f*params.mainLen)/(0.5f*params.mainLen-params.wingPos),0.5f*params.noseWidBot,0.5f*params.mainWid);
	printf("Building mid-section: Standard\n");
	//MID----------------------------------------------------------------------------------------
	ship->mid.ntris=28; //8
	ship->mid.nverts=18;
	
	ship->mid.verts=malloc(ship->mid.nverts*sizeof(vert_t));
	ship->mid.polys=malloc(ship->mid.ntris*sizeof(poly_t));
	
	if(ship->mid.verts == NULL || ship->mid.polys == NULL ){
		fprintf(stderr,"Failed to allocate ship mid memory.\n");
		exit(-1);
	}
	
	
	ship->mid.verts[0].x=tgtWidth; ship->mid.verts[0].y=0.0f; ship->mid.verts[0].z=0.0f;
	ship->mid.verts[1].x=0.5f*params.cockpitWid; ship->mid.verts[1].y=params.cockpitPos; ship->mid.verts[1].z=params.mainHei;
	
	ship->mid.verts[2].x=-0.5f*params.cockpitWid; ship->mid.verts[2].y=params.cockpitPos; ship->mid.verts[2].z=params.mainHei;
	ship->mid.verts[3].x=-tgtWidth; ship->mid.verts[3].y=0.0f; ship->mid.verts[3].z=0.0f;
	
	ship->mid.verts[4].x=0.5f*params.mainWid; ship->mid.verts[4].y=params.wingPos; ship->mid.verts[4].z=0.0f;
	
	ship->mid.verts[5]=VLerp(params.cockpitDropFact,ship->mid.verts[1],ship->mid.verts[4]);
	ship->mid.verts[5].x=lerp(1.0f-params.cockpitDropFact,0.5f*params.mainWid,0.7f*params.cockpitWid);
	ship->mid.verts[5].y=params.cockpitPos-params.cockpitLen*0.4f;
	
	ship->mid.verts[6].x=-ship->mid.verts[5].x; ship->mid.verts[6].y=ship->mid.verts[5].y; ship->mid.verts[6].z=ship->mid.verts[5].z;
	ship->mid.verts[7].x=-0.5f*params.mainWid; ship->mid.verts[7].y=params.wingPos; ship->mid.verts[7].z=0.0f;
	
	ship->mid.verts[8].x=0.5f*params.mainWid; ship->mid.verts[8].y=params.wingPos-params.wingRoot; ship->mid.verts[8].z=0.0f;
	ship->mid.verts[9].x=ship->mid.verts[5].x; ship->mid.verts[9].y=params.cockpitPos-params.cockpitLen*0.8f; ship->mid.verts[9].z=ship->mid.verts[5].z;
	
	ship->mid.verts[10].x=-ship->mid.verts[5].x; ship->mid.verts[10].y=params.cockpitPos-params.cockpitLen*0.8f; ship->mid.verts[10].z=ship->mid.verts[5].z;
	ship->mid.verts[11].x=-0.5f*params.mainWid; ship->mid.verts[11].y=params.wingPos-params.wingRoot; ship->mid.verts[11].z=0.0f;
	
	ship->mid.verts[12].x=0.4f*params.cockpitWid; ship->mid.verts[12].y=params.cockpitPos-0.4f*params.cockpitLen; ship->mid.verts[12].z=params.mainHei+params.cockpitHei;
	ship->mid.verts[13].x=-0.4f*params.cockpitWid; ship->mid.verts[13].y=params.cockpitPos-0.4f*params.cockpitLen; ship->mid.verts[13].z=params.mainHei+params.cockpitHei;
	
	ship->mid.verts[14].x=0.4f*params.cockpitWid; ship->mid.verts[14].y=params.cockpitPos-0.75f*params.cockpitLen; ship->mid.verts[14].z=params.mainHei+params.cockpitHei;
	ship->mid.verts[15].x=-0.4f*params.cockpitWid; ship->mid.verts[15].y=params.cockpitPos-0.75f*params.cockpitLen; ship->mid.verts[15].z=params.mainHei+params.cockpitHei;
	
	ship->mid.verts[16].x=0.7f*params.cockpitWid; ship->mid.verts[16].y=params.cockpitPos-params.cockpitLen; ship->mid.verts[16].z=params.mainHei;
	ship->mid.verts[17].x=-0.7f*params.cockpitWid; ship->mid.verts[17].y=params.cockpitPos-params.cockpitLen; ship->mid.verts[17].z=params.mainHei;
	
	//TRIANGLES
	
	//FRONT LEFT
	Triangulate(&ship->mid.polys[0],&ship->mid.polys[1],4,5,1,0,1,DIFFUSE1);
	//FRONT RIGHT
	Triangulate(&ship->mid.polys[2],&ship->mid.polys[3],7,3,2,6,1,DIFFUSE1);
	//BACK LEFT
	Triangulate(&ship->mid.polys[4],&ship->mid.polys[5],4,8,9,5,1,DIFFUSE2);
	//BACK RIGHT
	Triangulate(&ship->mid.polys[6],&ship->mid.polys[7],10,11,7,6,1,DIFFUSE2);
	//FRONT BOT
	Triangulate(&ship->mid.polys[8],&ship->mid.polys[9],0,4,7,3,-1,BOTTOM);
	//BACK BOT
	Triangulate(&ship->mid.polys[10],&ship->mid.polys[11],4,8,11,7,-1,BOTTOM);
	
	//Cockpit front
	Triangulate(&ship->mid.polys[12],&ship->mid.polys[13],1,12,13,2,1,GLASS);
	//Cockpit top
	Triangulate(&ship->mid.polys[14],&ship->mid.polys[15],12,14,15,13,1,GLASS);
	//Cockpit back
	Triangulate(&ship->mid.polys[16],&ship->mid.polys[17],14,16,17,15,1,GLASS);
	
	//Cockpit sides (front mid back)
	CreateTriangle(&ship->mid.polys[18],1,5,12,1,GLASS);
	CreateTriangle(&ship->mid.polys[19],2,13,6,1,GLASS);
	CreateTriangle(&ship->mid.polys[20],9,16,14,1,GLASS);
	CreateTriangle(&ship->mid.polys[21],15,17,10,1,GLASS);
	Triangulate(&ship->mid.polys[22],&ship->mid.polys[23],5,9,14,12,1,GLASS);
	Triangulate(&ship->mid.polys[24],&ship->mid.polys[25],6,13,15,10,1,GLASS);
	
	CreateTriangle(&ship->mid.polys[26],9,8,16,1,DIFFUSE1);
	CreateTriangle(&ship->mid.polys[27],10,17,11,1,DIFFUSE1);
	
}

void ShipOneWings(shipOne_t* ship, shipOneParams_t params){
	uint32_t i = 0;
	printf("Building wings: ");
	//WINGS-----------------------------------------------------------------------------------------------
	ship->wings.ntris=8; //12
	ship->wings.nverts=16; //16
	
	switch(params.isLongWing){
		
		case STDWING:
			printf("Standard\n");
			break;
		
		case LERX1:
			printf("LERX #1 (Long Wing)\n");
			ship->wings.ntris+=4;
			ship->wings.nverts+=4;	
			break;
			
		case LERX2:
			printf("LERX #2\n");
			ship->wings.ntris+=4;
			ship->wings.nverts+=6;
			break;
		
				
	}
	
	ship->wings.verts=malloc(ship->wings.nverts*sizeof(vert_t));
	ship->wings.polys=malloc(ship->wings.ntris*sizeof(poly_t));
	
	if(ship->wings.verts == NULL || ship->wings.polys == NULL ){
		fprintf(stderr,"Failed to allocate ship wings memory.\n");
		exit(-1);
	}
	//LEFT MAIN WING
	ship->wings.verts[0].x=0.5f*params.mainWid; 				ship->wings.verts[0].y=params.wingPos; 																		ship->wings.verts[0].z=0.0f;
	ship->wings.verts[1].x=0.5f*params.mainWid+params.wingSpan; ship->wings.verts[1].y=-params.wingSpan/tan(degrad*params.wingSweep)+params.wingPos; 						ship->wings.verts[1].z=-tan(degrad*params.wingDihedral)*params.wingSpan;
	ship->wings.verts[2].x=ship->wings.verts[1].x; 				ship->wings.verts[2].y=-params.wingSpan/tan(degrad*params.wingSweep)+params.wingPos-params.wingTip; 	ship->wings.verts[2].z=ship->wings.verts[1].z;
	ship->wings.verts[3].x=0.5f*params.mainWid; 				ship->wings.verts[3].y=params.wingPos-params.wingRoot; 														ship->wings.verts[3].z=0.0f;
	//RIGHT MAIN WING
	for(i=0;i<4;i++){
		ship->wings.verts[i+4].x=-ship->wings.verts[i].x;
		ship->wings.verts[i+4].y=ship->wings.verts[i].y;
		ship->wings.verts[i+4].z=ship->wings.verts[i].z;
	}
	
	//LEFT AIRBRAKE
	ship->wings.verts[8]=VLerp(params.aBrakeEdgeOffset,ship->wings.verts[3],ship->wings.verts[2]);
	ship->wings.verts[9]=ship->wings.verts[2];
	ship->wings.verts[10]=VLerp(1.0f,ship->wings.verts[8],ship->wings.verts[9]);
	ship->wings.verts[11]=VLerp(params.aBrakeEdgeOffset,ship->wings.verts[8],ship->wings.verts[9]);
	ship->wings.verts[10].y-=params.aBrakeLen; ship->wings.verts[11].y-=params.aBrakeLen;
	
	for(i=8;i<12;i++){
		ship->wings.verts[i+4].x=-ship->wings.verts[i].x;
		ship->wings.verts[i+4].y=ship->wings.verts[i].y;
		ship->wings.verts[i+4].z=ship->wings.verts[i].z;
	}
	
	Triangulate(&ship->wings.polys[0],&ship->wings.polys[1],1,2,3,0,1,DIFFUSE1);
	Triangulate(&ship->wings.polys[2],&ship->wings.polys[3],7,6,5,4,1,DIFFUSE1);
	Triangulate(&ship->wings.polys[4],&ship->wings.polys[5],8,9,10,11,1,DIFFUSE2);
	Triangulate(&ship->wings.polys[6],&ship->wings.polys[7],12,15,14,13,1,DIFFUSE2);
	
	
	
	switch(params.isLongWing){
		float LERX_xoffset=0.0f, LERX_yoffset=0.0f, LERX_span_factor=0.0f;
		uint8_t diffSweep = rand()%2; //has bigger sweep than Wing LE
		case LERX1:
			
			ship->wings.verts[16]=ship->mid.verts[0];
			ship->wings.verts[17]=ship->nose.verts[0];
			ship->wings.verts[18]=ship->mid.verts[3];
			ship->wings.verts[19]=ship->nose.verts[3];
			
			Triangulate(&ship->wings.polys[8],&ship->wings.polys[9],0,16,17,1,1,DIFFUSE2);
			Triangulate(&ship->wings.polys[10],&ship->wings.polys[11],19,18,4,5,1,DIFFUSE2);
		
			break;
			
			
		case LERX2:
		
			//LEFT LERX
			LERX_xoffset = fabs(ship->mid.verts[0].x-ship->wings.verts[0].x);
			LERX_yoffset = fabs(ship->mid.verts[0].y-ship->wings.verts[0].y);
			LERX_span_factor = (rand()%50)/100.0f;

			ship->wings.verts[16]=ship->mid.verts[0];
			
			ship->wings.verts[17]=VLerp(LERX_span_factor, ship->wings.verts[0], ship->wings.verts[1]);
			
			ship->wings.verts[18]=TranslateVert(ship->wings.verts[17], -LERX_xoffset, LERX_yoffset-diffSweep*LERX_span_factor*LERX_yoffset, 0.0f);
			
			Triangulate(&ship->wings.polys[8],&ship->wings.polys[9],16,0,17,18,-1,DIFFUSE2);
			
			//RIGHT LERX
			ship->wings.verts[19]=ship->mid.verts[3];
			
			ship->wings.verts[20]=VLerp(LERX_span_factor, ship->wings.verts[4], ship->wings.verts[5]);
			
			ship->wings.verts[21]=TranslateVert(ship->wings.verts[20], LERX_xoffset, LERX_yoffset-diffSweep*LERX_span_factor*LERX_yoffset, 0.0f);
			
			Triangulate(&ship->wings.polys[10],&ship->wings.polys[11],19,4,20,21,1,DIFFUSE2);
		
			break;
			
	}
	
}

void ShipOneTwinTail(shipOne_t* ship, shipOneParams_t params){
	
	//TAIL---------------------------------------------------------------------------------------------------
	printf("Building tail: ");
	ship->tail.ntris=26; //24
	ship->tail.nverts=16; //16
	
	ship->tail.verts=malloc(ship->tail.nverts*sizeof(vert_t));
	ship->tail.polys=malloc(ship->tail.ntris*sizeof(poly_t));
	
	if(ship->tail.verts == NULL || ship->tail.polys == NULL ){
		fprintf(stderr,"Failed to allocate ship tail memory.\n");
		exit(-1);
	}
	
	ship->tail.verts[0]=ship->mid.verts[8];
	ship->tail.verts[1]=ship->mid.verts[16];
	ship->tail.verts[2]=ship->mid.verts[17];
	ship->tail.verts[3]=ship->mid.verts[11];
	
	NGonize(&ship->tail, 4, 8, params.exhRadius,(18.0f)*degrad, params.tailWid*0.5f, -params.mainLen*0.5f, params.exhRadius);
	NGonize(&ship->tail, 9, 13, params.exhRadius,(18.0f)*degrad, -params.tailWid*0.5f, -params.mainLen*0.5f, params.exhRadius);
	
	ship->tail.verts[14].x = params.tailWid*0.5f; ship->tail.verts[14].y = -params.mainLen*0.5f+2*params.exhRadius; ship->tail.verts[14].z = params.exhRadius;
	ship->tail.verts[15].x = -params.tailWid*0.5f; ship->tail.verts[15].y = -params.mainLen*0.5f+2*params.exhRadius; ship->tail.verts[15].z = params.exhRadius;
	
	if(params.isDualEngine==1){
		printf("standard twin exhaust #1\n");
		Triangulate(&ship->tail.polys[0],&ship->tail.polys[1],1,0,4,5,1,DIFFUSE1); //Left Quad
		CreateTriangle(&ship->tail.polys[2],1,5,6,1,DIFFUSE2); //Left Tris Top
		CreateTriangle(&ship->tail.polys[3],8,4,0,1,DIFFUSE2); //Left Tris Bot
		
		Triangulate(&ship->tail.polys[4],&ship->tail.polys[5],1,6,10,2,1,MECHANICS); //Top Quad
		Triangulate(&ship->tail.polys[6],&ship->tail.polys[7],13,8,0,3,1,BOTTOM); //Bottom Qud
		
		Triangulate(&ship->tail.polys[8],&ship->tail.polys[9],2,11,12,3,1,DIFFUSE1); //Right Quad
		CreateTriangle(&ship->tail.polys[10],2,10,11,1,DIFFUSE2); //Left Tris Top
		CreateTriangle(&ship->tail.polys[11],13,3,12,1,DIFFUSE2); //Left Tris Bot
		
		Triangulate(&ship->tail.polys[12],&ship->tail.polys[13],6,7,9,10,1,DIFFUSE2); //Back Top 
		Triangulate(&ship->tail.polys[14],&ship->tail.polys[15],7,8,13,9,1,BOTTOM); //Back Bot
		
		TriangleFan(&ship->tail,16,14,4,8,-1,EXHAUST); //Left Exhaust
		TriangleFan(&ship->tail,21,15,9,13,-1,EXHAUST); //Right Exhaust
	}
	else if(params.isDualEngine==2){
		printf("standard twin exhaust #2\n");
		Triangulate(&ship->tail.polys[0],&ship->tail.polys[1],1,0,4,5,1,DIFFUSE1); //Left Quad
		CreateTriangle(&ship->tail.polys[2],1,5,6,1,DIFFUSE2); //Left Tris Top
		CreateTriangle(&ship->tail.polys[3],8,4,0,1,DIFFUSE2); //Left Tris Bot
		
		Triangulate(&ship->tail.polys[4],&ship->tail.polys[5],1,7,9,2,1,MECHANICS); //Top Quad
		Triangulate(&ship->tail.polys[6],&ship->tail.polys[7],13,8,0,3,1,BOTTOM); //Bottom Qud
		
		Triangulate(&ship->tail.polys[8],&ship->tail.polys[9],2,11,12,3,1,DIFFUSE1); //Right Quad
		CreateTriangle(&ship->tail.polys[10],2,10,11,1,DIFFUSE2); //Left Tris Top
		CreateTriangle(&ship->tail.polys[11],13,3,12,1,DIFFUSE2); //Left Tris Bot
		
		CreateTriangle(&ship->tail.polys[12],1,6,7,1,DIFFUSE2); //Left Tris Top
		CreateTriangle(&ship->tail.polys[13],2,9,10,1,DIFFUSE2); //Left Tris Top
		
		//Triangulate(&ship->tail.polys[12],&ship->tail.polys[13],6,7,9,10,1); //Back Top 
		Triangulate(&ship->tail.polys[14],&ship->tail.polys[15],7,8,13,9,1,BOTTOM); //Back Bot
		
		TriangleFan(&ship->tail,16,14,4,8,-1,EXHAUST); //Left Exhaust
		TriangleFan(&ship->tail,21,15,9,13,-1,EXHAUST); //Right Exhaust
		
	}
	
}

void ShipOneMonoTail(shipOne_t* ship, shipOneParams_t params){
	
	//TAIL---------------------------------------------------------------------------------------------------
	printf("Building tail: standard single exhaust (big)\n");
	
	ship->tail.ntris=26; //26
	ship->tail.nverts=16; //16
	
	ship->tail.verts=malloc(ship->tail.nverts*sizeof(vert_t));
	ship->tail.polys=malloc(ship->tail.ntris*sizeof(poly_t));
	
	if(ship->tail.verts == NULL || ship->tail.polys == NULL ){
		fprintf(stderr,"Failed to allocate ship tail memory.\n");
		exit(-1);
	}
	
	ship->tail.verts[0]=ship->mid.verts[8];
	ship->tail.verts[1]=ship->mid.verts[16];
	ship->tail.verts[2]=ship->mid.verts[17];
	ship->tail.verts[3]=ship->mid.verts[11];
	
	
	ship->tail.verts[4].x = params.tailWid*0.5f+params.exhRadius; ship->tail.verts[4].y = -params.mainLen*0.5f; ship->tail.verts[4].z = params.exhRadius;
	ship->tail.verts[5].x = params.tailWid*0.5f; ship->tail.verts[5].y = -params.mainLen*0.5f; ship->tail.verts[5].z = 2*params.exhRadius;
	ship->tail.verts[8].x = params.tailWid*0.5f; ship->tail.verts[8].y = -params.mainLen*0.5f; ship->tail.verts[8].z = 0.0f;
	
	ship->tail.verts[11].x = -ship->tail.verts[5].x; ship->tail.verts[11].y = ship->tail.verts[5].y; ship->tail.verts[11].z = ship->tail.verts[5].z;
	ship->tail.verts[12].x = -ship->tail.verts[4].x; ship->tail.verts[12].y = ship->tail.verts[4].y; ship->tail.verts[12].z = ship->tail.verts[4].z;
	ship->tail.verts[13].x = -ship->tail.verts[8].x; ship->tail.verts[13].y = ship->tail.verts[8].y; ship->tail.verts[13].z = ship->tail.verts[8].z;
	
	ship->tail.verts[14].x = params.tailWid*0.5f; ship->tail.verts[14].y = -params.mainLen*0.5f+2*params.exhRadius; ship->tail.verts[14].z = params.exhRadius;
	ship->tail.verts[6].x = params.tailWid*0.5f-0.5f*params.exhRadius; ship->tail.verts[6].y = -params.mainLen*0.5f+2*params.exhRadius; ship->tail.verts[6].z = params.exhRadius+0.5f*params.exhRadius;
	ship->tail.verts[7].x = params.tailWid*0.5f-0.5f*params.exhRadius; ship->tail.verts[7].y = -params.mainLen*0.5f+2*params.exhRadius; ship->tail.verts[7].z = params.exhRadius-0.5f*params.exhRadius;
	
	ship->tail.verts[15].x = -ship->tail.verts[14].x; ship->tail.verts[15].y = ship->tail.verts[14].y; ship->tail.verts[15].z = ship->tail.verts[14].z;
	ship->tail.verts[10].x = -ship->tail.verts[6].x; ship->tail.verts[10].y = ship->tail.verts[6].y; ship->tail.verts[10].z = ship->tail.verts[6].z;
	ship->tail.verts[9].x = -ship->tail.verts[7].x; ship->tail.verts[9].y = ship->tail.verts[7].y; ship->tail.verts[9].z = ship->tail.verts[7].z;
	
	
	Triangulate(&ship->tail.polys[0],&ship->tail.polys[1],5,1,0,4,1,DIFFUSE1); //Left Quad
	Triangulate(&ship->tail.polys[2],&ship->tail.polys[3],1,5,11,2,1,MECHANICS); //Top Quad
	Triangulate(&ship->tail.polys[4],&ship->tail.polys[5],3,2,11,12,1,DIFFUSE1); //Right Quad
	Triangulate(&ship->tail.polys[6],&ship->tail.polys[7],0,3,13,8,1,BOTTOM); //Bottom Quad
	CreateTriangle(&ship->tail.polys[8],0,8,4,1,DIFFUSE2); //Left Bottom Side
	CreateTriangle(&ship->tail.polys[9],3,12,13,1,DIFFUSE2); //Right Bottom Side
	
	Triangulate(&ship->tail.polys[10],&ship->tail.polys[11],6,7,9,10,1,EXHAUST_INNER); //ExhaustQuad
	CreateTriangle(&ship->tail.polys[12],14,7,6,1,EXHAUST_INNER); //Left ExhaustBit
	CreateTriangle(&ship->tail.polys[13],9,15,10,1,EXHAUST_INNER); //Left ExhaustBit	
	
	Triangulate(&ship->tail.polys[14],&ship->tail.polys[15],6,10,11,5,1,EXHAUST); //Exhaust Top Quad
	Triangulate(&ship->tail.polys[16],&ship->tail.polys[17],7,8,13,9,1,EXHAUST); //Exhaust Bot Quad
	
	Triangulate(&ship->tail.polys[18],&ship->tail.polys[19],14,6,5,4,1,EXHAUST); //Exhaust Top Left Quad
	Triangulate(&ship->tail.polys[20],&ship->tail.polys[21],10,15,12,11,1,EXHAUST); //Exhaust Top Right Quad
	
	Triangulate(&ship->tail.polys[22],&ship->tail.polys[23],14,4,8,7,1,EXHAUST); //Exhaust Bot Left Quad
	Triangulate(&ship->tail.polys[24],&ship->tail.polys[25],15,9,13,12,1,EXHAUST); //Exhaust Bot Right Quad
	//TriangleFan(&ship->tail,10,14,4,8,1); //Left Exhaust
	//TriangleFan(&ship->tail,21,15,9,13,1); //Right Exhaust
		
	
}

