/**
 * @file   glFunc.c
 * @author Devil Wang <wxjeacen@gmail.com>
 * @date   Tue Dec 28 22:43:47 2010
 * 
 * @brief  
 * 
 * 
 */

#include <stdio.h>
#include <stdlib.h>
#include "glFunc.h"
#include "image.h"
#include "Cube.h"

GLuint texture[1];
Cube g_cube;


GLint g_viewport[4];
GLdouble g_modelMatrix[16];
GLdouble g_projMatrix[16];
GLdouble g_winX, g_winY, g_winZ;
GLfloat g_angleX = 0.0;
GLfloat g_angleY = 0.0;
int g_mouseX, g_mouseY, g_mouseState, g_mouseButton;
const GLfloat step = 20;

int g_selFace;
GLfloat g_cubeRot;
int g_rotDir;

void InitGL(int width, int height)
{
    LoadTextures();
    glEnable(GL_TEXTURE_2D);
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClearDepth(1.5f);
    glDepthFunc(GL_LESS);
    glEnable(GL_DEPTH_TEST);
    glShadeModel(GL_FLAT);
    glLineWidth(3.0);

    resetCube();
}

void LoadTextures()
{
    Image *image = (Image*)malloc(sizeof(Image));
    if ((LoadBmp("Data/block.bmp", image)) != 1)
    {
        printf("Error on loading textures!\n");
        exit(1);
    }

    glGenTextures((GLsizei)1, &texture[0]);
    glBindTexture(GL_TEXTURE_2D, texture[0]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, 3, image->sizeX, image->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, image->data);
}

void glReshape(int width, int height)
{
    glViewport(0, 0, width, height);
    glGetIntegerv(GL_VIEWPORT, g_viewport);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0f, (float) width / (float) height, 1.0f, 300.0f);
    glGetDoublev(GL_PROJECTION_MATRIX, g_projMatrix);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(0.0, 0.0, 3.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
    glGetDoublev(GL_MODELVIEW_MATRIX, g_modelMatrix);
    gluProject(0.0, 0.0, 0.0,
               g_modelMatrix,
               g_projMatrix,
               g_viewport,
               &g_winX, &g_winY, &g_winZ);	
}

void glDisplay()
{
    GLdouble wx, wy, wz;
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    if (abs(g_angleY) >= 1e-06)
    {
        gluUnProject(g_winX, 0.0, g_winZ,
                     g_modelMatrix,
                     g_projMatrix,
                     g_viewport,
                     &wx, &wy, &wz);
	
        glRotatef(g_angleY, wx, wy, wz);
        glGetDoublev(GL_MODELVIEW_MATRIX, g_modelMatrix);
    }

    if (abs(g_angleX) >= 1e-06)
    {
        gluUnProject(0.0, g_winY, g_winZ,
                     g_modelMatrix,
                     g_projMatrix,
                     g_viewport,
                     &wx, &wy, &wz);
	
        glRotatef(g_angleX, wx, wy, wz);
        glGetDoublev(GL_MODELVIEW_MATRIX, g_modelMatrix);
    }

    g_angleX = 0.0;
    g_angleY = 0.0;

    glBindTexture(GL_TEXTURE_3D, texture[0]);
    drawCube();
    glutSwapBuffers();
}

void glKeyboard(unsigned char key, int x, int y)
{
    switch (tolower(key))
    {
        case 'a':
            g_angleY = step;
            glutPostRedisplay();
            break;
        case 'd':
            g_angleY = -step;
            glutPostRedisplay();
            break;
        case 'w':
            g_angleX = step;
            glutPostRedisplay();
            break;
        case 's':
            g_angleX = -step;
            glutPostRedisplay();
            break;
    }
}

void processHits(GLint hits, GLuint* selBuffer)
{
    if (hits == 0)
    {
        g_selFace = -1;
        return;
    }
    int i;
    int min = 0;
    for (i = 0; i < hits; i++)
    {
        if (selBuffer[i * 4 + 1] < selBuffer[min * 4 + 1])
        {
            min = i;
        }
    }
    g_selFace = selBuffer[min * 4 + 3];
    g_cubeRot = 0;
    g_rotDir = g_mouseButton;
}

#define BUFSIZE 512

void selectFace(int x, int y)
{
    GLuint selBuffer[BUFSIZE];
    GLint hits;
    glSelectBuffer(BUFSIZE, selBuffer);
    glRenderMode(GL_SELECT);

    glInitNames();
    glPushName(0);
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    gluPickMatrix((GLfloat) x, (GLfloat) (g_viewport[3] - y),
                  1.0, 1.0, g_viewport);
    gluPerspective(45.0, (GLfloat) g_viewport[2] / (GLfloat) g_viewport[3], 1.0, 300.0);
    drawCube();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glFlush();

    hits = glRenderMode(GL_RENDER);
    processHits(hits, selBuffer);
    glutPostRedisplay();
}

void glMouse(int button, int state, int x, int y)
{
    g_mouseX = x;
    g_mouseY = y;
    g_mouseState = state;
    if (state == GLUT_DOWN)
    {
        g_mouseButton = button;
        selectFace(x, y);
    }
}

void glMotion(int x, int y)
{
    if (g_mouseState == GLUT_DOWN)
    {
        g_angleY = (g_mouseX -x) / 5.0;
        g_angleX = (g_mouseY -y) / 5.0;
        g_mouseX = x;
        g_mouseY = y;
        glutPostRedisplay();
    }
}
