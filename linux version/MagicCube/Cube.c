/**
 * @file   Cube.c
 * @author Devil Wang <wxjeacen@gmail.com>
 * @date   Tue Dec 28 22:36:57 2010
 * 
 * @brief  
 * 
 * 
 */

#include "Cube.h"
#include <time.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
//Global MagicCube
Cube g_cube;
int g_selFace = -1;
int g_rotDir;
GLfloat g_cubeRot = 90.0;

GLubyte colors[6][3] = {
	{255, 0, 0},	//red
	{255, 255, 0},	//yellow
	{0, 0, 255},	//blue
	{255, 0, 255},	//violet
	{255, 128, 0},	//orange
	{0, 255, 0}		//green
};

float rotate[6][4] = {
	{ 90.0f,-1.0f, 0.0f, 0.0f},		//rotate to top
	{ 90.0f, 1.0f, 0.0f, 0.0f},		//rotate to front
	{ 90.0f, 0.0f,-1.0f, 0.0f},		//rotate to left
	{180.0f, 0.0f, 1.0f, 0.0f},		//rotate to right
	{ 90.0f, 0.0f, 1.0f, 0.0f},		//rotate to back
	{ 90.0f, 1.0f, 0.0f, 0.0f}		//rotate to bottom
};

int adjVertex[6][6][4] = {
	{
		{-1, -1,  0,  0},
		{ 3,  0,  0, -1},
		{ 0,  3, -1,  0},
		{ 2,  3,  1,  0},
		{ 3,  2,  0,  1},
		{-1, -1,  0,  0},
	},
	{
		{ 3,  2,  0,  1},
		{-1, -1,  0,  0},
		{ 0,  3, -1,  0},
		{ 2,  3,  1,  0},
		{-1, -1,  0,  0},
		{ 3,  0,  0, -1},
	},
	{
		{ 3,  2,  0,  1},
		{ 2,  3,  1,  0},
		{-1, -1,  0,  0},
		{-1, -1,  0,  0},
		{ 0,  3, -1,  0},
		{ 3,  0,  0, -1},
	},
	{
		{ 3,  2,  0,  1},
		{ 0,  3, -1,  0},
		{-1, -1,  0,  0},
		{-1, -1,  0,  0},
		{ 2,  3,  1,  0},
		{ 3,  0,  0, -1},
	},
	{
		{ 3,  2,  0,  1},
		{-1, -1,  0,  0},
		{ 2,  3,  1,  0},
		{ 0,  3, -1,  0},
		{-1, -1,  0,  0},
		{ 3,  0,  0, -1},
	},
	{
		{-1, -1,  0,  0},
		{ 3,  0,  0, -1},
		{ 2,  3,  1,  0},
		{ 0,  3, -1,  0},
		{ 3,  2,  0,  1},
		{-1, -1,  0,  0},
	}
};


void resetCube()
{
	int face;
	int x, y;
	int c = 0;
	for (face = 0; face < 6; face++)
		for(x = 0; x < 3; x++)
			for(y = 0; y < 3; y++)
				g_cube.block[face][x][y] = face;
//				g_cube.block[face][x][y] = c++ % 6;
	srandom(time((time_t*)0));
	for (c = 0; c < 100; c++)
	{
		rotateCube(random() % 6);
	}
}

