#include "UVOPS.h"

//UV MAPPING OPS

xyzbounds_t TriangleBounds(poly_t tri, vert_t* verts){
	
	uint32_t i=0;
	vert_t v[3];
	xyzbounds_t bound = {0};
	float limit = 10000.0f;
	
	bound.xmax = -limit;
	bound.xmin = limit;
	
	bound.ymax = -limit;
	bound.ymin = limit;
	
	bound.zmax = -limit;
	bound.zmin = limit;
	

	for(i=0;i<3;i++){
		v[i] = verts[tri.vert[i]];
	}

	for(i=0;i<3;i++){
		if(v[i].x>=bound.xmax){
			bound.xmax = v[i].x;
		}
		if(v[i].x<=bound.xmin){
			bound.xmin = v[i].x;
		}
		
		if(v[i].y>=bound.ymax){
			bound.ymax = v[i].y;
		}
		if(v[i].y<=bound.ymin){
			bound.ymin = v[i].y;
		}
		
		if(v[i].z>=bound.zmax){
			bound.zmax = v[i].z;
		}
		if(v[i].z<=bound.zmin){
			bound.zmin = v[i].z;
		}
		
	}

	//printf("Triangle Bounds:\nx:%f %f \ny:%f %f \nz:%f %f\n", bound.xmin,bound.xmax,bound.ymin,bound.ymax,bound.zmin,bound.zmax);
	return bound;
}

xyzbounds_t ModelBounds(mesh_t mesh){
	
	uint32_t i=0;
	
	xyzbounds_t bound = {0};
	float limit = 10000.0f;
	
	bound.xmax = -limit;
	bound.xmin = limit;
	
	bound.ymax = -limit;
	bound.ymin = limit;
	
	bound.zmax = -limit;
	bound.zmin = limit;


	for(i=0;i<mesh.nverts;i++){
		if(mesh.verts[i].x>=bound.xmax){
			bound.xmax = mesh.verts[i].x;
		}
		if(mesh.verts[i].x<=bound.xmin){
			bound.xmin = mesh.verts[i].x;
		}
		
		if(mesh.verts[i].y>=bound.ymax){
			bound.ymax = mesh.verts[i].y;
		}
		if(mesh.verts[i].y<=bound.ymin){
			bound.ymin = mesh.verts[i].y;
		}
		
		if(mesh.verts[i].z>=bound.zmax){
			bound.zmax = mesh.verts[i].z;
		}
		if(mesh.verts[i].z<=bound.zmin){
			bound.zmin = mesh.verts[i].z;
		}
		
	}
	//printf("Done\n");
	printf("Model Bounds:\nx:%f %f \ny:%f %f \nz:%f %f\n", bound.xmin,bound.xmax,bound.ymin,bound.ymax,bound.zmin,bound.zmax);
	return bound;
}


uint32_t* GenerateUVPolyGroup(mesh_t* shipmesh,texType_t type, uint16_t* n_poly_added){
	uint32_t i = 0, j = 0;
	uint32_t* UVpolygroup = NULL;
	(*n_poly_added) = 0;
	
	
	for(i=0;i<shipmesh->ntris;i++){

		if(shipmesh->polys[i].type == type){
	
			(*n_poly_added)++;
			
		}
	}
	
	if(*n_poly_added==0){
		printf("No triangles found for texture type %d\n",type);
		return NULL;
	}
	
	UVpolygroup = calloc((*n_poly_added),sizeof(uint32_t));
	
	if(UVpolygroup == NULL){
		fprintf(stderr,"ERROR ALLOC\n");
		return NULL;
	}
	
	for(i=0;i<shipmesh->ntris;i++){

		if(shipmesh->polys[i].type == type){
	
			UVpolygroup[j] = i;
			//printf("%d %d\n",i,j);
			j++;

		}
	}
	//printf("Found %u polygons for uv type %u\n",*n_poly_added,type);
	return UVpolygroup;
}


uint32_t* GenerateSubGroup(mesh_t* shipmesh, uint32_t* UVgroup, uint16_t nuvpolys ,partType_t ID, uint16_t* n_poly_added){
	
	uint32_t i = 0, j = 0;
	uint32_t* UVpolygroup = NULL;
	(*n_poly_added) = 0;
	
	for(i=0;i<nuvpolys;i++){

		if(shipmesh->polys[UVgroup[i]].polysubgrp == ID){
		
			(*n_poly_added)++;
			
		}
	}
	if(*n_poly_added==0){
		printf("No triangles found for polysubgrp %d\n",ID);
		return NULL;
	}
	UVpolygroup = calloc((*n_poly_added),sizeof(uint32_t));
	
	if(UVpolygroup == NULL){
		fprintf(stderr,"ERROR ALLOC\n");
		return NULL;
	}
	
	for(i=0;i<nuvpolys;i++){

		if(shipmesh->polys[UVgroup[i]].polysubgrp == ID){
	
			UVpolygroup[j] = UVgroup[i];
			//printf("%d %d\n",i,j);
			j++;

		}
	}
	//printf("Found %u polygons for uv type %u\n",*n_poly_added,type);
	return UVpolygroup;
	
}

