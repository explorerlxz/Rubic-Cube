/**
 * @file   Cube.h
 * @author Devil Wang <wxjeacen@gmail.com>
 * @date   Tue Dec 28 22:36:35 2010
 *
 * @brief
 *
 *
 */

#ifndef MAGIC_CUBE_CUBE_H
#define MAGIC_CUBE_CUBE_H
#include<GL/gl.h>
#include<stdlib.h>

typedef enum
{
	FACE_TOP,
	FACE_FRONT,
	FACE_LEFT,
	FACE_RIGHT,
	FACE_BACK,
	FACE_BOTTOM
}Face;

typedef struct
{
	int block[6][3][3];
	Face rotatingFace;
}Cube;

/* extern Cube g_cube; */
/* extern int g_selFace ; */
/* extern int g_rotDir; */
/* extern GLfloat g_cubeRot; */
/* extern GLubyte colors[6][3]; */
/* extern float rotate[6][4]; */
/* extern int adjVertex[6][6][4]; */
void resetCube();
void rotateCube(int face);
void drawCube();

#endif
