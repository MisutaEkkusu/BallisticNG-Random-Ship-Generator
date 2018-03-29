#ifndef _SHIPGENMATHS_H_
#define _SHIPGENMATHS_H_

#include <stdint.h>
#include <math.h>

float degrad;

#define TEXTURE_WIDTH 256
#define TEXTURE_HEIGHT 256

//DEFINE UV OFFSETS FOR TEXTURE TYPES
//CONSIDERS U,V = 0,0 AS TOP LEFT CORNER OF TEXTURE
//BLENDER CONSIDERS U,V = 0,0 AS BOTTOM LEFT CORNER

#define DIFFUSE1_UOFFSET 0
#define DIFFUSE1_VOFFSET 0
#define DIFFUSE1_USIZE 128
#define DIFFUSE1_VSIZE 128

#define DIFFUSE2_UOFFSET 128
#define DIFFUSE2_VOFFSET 0
#define DIFFUSE2_USIZE 128
#define DIFFUSE2_VSIZE 128

#define BOTTOM_UOFFSET 0
#define BOTTOM_VOFFSET 128
#define BOTTOM_USIZE 64
#define BOTTOM_VSIZE 128

#define MECHANICS_UOFFSET 64
#define MECHANICS_VOFFSET 128
#define MECHANICS_USIZE 64
#define MECHANICS_VSIZE 128

#define GLASS_UOFFSET 128
#define GLASS_VOFFSET 128
#define GLASS_USIZE 64
#define GLASS_VSIZE 64

#define INTAKE_UOFFSET 128
#define INTAKE_VOFFSET 192
#define INTAKE_USIZE 64
#define INTAKE_VSIZE 64

#define EXHAUST_UOFFSET 192
#define EXHAUST_VOFFSET 128
#define EXHAUST_USIZE 64
#define EXHAUST_VSIZE 64

#define EXHAUST_INNER_UOFFSET 192
#define EXHAUST_INNER_VOFFSET 192
#define EXHAUST_INNER_USIZE 64
#define EXHAUST_INNER_VSIZE 64

#define NUMTEXTURES 8
#define NUMSUBPARTS 4

typedef enum {DIFFUSE1, DIFFUSE2, BOTTOM, MECHANICS, GLASS, EXHAUST, EXHAUST_INNER, INTAKE} texType_t;
typedef enum {NOSE, MID, TAIL, WINGS} partType_t;

typedef struct vert{
	
	float x;
	float y;
	float z;
	
}vert_t;

typedef struct uv{
	
	float u;
	float v;
	
}uv_t;

typedef struct poly{

	uint16_t vert[4]; //vert[3]==0xFFFF if triangle (deprecated)
	uint16_t uvindx[3];
	
	vert_t* vptr[3];
	
	uv_t* uvptr[3];
	
	int16_t zbuf;
	uint8_t uvmapped;
	
	partType_t polysubgrp;
	texType_t type; //0 - main body color #1; 1 - main body color #2; 2 - airbrake; 3 - mechanical parts; 4 - underside; 5 - exhaust interior  
	
}poly_t;

typedef struct mesh{
	
	uint16_t nverts;
	uint16_t ntris;
	uint16_t nquads;
	uint16_t nuvs;
	
	vert_t *verts;
	poly_t *polys;
	uv_t* uvs;
	
}mesh_t;

typedef struct xyzbounds{
	
	float xmax;
	float xmin;
	float ymax;
	float ymin;
	float zmax;
	float zmin;
	
}xyzbounds_t;

float lerp(float t, float a, float b);
float max(mesh_t* mesh, char axis);
float min(mesh_t* mesh, char axis);

vert_t TranslateVert(vert_t vert, float x, float y, float z);
vert_t VLerp(float t, vert_t a, vert_t b);
uint8_t VertexCoincident(vert_t v1, vert_t v2);

void CreateTriangle(poly_t* tri1,uint16_t v0, uint16_t v1, uint16_t v2, int8_t dir, texType_t type);
void Triangulate(poly_t* tri1, poly_t* tri2, uint16_t v0, uint16_t v1, uint16_t v2, uint16_t v3, int8_t dir, texType_t type);
void NGonize(mesh_t *mesh, uint16_t v0i, uint16_t vni, float radius, float angleoffset, float xoffset, float yoffset, float zoffset);
void TriangleFan(mesh_t *mesh, uint16_t triIndx0, uint16_t vc0, uint16_t v0i, uint16_t vni, uint8_t dir, texType_t type);

#endif
