#ifndef _UVOPS_H_
#define _UVOPS_H_

#include "prim.h"


//UV OPS
xyzbounds_t TriangleBounds(poly_t tri, vert_t* verts);

uint32_t* GenerateUVPolyGroup(mesh_t* shipmesh,texType_t type, uint16_t* n_poly_added);
uint32_t* GenerateSubGroup(mesh_t* shipmesh, uint32_t* UVgroup, uint16_t nuvpolys ,partType_t ID, uint16_t* n_poly_added);
xyzbounds_t GetPolyGroupBounds(mesh_t* shipmesh, uint32_t* polygroup, uint16_t npolys);
uvProject_t GetBestUVProjection(mesh_t* mesh, uint32_t* polygroup, uint16_t npolys);
void XYZ2UV_triangle2(poly_t* tri, mesh_t* mesh, xyzbounds_t mdlbounds, xyzbounds_t uvbounds, uvProject_t UVP);
void ProjectMapUVGroup(mesh_t* shipmesh, uint32_t* uvgrp, uint16_t npolys, uvProject_t projplane, xyzbounds_t bounds);
void GenerateUVs(mesh_t* shipmesh);

#endif
