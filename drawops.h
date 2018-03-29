#ifndef _DRAWOPS_H_
#define _DRAWOPS_H_

#include <stdio.h>
#include <stdlib.h>

#include <stdint.h>
#include <string.h>
#include <time.h>
//#include <windows.h>

#include "shipgenmaths.h"
#include "prim.h"

//HANDLE hConsole; 
//COORD cursorPos;

mesh_t *mainMesh;
mesh_t *dispMesh;

void BltText(char* text, uint16_t x, uint16_t y);
void DrawMesh(mesh_t mesh, float pitch, float yaw, float scale);
void ClrScr(uint8_t col);
void RenderBuffer();
void RenderLoop();
void PutPixel(uint16_t x, uint16_t y, uint8_t col);
void DrawLine(int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint8_t col);

void SetMainMesh(mesh_t *mesh);

#endif
