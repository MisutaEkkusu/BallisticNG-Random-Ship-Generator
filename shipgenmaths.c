#include "shipgenmaths.h"


float lerp(float t, float a, float b){
	/*
		Linear Interpolation
	*/
	return a*(1-t) + b*t;
	
}



vert_t TranslateVert(vert_t vert, float x, float y, float z){
	vert_t ret;
	
	ret.x=vert.x+x;
	ret.y=vert.y+y;
	ret.z=vert.z+z;
	
	return ret;
	
}

vert_t MirrorVert(vert_t vert, char axis){
	
	switch(axis){
		
		case 'x':
			vert.x = -vert.x;
			break;
			
		case 'y':
			vert.y = -vert.y;
			break;
			
		case 'z':
			vert.z = -vert.z;
			break;
			
		default:
		
			break;
		
	}
	return vert;
	
}

vert_t ScaleVert(vert_t vert, float sx, float sy, float sz){
	vert_t ret;
	
	ret.x=vert.x*sx;
	ret.y=vert.y*sy;
	ret.z=vert.z*sz;
	
	return ret;
	
}

void CreateTriangle(poly_t* tri1,uint16_t v0, uint16_t v1, uint16_t v2, int8_t dir, texType_t type){
	/*
		Generate a triangle from 3 vertices
		dir toggles between clockwise and anti-clockwise vertex indexing for future face normal calculation.
		
	*/
	if(dir>=0){
		tri1->vert[0]=v0; tri1->vert[1]=v1; tri1->vert[2]=v2;
	}
	else if (dir<0){
		tri1->vert[0]=v0; tri1->vert[1]=v2; tri1->vert[2]=v1;	
	}
	tri1->vert[3]=0xFFFF;
	tri1->type = type;
}

void Triangulate(poly_t* tri1, poly_t* tri2, uint16_t v0, uint16_t v1, uint16_t v2, uint16_t v3, int8_t dir, texType_t type){
	/*
		Triangulate a group of 4 vertices
	*/
	if(dir>=0){
		tri1->vert[0]=v3; tri1->vert[1]=v0; tri1->vert[2]=v1;
		tri2->vert[0]=v2; tri2->vert[1]=v3; tri2->vert[2]=v1;	
	}
	else if (dir<0){
		tri1->vert[0]=v0; tri1->vert[1]=v3; tri1->vert[2]=v1;
		tri2->vert[0]=v3; tri2->vert[1]=v2; tri2->vert[2]=v1;	
	}
	tri1->vert[3]=0xFFFF;
	tri2->vert[3]=0xFFFF;
	
	tri1->type = type;
	tri2->type = type;
}

vert_t VLerp(float t, vert_t a, vert_t b){
	
	/*
		Linear Interpolation between two vertices. Returns the vertex coordinates at "t" along "ab".
	*/
	
	vert_t ret={0.0f};
	
	ret.x=lerp(t,a.x,b.x);
	ret.y=lerp(t,a.y,b.y);
	ret.z=lerp(t,a.z,b.z);
	
	return ret;
	
}

void NGonize(mesh_t *mesh, uint16_t v0i, uint16_t vni, float radius, float angleoffset, float xoffset, float yoffset, float zoffset){
	
	/*
	 Places vertices "v0i" through to "vni" in "mesh", such that they form a regular polygon at coordinates (xoffset,yoffset,zoffset) in the XZ plane with radius "radius" and rotated by "angleoffset"
	*/
	
	uint16_t nsides=0, i=0, t=0;

	if(v0i > vni){
		uint16_t temp=v0i;
		v0i=vni;
		vni=temp;
	}
	
	else if(v0i == vni){
		return;
	}
	nsides = vni-v0i + 1;
	
	for(i=v0i;i<=vni;i++){
		t=i-v0i;
		mesh->verts[i].x=radius*cos(t*2.0f*M_PI/(1.0f*nsides)-angleoffset)+xoffset;
		mesh->verts[i].z=radius*sin(t*2.0f*M_PI/(1.0f*nsides)-angleoffset)+zoffset;
		mesh->verts[i].y=yoffset;
	}
	
}

void TriangleFan(mesh_t *mesh, uint16_t triIndx0, uint16_t vc0, uint16_t v0i, uint16_t vni, uint8_t dir, texType_t type){
	
	/*
		Joins vertices "v0i" through to "vni" around a central vertex "vc0" with triangles.
		CAUTION: THIS ROUTINE DOES NOT CHECK IF THE CENTRAL VERTEX IS INDEED INSIDE THE v0i->vni NGON.
	*/
	uint16_t indx=0, triIndx=triIndx0;
	
	if(v0i > vni){
		uint16_t temp=v0i;
		v0i=vni;
		vni=temp;
	}
	
	else if(v0i == vni){
		return;
	}	
	
	for(indx=v0i;indx<vni;indx++){
		
		CreateTriangle(&mesh->polys[triIndx],vc0,indx,indx+1,dir,type);
		triIndx++;
		
	}
		CreateTriangle(&mesh->polys[triIndx],vc0,vni,v0i,dir,type);
		
}

uint8_t VertexCoincident(vert_t v1, vert_t v2){
	
	if(v1.x == v2.x && v1.y == v2.y && v1.z == v2.z)
		return 1;
	else
		return 0;
	
}

float max(mesh_t* mesh, char axis){
	
	float ret = -10000.0f;
	uint32_t i=0;
	
	switch(axis){
		case 'x':
			for(i=0;i<mesh->nverts;i++){
				if(mesh->verts[i].x>=ret){
					ret = mesh->verts[i].x;
				}
			}
			break;
			
		case 'y':
			for(i=0;i<mesh->nverts;i++){
				if(mesh->verts[i].y>=ret){
					ret = mesh->verts[i].y;
				}
			}
			break;
			
		case 'z':
			for(i=0;i<mesh->nverts;i++){
				if(mesh->verts[i].z>=ret){
					ret = mesh->verts[i].z;
				}
			}
			break;
		default:
			
			break;
	}
	return ret;
}

float min(mesh_t* mesh, char axis){
	
	float ret = 10000.0f;
	uint32_t i=0;
	
	switch(axis){
		case 'x':
			for(i=0;i<mesh->nverts;i++){
				if(mesh->verts[i].x<=ret){
					ret = mesh->verts[i].x;
				}
			}
			break;
			
		case 'y':
			for(i=0;i<mesh->nverts;i++){
				if(mesh->verts[i].y<=ret){
					ret = mesh->verts[i].y;
				}
			}
			break;
			
		case 'z':
			for(i=0;i<mesh->nverts;i++){
				if(mesh->verts[i].z<=ret){
					ret = mesh->verts[i].z;
				}
			}
			break;
		default:
			
			break;
	}
	return ret;
}


vert_t CalcNormalTri(poly_t tri){
	
	vert_t ret = {0.0f};
	
	vert_t v1, v2;
	
	v1.x = (*tri.vptr[1]).x - (*tri.vptr[0]).x;
	v1.y = (*tri.vptr[1]).y - (*tri.vptr[0]).y;
	v1.z = (*tri.vptr[1]).z - (*tri.vptr[0]).z;
	
	v2.x = (*tri.vptr[2]).x - (*tri.vptr[0]).x;
	v2.y = (*tri.vptr[2]).y - (*tri.vptr[0]).y;
	v2.z = (*tri.vptr[2]).z - (*tri.vptr[0]).z;

	ret.x = v1.y*v2.z - v2.y*v1.z;
	ret.y = v1.z*v2.x - v2.z*v1.x;
	ret.z = v1.x*v2.y - v2.x*v1.y;
	
	return ret;
}