xyzbounds_t GetPolyGroupBounds(mesh_t* shipmesh, uint32_t* polygroup, uint16_t npolys){
	
	xyzbounds_t ret = {0};
	uint16_t i = 0, j = 0;
	vert_t v = {0.0f};
	
	float limits = 1000.0f;
	
	ret.xmax = -limits;
	ret.xmin = limits;
	
	ret.ymax = -limits;
	ret.ymin = limits;
	
	ret.zmax = -limits;
	ret.zmin = limits;
	
	for(i=0;i<npolys;i++){
		for(j=0;j<3;j++){
			
			v = *(shipmesh->polys[polygroup[i]].vptr[j]);
			
			if(v.x>=ret.xmax){
				ret.xmax = v.x;
			}
			if(v.x<=ret.xmin){
				ret.xmin = v.x;
			}
			
			if(v.y>=ret.ymax){
				ret.ymax = v.y;
			}
			if(v.y<=ret.ymin){
				ret.ymin = v.y;
			}
			
			if(v.z>=ret.zmax){
				ret.zmax = v.z;
			}
			if(v.z<=ret.zmin){
				ret.zmin = v.z;
			}
				
		}
	}
	
	//printf("UV poly group bounds: %f %f, %f %f, %f %f\n",ret.xmin,ret.xmax,ret.ymin,ret.ymax,ret.zmin,ret.zmax);
	
	return ret;
	
}

uvProject_t GetBestUVProjection(mesh_t* mesh, uint32_t* polygroup, uint16_t npolys){
	
	uvProject_t ret = UVXY;
	
	float totalArea[3] = {0.0f}, triArea[3] = {0.0f}, maxArea = -10000.0f;
	
	vert_t v1, v2, v3;
	
	uint16_t i = 0, j = 0, areaIndx = 0;
	
	for(i=0;i<npolys;i++){

		v1 = *(mesh->polys[polygroup[i]].vptr[0]);
		v2 = *(mesh->polys[polygroup[i]].vptr[1]);
		v3 = *(mesh->polys[polygroup[i]].vptr[2]);
		
		
		triArea[0] = fabs(0.5f*(v1.x*v2.y + v2.x*v3.y + v3.x*v1.y - v2.x*v1.y - v3.x*v2.y - v1.x*v3.y)); //XY
		triArea[1] = fabs(0.5f*(v1.x*v2.z + v2.x*v3.z + v3.x*v1.z - v2.x*v1.z - v3.x*v2.z - v1.x*v3.z)); //XZ
		triArea[2] = fabs(0.5f*(v1.y*v2.z + v2.y*v3.z + v3.y*v1.z - v2.y*v1.z - v3.y*v2.z - v1.y*v3.z)); //YZ
		
		for(j=0;j<3;j++)
			totalArea[j] += triArea[j];
		
	}

	for(i=0;i<3;i++){
		
		if(totalArea[i]>maxArea){
			maxArea = totalArea[i];
			areaIndx = i;
		}
		
	}
	//printf("MaxArea = %f at index %d\n",maxArea,areaIndx);
	switch(areaIndx){
		
		case 0:
			//printf("Projecting UVs in the XY plane.\n");
			ret = UVXY;
			break;
			
		case 1:
			//printf("Projecting UVs in the XZ plane.\n");
			ret = UVXZ;
			break;
			
		case 2:
			//printf("Projecting UVs in the YZ plane.\n");
			ret = UVYZ;
			break;
			
		default:
			fprintf(stderr,"ERROR: Invalid area index! Defaulting to UVXY.\n");
			break;
		
	}
	
	return ret;
	
}

