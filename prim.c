#include "prim.h"

void MeshToObj(mesh_t* mesh, char* filename){
	
	FILE* fp=NULL;
	uint32_t i=0;
	fp=fopen(filename,"w+");
	
	if(fp==NULL){
		fprintf(stderr,"Failed to create .obj file.\n");
		return;
	}
	printf("Saving...\n");
	fprintf(fp,"#Made with Xpand's RNG ship generator v1.0\n");
	for(i=0;i<mesh->nverts;i++){
		//fprintf(fp,"#Vertex %d (or %d)\n",i,i+1);
		fprintf(fp,"v %f %f %f\n",-mesh->verts[i].x,mesh->verts[i].z,-mesh->verts[i].y);
	}
	fprintf(fp,"#%d Vertices\n",mesh->nverts);
	for(i=0;i<mesh->nuvs;i++){
		fprintf(fp,"vt %f %f\n",mesh->uvs[i].u,1.0f-mesh->uvs[i].v);
		
	}
	fprintf(fp,"#%d UVs\n",mesh->nuvs);
	fprintf(fp,"g RNGShip\n");
	for(i=0;i<mesh->ntris;i++){
		//fprintf(fp,"#Triangle %d (or %d)\n",i,i+1);
		
		fprintf(fp,"f %d/%d %d/%d %d/%d\n",mesh->polys[i].vert[0]+1,mesh->polys[i].uvindx[0]+1,mesh->polys[i].vert[1]+1,mesh->polys[i].uvindx[1]+1,mesh->polys[i].vert[2]+1,mesh->polys[i].uvindx[2]+1);
	}
	fprintf(fp,"#%d Triangles\n",mesh->ntris);
	fclose(fp);
	printf("Save Successful!\n");
	//system("PAUSE");
	//system("cls");
	
}

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
	
	params->isLongWing=rand()%2; //GENERATE FEISAR WINGS
	params->isDualEngine=rand()%3; //Generate Twin Engine
	printf("Done!\n");

}