void rotateCube(int face)
{
	int (*b)[3][3] = g_cube.block;
	int (*f)[3] = b[face];
	int temp;
	temp = f[0][0];
	f[0][0] = f[0][2];
	f[0][2] = f[2][2];
	f[2][2] = f[2][0];
	f[2][0] = temp;

	temp = f[0][1];
	f[0][1] = f[1][2];
	f[1][2] = f[2][1];
	f[2][1] = f[1][0];
	f[1][0] = temp;	

	switch (face)
	{
		case 0:
			temp = b[1][0][2];
			b[1][0][2] = b[2][0][2];
			b[2][0][2] = b[4][0][2];
			b[4][0][2] = b[3][0][2];
			b[3][0][2] = temp;
	
			temp = b[1][1][2];
			b[1][1][2] = b[2][1][2];
			b[2][1][2] = b[4][1][2];
			b[4][1][2] = b[3][1][2];
			b[3][1][2] = temp;
	
			temp = b[1][2][2];
			b[1][2][2] = b[2][2][2];
			b[2][2][2] = b[4][2][2];
			b[4][2][2] = b[3][2][2];
			b[3][2][2] = temp;
			break;
		case 1:
			temp = b[0][0][0];
			b[0][0][0] = b[3][0][2];
			b[3][0][2] = b[5][0][0];
			b[5][0][0] = b[2][2][0];
			b[2][2][0] = temp;

			temp = b[0][1][0];
			b[0][1][0] = b[3][0][1];
			b[3][0][1] = b[5][1][0];
			b[5][1][0] = b[2][2][1];
			b[2][2][1] = temp;

			temp = b[0][2][0];
			b[0][2][0] = b[3][0][0];
			b[3][0][0] = b[5][2][0];
			b[5][2][0] = b[2][2][2];
			b[2][2][2] = temp;
			break;
		case 2:
			temp = b[0][0][0];
			b[0][0][0] = b[1][0][0];
			b[1][0][0] = b[5][2][2];
			b[5][2][2] = b[4][2][2];
			b[4][2][2] = temp;

			temp = b[0][0][1];
			b[0][0][1] = b[1][0][1];
			b[1][0][1] = b[5][2][1];	
			b[5][2][1] = b[4][2][1];
			b[4][2][1] = temp;

			temp = b[0][0][2];
			b[0][0][2] = b[1][0][2];
			b[1][0][2] = b[5][2][0];
			b[5][2][0] = b[4][2][0];
			b[4][2][0] = temp;
			break;
		case 3:
			temp = b[0][2][0];
			b[0][2][0] = b[4][0][2];
			b[4][0][2] = b[5][0][2];
			b[5][0][2] = b[1][2][0];
			b[1][2][0] = temp;

			temp = b[0][2][1];
			b[0][2][1] = b[4][0][1];
			b[4][0][1] = b[5][0][1];
			b[5][0][1] = b[1][2][1];
			b[1][2][1] = temp;

			temp = b[0][2][2];
			b[0][2][2] = b[4][0][0];
			b[4][0][0] = b[5][0][0];
			b[5][0][0] = b[1][2][2];
			b[1][2][2] = temp;
			break;
		case 4:
			temp = b[0][0][2];
			b[0][0][2] = b[2][0][0];
			b[2][0][0] = b[5][0][2];
			b[5][0][2] = b[3][2][2];
			b[3][2][2] = temp;

			temp = b[0][1][2];
			b[0][1][2] = b[2][0][1];
			b[2][0][1] = b[5][1][2];
			b[5][1][2] = b[3][2][1];
			b[3][2][1] = temp;

			temp = b[0][2][2];
			b[0][2][2] = b[2][0][2];
			b[2][0][2] = b[5][2][2];
			b[5][2][2] = b[3][2][0];
			b[3][2][0] = temp;
			break;
		case 5:
			temp = b[1][0][0];
			b[1][0][0] = b[3][0][0];
			b[3][0][0] = b[4][0][0];
			b[4][0][0] = b[2][0][0];
			b[2][0][0] = temp;
			
			temp = b[1][1][0];
			b[1][1][0] = b[3][1][0];
			b[3][1][0] = b[4][1][0];
			b[4][1][0] = b[2][1][0];
			b[2][1][0] = temp;

			temp = b[1][2][0];
			b[1][2][0] = b[3][2][0];
			b[3][2][0] = b[4][2][0];
			b[4][2][0] = b[2][2][0];
			b[2][2][0] = temp;
			break;
	}
}

