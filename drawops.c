#include "drawops.h"

#define SW 80 //80
#define SH 38 //20
#define SSIZE SW*SH

#define Q_KEY_CODE 0x51
#define S_KEY_CODE 0x53
#define sgn(x) x<0 ? -1 : 1

uint8_t buffer[SSIZE];

int16_t *OT;

void BltText(char* text, uint16_t x, uint16_t y){
	
	uint16_t offset=(x+y*(SW));
	uint16_t length=strlen(text), i=0;
	//if(offset+length<SSIZE)
	memcpy(buffer+offset,text,length);

}


void DrawMesh(mesh_t mesh, float pitch, float yaw, float scale){
	
	uint16_t i=0, j=0, k=0, nx=0, ny=0, vtr=0;
	vert_t orig, mod;
	
	int16_t sx[3], sy[3];

	//Transform to screen
	for(i=0;i<mesh.ntris;i++){
		mesh.polys[i].zbuf=0;
		for(vtr=0;vtr<3;vtr++){
			
			orig=TranslateVert(mesh.verts[mesh.polys[i].vert[vtr]],0,0,0);
			mod=orig;
		
			mod.y=cos(pitch)*orig.y-sin(pitch)*orig.z;
			mod.z=sin(pitch)*orig.y+cos(pitch)*orig.z;
			orig=mod;
			mod.x=cos(yaw)*orig.x-sin(yaw)*orig.z;
			mod.z=sin(yaw)*orig.x+cos(yaw)*orig.z;
			
			
			sx[vtr]=(int16_t)(scale*(mod.x))+(SW>>1);		
			sy[vtr]=((int16_t)(scale*(mod.y))>>1)+(SH>>1);
			//mesh.polys[i].zbuf+=(int16_t)(mod.z*100);
			
			
		}
		//printf("%d\n",mesh.polys[i].zbuf);
	/*
		for(j=0;j<i;j++){
			
			if(mesh.polys[i].zbuf>mesh.polys[j].zbuf){

				OT[i]=j;
			}
			
		}*/
		
		DrawLine(sx[0],sy[0],sx[1],sy[1],219);
		DrawLine(sx[1],sy[1],sx[2],sy[2],219);
		DrawLine(sx[0],sy[0],sx[2],sy[2],219);
		/*
		PutPixel(sx[0],sy[0],'+');
		PutPixel(sx[1],sy[1],'+');
		PutPixel(sx[2],sy[2],'+');
	*/
	}
	for(i=SW-1;i<SSIZE;i+=SW){
		buffer[i]='\n';
	}
	buffer[SSIZE-1]=0;
	
}

void ClrScr(uint8_t col){
	uint16_t i=0;
	
	memset(buffer,col,SSIZE);
	for(i=SW-1;i<SSIZE;i+=SW){
		buffer[i]='\n';
	}
	buffer[SSIZE-1]=0;
	
}

void RenderBuffer(){
	
	printf("%s\n",buffer);
	
}

void RenderLoop(){
	uint8_t quit=0, xoff=0;
	uint16_t i=0;
	srand(time(NULL));
	//cursorPos.X=0;
	//cursorPos.Y=0;
	float yaw=0, pitch=M_PI/2.0f, angInc=M_PI/100.0f, scale=0.05f*SW;

	float t=0.0f, dt=0.02f;
	
	//hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	//system("cls");
	/*OT=malloc(mainMesh->ntris*sizeof(int16_t));
	memset(OT,0,sizeof(int16_t)*mainMesh->ntris);
	*/
	while(!quit){
		//SetConsoleCursorPosition(hConsole,cursorPos);
		

		ClrScr(' ');
		/*DrawMesh(ship.nose,pitch,yaw,scale);
		DrawMesh(ship.mid,pitch,yaw,scale);
		DrawMesh(ship.wings,pitch,yaw,scale);
		DrawMesh(ship.tail,pitch,yaw,scale);
		*/
		DrawMesh(ship.obj,pitch,yaw,scale);
		
		BltText("SHIP GENERATOR 1.0", 0,0);
		
		RenderBuffer();
/*
		if(GetAsyncKeyState(Q_KEY_CODE)){
			quit=1;
		}
		
		if(GetAsyncKeyState(VK_LEFT))
			yaw+=angInc;
			
		if(GetAsyncKeyState(VK_RIGHT))
			yaw-=angInc;
		
		if(GetAsyncKeyState(VK_UP))
			pitch-=angInc;
			
		if(GetAsyncKeyState(VK_DOWN))
			pitch+=angInc;	
		
		if(GetAsyncKeyState(VK_ADD))
			scale+=0.1f;	
		
		if(GetAsyncKeyState(VK_SUBTRACT))
			scale-=0.1f;
			
		if(GetAsyncKeyState(VK_SPACE) & 0x80 !=0){
			//free(mainMesh->verts);
			//free(mainMesh->polys);
			//MakeWOShip(mainMesh);
			//SetMainMesh(mainMesh);
			FreeShip(&ship);
			GenShipOneParams(&shipParams);
			MakeShipOne(&ship,shipParams);
		}
		
		if(GetAsyncKeyState(S_KEY_CODE) & 0x80 !=0){
			//MeshToObj(mainMesh, "ship.obj");
			ShipToObj(ship);
		}
			
		Sleep(10);*/
	}
	free(OT);
}

void SetMainMesh(mesh_t *mesh){
	
	mainMesh=mesh;

}

void PutPixel(uint16_t x, uint16_t y, uint8_t col){
	if(x<SW && y<SH)
		buffer[x+y*SW]=col;
	
}

void DrawLine(int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint8_t col)
{
	int16_t i=0,j=0,dx=0,dy=0,sdx=0,sdy=0,dxabs=0,dyabs=0,x=0,y=0,px=0,py=0;
	
	if(x1>x2 || y1>y2){
		
		int16_t temp=x2;
		x2=x1;
		x1=temp;
		temp=y2;
		y2=y1;
		y1=temp;
		
	}
	
	dx=x2-x1;
	dy=y2-y1;
	
	if(dy==0){
	
		for(i=x1;i<x2;i++){
			PutPixel(i,y1,col);
		}
		return;
	}
	
	else if(dx==0){
		
		for(i=y1;i<y2;i++){
			PutPixel(x1,i,col);
		}
		return;
	}

	dxabs=abs(dx);
	dyabs=abs(dy);
	sdx=sgn(dx);
	sdy=sgn(dy);
	x=dyabs>>1;
	y=dxabs>>1;
	px=x1;
	py=y1;
	
	PutPixel(px,py,col);

	if (dxabs>=dyabs)
	{
		for(i=0;i<dxabs;i++)
		{
			y+=dyabs;
			if (y>=dxabs)
			{
				y-=dxabs;
				py+=sdy;
			}
			px+=sdx;

			PutPixel(px,py,col);
			
		}
	}
	else
	{
		for(i=0;i<dyabs;i++)
		{
			x+=dxabs;
				if (x>=dyabs)
				{
					x-=dyabs;
					px+=sdx;
				}
				py+=sdy;
			
			PutPixel(px,py,col);
		}
	}
 
}