void MakeShipOne(shipOne_t* ship, shipOneParams_t params){
	
	uint32_t i=0;
	//float tgtWidth=((0.5f*params.mainLen+params.wingPos)/(0.5f*params.mainLen))/(0.5f*(params.mainWid))+0.5f*params.noseWidBot; //Used to calculate the x for vertices at y=0.0f
	
	float tgtWidth=lerp((0.5f*params.mainLen)/(0.5f*params.mainLen-params.wingPos),0.5f*params.noseWidBot,0.5f*params.mainWid);
	
	printf("Generating ship mesh...\n");
	
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
	
	//WINGS-----------------------------------------------------------------------------------------------
	ship->wings.ntris=8; //12
	ship->wings.nverts=16; //16

	if(params.isLongWing){
		ship->wings.ntris+=4;
		ship->wings.nverts+=4;	
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
	
	if(params.isLongWing){
		
		ship->wings.verts[16]=ship->mid.verts[0];
		ship->wings.verts[17]=ship->nose.verts[0];
		ship->wings.verts[18]=ship->mid.verts[3];
		ship->wings.verts[19]=ship->nose.verts[3];
		
		Triangulate(&ship->wings.polys[8],&ship->wings.polys[9],0,16,17,1,1,DIFFUSE2);
		Triangulate(&ship->wings.polys[10],&ship->wings.polys[11],19,18,4,5,1,DIFFUSE2);
	}

	
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



void ShipOneTwinTail(shipOne_t* ship, shipOneParams_t params){
	
	//TAIL---------------------------------------------------------------------------------------------------
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

void ShipToObj(shipOne_t ship){
	
	//MeshToObj(&ship.nose,"shipNose.obj");
	//MeshToObj(&ship.mid,"shipMid.obj");
	//MeshToObj(&ship.wings,"shipWings.obj");
	//MeshToObj(&ship.tail,"shipTail.obj");
	MeshToObj(&ship.obj,"shipOBJ.obj");
	
}

void FreeShip(shipOne_t* ship){
	printf("Freeing memory...\n");
	free(ship->nose.verts);
	free(ship->nose.polys);
	free(ship->nose.uvs);

	free(ship->mid.verts);
	free(ship->mid.polys);
	free(ship->mid.uvs);
	
	free(ship->wings.verts);
	free(ship->wings.polys);
	free(ship->wings.uvs);
	
	free(ship->tail.verts);
	free(ship->tail.polys);	
	free(ship->tail.uvs);	
	
	free(ship->obj.verts);
	free(ship->obj.polys);
	free(ship->obj.uvs);
	printf("Done!\n");
}

uint8_t MergeMesh(mesh_t m1, mesh_t m2, mesh_t* ret, uint8_t noMerge){
	
	uint32_t i=0, j=0, k=0, nCverts=0, rmindx = 0, numReplaced = 0;
	uint32_t replTriIndx[3] = {0xFFFFFFFF};
 	uint8_t vertCopy=0, incIndx=0; //bool to copy vertex
	
	uint32_t totnpolys=m1.ntris+m2.ntris; //max number of triangles
	uint32_t totnverts=m1.nverts+m2.nverts; //max number of vertices
	
	uint32_t cVertList[32][2]; //coincident vertex list. Column 0 is the vertex index in mesh1, Column 1 is the vertex index in mesh2
	
	mesh_t retmesh = {0}; //return mesh_t
	
	printf("Merging meshes...\n");
	
	retmesh.ntris=totnpolys;
	retmesh.nverts=totnverts;
	retmesh.nquads=0;
	
	for(i=0;i<m1.nverts;i++){
		for(j=0;j<m2.nverts;j++){
			//check if two vertices in both meshes are coincident
			if(VertexCoincident(m1.verts[i],m2.verts[j])){
				
				cVertList[nCverts][0]=i; //save vertex to be used
				cVertList[nCverts][1]=j; //save vertex to be discarded
				
				nCverts++; //increment the number of coincident vertices
				
				if(nCverts>=32){
					
					fprintf(stderr,"ERROR: nCvert index out of range!\n");
					break;
					
				}
			}
			
		}
	}
	
	if(noMerge)	nCverts = 0; //ignore coincident vertices
	
	//Add exception for merging non overlapping meshes
	if(nCverts == 0){
		//fprintf(stderr,"Vertex Merging disabled!\n");
		retmesh.verts = calloc(retmesh.nverts,sizeof(vert_t));
		retmesh.polys = calloc(retmesh.ntris,sizeof(poly_t));
		if(retmesh.verts == NULL || retmesh.polys == NULL){
			fprintf(stderr,"Failed to allocate temp mesh memory...\n");
			return 0;
		}
		memcpy(retmesh.verts,m1.verts,m1.nverts*sizeof(vert_t)); //copy mesh1's vertex data onto the retmesh vertex array
		memcpy(retmesh.polys,m1.polys,m1.ntris*sizeof(poly_t)); //copy mesh1's triangle data onto the retmesh triangle array
		
		memcpy(retmesh.verts+m1.nverts,m2.verts,m2.nverts*sizeof(vert_t)); //copy mesh1's vertex data onto the retmesh vertex array
		
		//COPY MESH2's TRIANGLES.
		for(i=0;i<m2.ntris;i++){
			retmesh.polys[i+m1.ntris] = m2.polys[i];
			for(j=0;j<3;j++){
				retmesh.polys[i+m1.ntris].vert[j] += m1.nverts;
			}	
		}
		
		ret->verts  = retmesh.verts;
		ret->polys  = retmesh.polys;
		ret->nverts = retmesh.nverts;
		ret->ntris  = retmesh.ntris;
		ret->nquads = 0;

		return 1;
	}
	//printf("Found %d coincident vertices.\n",nCverts);
	
	retmesh.nverts = retmesh.nverts - nCverts;
	//printf("Creating mesh with %d vertices and %d polys.\n",retmesh.nverts,retmesh.ntris);
	
	retmesh.verts = calloc(retmesh.nverts,sizeof(vert_t));
	retmesh.polys = calloc(retmesh.ntris,sizeof(poly_t));
	
	if(retmesh.verts == NULL || retmesh.polys == NULL){
		fprintf(stderr,"Failed to allocate temp mesh memory...\n");
		return 0;
	}
	
	memcpy(retmesh.verts,m1.verts,m1.nverts*sizeof(vert_t)); //copy mesh1's vertex data onto the retmesh vertex array
	memcpy(retmesh.polys,m1.polys,m1.ntris*sizeof(poly_t)); //copy mesh1's triangle data onto the retmesh triangle array
	
	//COPY MESH2 VERTICES. CULL OVERLAPPING/SHARED VERTICES
	for(i=0;i<m2.nverts;i++){
		
		for(j=0;j<nCverts;j++){
			if(i == cVertList[j][1]){
				vertCopy=0;
				break; //if the current vertex is in the remove list, break out of the loop without copying and carry on to the next vertex
			}
			else{
				vertCopy = 1; //else enable copy
			}
		}
		if(vertCopy){
			retmesh.verts[rmindx+m1.nverts] = m2.verts[i]; //copy mesh2's vertex to index+offset
			rmindx++;
			vertCopy=0; //reset boolean
		}
		
	}
	
	//COPY MESH2's TRIANGLES. CHANGE VERTEX INDEX TO USE MESH1's ON SHARED VERTICES
	for(i=0;i<m2.ntris;i++){
		
		retmesh.polys[i+m1.ntris] = m2.polys[i];
		numReplaced=0;
		for(j=0;j<3;j++){
			retmesh.polys[i+m1.ntris].vert[j] += m1.nverts-nCverts;
		
			for(k=0;k<nCverts;k++){
				if(m2.polys[i].vert[j] == cVertList[k][1]){
					retmesh.polys[i+m1.ntris].vert[j] = cVertList[k][0]; //replace MESH2's vertex index with MESH1's vertex index
					numReplaced++;
					replTriIndx[numReplaced] = j;
					//break;
				}
			}
			

		}

		
	}
	
	ret->verts  = retmesh.verts;
	ret->polys  = retmesh.polys;
	ret->nverts = retmesh.nverts;
	ret->ntris  = retmesh.ntris;
	ret->nquads = 0;

	printf("Done!\n");
	return 1;
}
