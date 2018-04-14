#include "prim.h"

void MeshToObj(mesh_t* mesh, char* filename){
	
	FILE* fp=NULL;
	uint32_t i=0, j=0;
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
	
	for(i=0;i<mesh->nnorms;i++){
		fprintf(fp,"vn %f %f %f\n",mesh->vnorms[i].x,mesh->vnorms[i].y,mesh->vnorms[i].z);
		
	}
	fprintf(fp,"#%d Normals\n",mesh->nnorms);
	
	for(i=0;i<mesh->nuvs;i++){
		fprintf(fp,"vt %f %f\n",mesh->uvs[i].u,1.0f-mesh->uvs[i].v);
		
	}
	fprintf(fp,"#%d UVs\n",mesh->nuvs);
	
	fprintf(fp,"g RNGShip\n");
	for(i=0;i<mesh->ntris;i++){
		
		fprintf(fp,"f ");
		for(j=0;j<3;j++){
			
			fprintf(fp,"%d/%d/%d",mesh->polys[i].vert[j]+1,mesh->polys[i].uvindx[j]+1,mesh->polys[i].vnindx[j]+1);
			if(j!=2){
				fprintf(fp," ");
			}
		}
		fprintf(fp,"\n");
	}
	fprintf(fp,"#%d Triangles\n",mesh->ntris);
	fclose(fp);
	printf("Save Successful!\n");
	//system("PAUSE");
	//system("cls");
	
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

uint8_t GenerateMeshNormals(mesh_t* mesh){
	
	uint32_t i = 0, j = 0;
	
	mesh->vnorms = malloc(mesh->ntris*sizeof(vert_t)); //one per triangle
	
	if(mesh->vnorms == NULL){
		fprintf(stderr,"FAILED TO ALLOCATE VERT NORMAL MEMORY!\n");
		exit(-1);
	}
	
	printf("Calculating model normals...\n");
	
	for(i=0;i<mesh->ntris;i++){
		for(j=0;j<3;j++){
			mesh->polys[i].vptr[j] = &mesh->verts[mesh->polys[i].vert[j]]; //get the pointers to the vertex data
			mesh->polys[i].vnptr[j] = &mesh->vnorms[i];
			mesh->polys[i].vnindx[j] = i;
		}
		
		mesh->vnorms[i] = CalcNormalTri(mesh->polys[i]);
		
	}
	mesh->nnorms = mesh->ntris;
	return 1;
}