void XYZ2UV_triangle2(poly_t* tri, mesh_t* mesh, xyzbounds_t mdlbounds, xyzbounds_t uvbounds, uvProject_t UVP){
	
	//Check which projection plane (xy,xz,yz) of the triangle has the largest area
	//Project UVs along the direction that yields the largest area of the triangle
	
	float areaval, u0 = uvbounds.xmin, usz = uvbounds.xmax, v0 = uvbounds.ymin, vsz = uvbounds.ymax, uscale = uvbounds.zmin, vscale = uvbounds.zmax;
	uv_t tempverts[3] = {0};
	uint8_t areaindex;
	uint32_t i = 0, j = 0;
	//printf("Area: %f\n",areaval);
		
	switch(UVP){
		
		case UVXY:
			//project along XY
			//printf("Project UVs on XY plane\n");
			for(i=0;i<3;i++){
				tempverts[i].u = mesh->verts[tri->vert[i]].x;
				tempverts[i].v = mesh->verts[tri->vert[i]].y;
				
				tempverts[i].u = (uscale*(tempverts[i].u-mdlbounds.xmin)/(mdlbounds.xmax-mdlbounds.xmin)*usz) + u0 + (0.5f*usz*(1.0f-uscale));
				tempverts[i].v = (vscale*(tempverts[i].v-mdlbounds.ymin)/(mdlbounds.ymax-mdlbounds.ymin)*vsz) + v0 + (0.5f*vsz*(1.0f-vscale));;
			}
			break;
			
		case UVXZ:
			//project along XZ
			for(i=0;i<3;i++){
				tempverts[i].u = mesh->verts[tri->vert[i]].x;
				tempverts[i].v = mesh->verts[tri->vert[i]].z;
				
				tempverts[i].u = (uscale*(tempverts[i].u-mdlbounds.xmin)/(mdlbounds.xmax-mdlbounds.xmin)*usz) + u0 + (0.5f*usz*(1.0f-uscale));
				tempverts[i].v = (vscale*(tempverts[i].v-mdlbounds.zmin)/(mdlbounds.zmax-mdlbounds.zmin)*vsz) + v0 + (0.5f*vsz*(1.0f-vscale));;
			}
			//printf("Project UVs on XZ plane\n");
			break;
			
		case UVYZ:
			for(i=0;i<3;i++){
				tempverts[i].u = mesh->verts[tri->vert[i]].y;
				tempverts[i].v = mesh->verts[tri->vert[i]].z;
				
				tempverts[i].u = (uscale*(tempverts[i].u-mdlbounds.ymin)/(mdlbounds.ymax-mdlbounds.ymin)*usz) + u0 + (0.5f*usz*(1.0f-uscale));
				tempverts[i].v = (vscale*(tempverts[i].v-mdlbounds.zmin)/(mdlbounds.zmax-mdlbounds.zmin)*vsz) + v0 + (0.5f*vsz*(1.0f-vscale));
			}
			//printf("Project UVs on YZ plane\n");
			//project along YZ
			break;
			
		default:
			fprintf(stderr,"ERROR: INVALID AREA INDEX (%d)!\n",areaindex);
			break;
		
	}
	if(mesh->nuvs == 0){
		mesh->nuvs = 1;
		
		mesh->uvs = NULL;
		mesh->uvs = malloc(mesh->nuvs*sizeof(uv_t));
		
		if(mesh->uvs == NULL){
			fprintf(stderr,"ERROR ALLOCATING FIRST UV MEMORY\n");
			return;
		}
	}

	for(j=0;j<3;j++){
		tri->uvmapped = 0x00;
		if(mesh->nuvs>0)
			for(i=0;i<mesh->nuvs;i++){
				if(tempverts[j].u == mesh->uvs[i].u && tempverts[j].v == mesh->uvs[i].v){
					tri->uvindx[j] = i; //IF UV COORDINATES ALREADY EXIST USE THOSE
					tri->uvmapped = 0x01;
				}
				
			}	
	
		if(!tri->uvmapped){ //ADD NEW UV COORDINATES TO LIST
			mesh->nuvs++;
			mesh->uvs = realloc(mesh->uvs,mesh->nuvs*sizeof(uv_t));
			
			if(mesh->uvs == NULL){
				fprintf(stderr,"ERROR REALLOCATING UV ARRAY\n");
				return;
			}
			
			mesh->uvs[mesh->nuvs-1] = tempverts[j];
			tri->uvindx[j] = mesh->nuvs-1;
			tri->uvmapped = 0x01;
		}
	}
	
	
	//printf("MESH N UVs: %d\n",mesh->nuvs); 
}