void drawCube()
{
	int face, x, y;
	float fx, fy;
	static float offset =  1.0f / 3.0f;
	int pushFlag1, pushFlag2;
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	for (face = 0; face < 6; face++)
	{
		glLoadName(face);
		glRotatef(rotate[face][0], rotate[face][1],
                  rotate[face][2], rotate[face][3]);

		pushFlag1 = 0;
		if (g_selFace == face)
		{
			/*
              glBegin(GL_LINE_LOOP);
              glColor3f(1.0, 1.0, 1.0);
              glVertex3f(-0.5, -0.5, 0.5);
              glVertex3f(-0.5,  0.5, 0.5);
              glVertex3f( 0.5,  0.5, 0.5);
              glVertex3f( 0.5, -0.5, 0.5);
              glEnd();
            */
			glBegin(GL_QUADS);
            glColor3f(0.0, 0.0, 0.0);
            glVertex3f(-0.49, -0.49, offset / 2.0);
            glVertex3f(-0.49,  0.49, offset / 2.0);
            glVertex3f( 0.49,  0.49, offset / 2.0);
            glVertex3f( 0.49, -0.49, offset / 2.0);
			glEnd();
			glBegin(GL_LINE_LOOP);
            glColor3f(1.0, 1.0, 1.0);
            glVertex3f(-0.5, -0.5, offset / 2.0);
            glVertex3f(-0.5,  0.5, offset / 2.0);
            glVertex3f( 0.5,  0.5, offset / 2.0);
            glVertex3f( 0.5, -0.5, offset / 2.0);
			glEnd();
			if (abs(g_cubeRot) < 90.0)
			{
				glPushMatrix();
				glRotatef(g_cubeRot, 0.0, 0.0, 1.0);
				pushFlag1 = 1;
			}
		}

		for(x = 0; x < 3; x++)
		{
			fx = (x - 1.5f) * offset;
			for(y = 0; y < 3; y++)
			{
				pushFlag2 = 0;
				if (abs(g_cubeRot) < 90.0 && g_selFace != -1)
				{
					int rx = adjVertex[face][g_selFace][0];
					int ry = adjVertex[face][g_selFace][1];
					int ax = adjVertex[face][g_selFace][2];
					int ay = adjVertex[face][g_selFace][3];
					if ((x == rx || rx == 3) && (y == ry || ry == 3))
					{
						glPushMatrix();
						glRotatef(g_cubeRot, ax, ay, 0);
						pushFlag2 = 1;
					}
				}
				fy = (y - 1.5f) * offset;
				glBegin(GL_QUADS);
				glColor3ubv( colors[ g_cube.block[face][x][y] ] );
				glTexCoord2f(0.0f, 0.0f);	//bottom left 
				glVertex3f(fx, fy, 0.5f);
				glTexCoord2f(1.0f, 0.0f);	//bottom right 
				glVertex3f(fx + offset, fy, 0.5f);
				glTexCoord2f(1.0f, 1.0f);	//top right
				glVertex3f(fx + offset, fy + offset, 0.5f);
				glTexCoord2f(0.0f, 1.0f);	//top left
				glVertex3f(fx, fy + offset, 0.5f);
				glEnd();
				if (pushFlag2 == 1)
				{
					glPopMatrix();
				}
			}
		}
		if (pushFlag1 == 1)
		{
			glPopMatrix();
			if (g_rotDir == GLUT_RIGHT_BUTTON)
			{
				g_cubeRot += 0.5;
				if (g_cubeRot >= 90.0)
				{
					rotateCube(g_selFace);
					g_selFace = -1;
				}
			}
			else if(g_rotDir == GLUT_LEFT_BUTTON)
			{
				g_cubeRot -= 0.5;
				if (g_cubeRot <= -90.0)
				{
					rotateCube(g_selFace);
					rotateCube(g_selFace);
					rotateCube(g_selFace);
					g_selFace = -1;
				}
			}

		}
	}
	glPopMatrix();
}