void ProjectMapUVGroup(mesh_t* shipmesh, uint32_t* uvgrp, uint16_t npolys, uvProject_t projplane, xyzbounds_t bounds){
	
	uint16_t i = 0;
	float u0, v0, usz, vsz, uscale = 0.95f, vscale = 0.95f;
	float du = 1.0f/TEXTURE_WIDTH, dv = 1.0f/TEXTURE_HEIGHT;
	xyzbounds_t uvbounds = {0};

	switch(shipmesh->polys[uvgrp[0]].type){
			
		case DIFFUSE1:
			u0=DIFFUSE1_UOFFSET*du; v0=DIFFUSE1_VOFFSET*dv;
			usz=DIFFUSE1_USIZE*du; vsz=DIFFUSE1_VSIZE*dv;

			break;
		
		case DIFFUSE2:
			u0=DIFFUSE2_UOFFSET*du; v0=DIFFUSE2_VOFFSET*dv;
			usz=DIFFUSE2_USIZE*du; vsz=DIFFUSE2_VSIZE*dv;

			break;
			
		case BOTTOM:
			u0=BOTTOM_UOFFSET*du; v0=BOTTOM_VOFFSET*dv;
			usz=BOTTOM_USIZE*du; vsz=BOTTOM_VSIZE*dv;

			break;
			
		case MECHANICS:
			u0=MECHANICS_UOFFSET*du; v0=MECHANICS_VOFFSET*dv;
			usz=MECHANICS_USIZE*du; vsz=MECHANICS_VSIZE*dv;

			break;
			
		case GLASS:
			u0=GLASS_UOFFSET*du; v0=GLASS_VOFFSET*dv;
			usz=GLASS_USIZE*du; vsz=GLASS_VSIZE*dv;
			
			break;
			
		case EXHAUST:
			u0=EXHAUST_UOFFSET*du; v0=EXHAUST_VOFFSET*dv;
			usz=EXHAUST_USIZE*du; vsz=EXHAUST_VSIZE*dv;

			break;
			
		case EXHAUST_INNER:
			u0=EXHAUST_INNER_UOFFSET*du; v0=EXHAUST_INNER_VOFFSET*dv;
			usz=EXHAUST_INNER_USIZE*du; vsz=EXHAUST_INNER_VSIZE*dv;
			break;
			
		case INTAKE:
			u0=INTAKE_UOFFSET*du; v0=INTAKE_VOFFSET*dv;
			usz=INTAKE_USIZE*du; vsz=INTAKE_VSIZE*dv;
		
			break;
	}
	
	for(i=0;i<npolys;i++){
		
		shipmesh->polys[i].uvmapped = 0x00;
		
		uvbounds.xmin = u0; uvbounds.xmax = usz;
		uvbounds.ymin = v0; uvbounds.ymax = vsz;
		uvbounds.zmin = uscale; uvbounds.zmax = vscale; //use them space lol
		
		XYZ2UV_triangle2(&shipmesh->polys[uvgrp[i]], shipmesh, bounds, uvbounds, projplane);
		
	}
	
}


void GenerateUVs(mesh_t* shipmesh){
	
	uint32_t* UVpolygroup = NULL;
	uint32_t* subgroup = NULL;
	uint16_t nUVpolys = 0, i = 0, j = 0, k = 0, nSubpolys = 0;
	xyzbounds_t UVpolygroupbounds = {0};
	uvProject_t UVprojectplane = UVXY;
	
	float du = 1.0f/TEXTURE_WIDTH, dv = 1.0f/TEXTURE_HEIGHT;
	xyzbounds_t UVoffsets;
	
	printf("Generating UV coordinates...\n");
	
	for(i=0;i<NUMTEXTURES;i++){ //i takes the role of texture type
		
		//Group faces that share the same texture
		UVpolygroup = GenerateUVPolyGroup(shipmesh,i, &nUVpolys);
		
		if(UVpolygroup == NULL)
			continue;
		
		if(i == DIFFUSE1 || i == DIFFUSE2){

			for(j=0;j<NUMSUBPARTS;j++){
				subgroup = GenerateSubGroup(shipmesh,UVpolygroup,nUVpolys,j,&nSubpolys);
				if(subgroup == NULL)
					continue;
				
				UVpolygroupbounds = GetPolyGroupBounds(shipmesh,subgroup,nSubpolys);
				//Get Best Projection plane
				UVprojectplane = GetBestUVProjection(shipmesh,subgroup,nSubpolys);
				
				if(j==WINGS){
					UVprojectplane = UVYZ;
				}
				
				//Do projection mapping	
				ProjectMapUVGroup(shipmesh,subgroup,nSubpolys,UVprojectplane,UVpolygroupbounds);
				//clean up
				
				free(subgroup);
				subgroup = NULL;
			}
		}
		else if(i == EXHAUST_INNER){
			UVpolygroupbounds = GetPolyGroupBounds(shipmesh,UVpolygroup,nUVpolys);
			UVprojectplane = GetBestUVProjection(shipmesh,UVpolygroup,nUVpolys);
			ProjectMapUVGroup(shipmesh,UVpolygroup,nUVpolys,UVprojectplane,UVpolygroupbounds);	
		}
		else{
			UVpolygroupbounds = GetPolyGroupBounds(shipmesh,UVpolygroup,nUVpolys);
			UVprojectplane = GetBestUVProjection(shipmesh,UVpolygroup,nUVpolys);
			ProjectMapUVGroup(shipmesh,UVpolygroup,nUVpolys,UVprojectplane,UVpolygroupbounds);
		}
		//clean up

		
		free(UVpolygroup);
		UVpolygroup = NULL;
		
	}
	
	printf("Done!\n");
		
}


