/**
 * @file   glFunc.h
 * @author Devil Wang <wxjeacen@gmail.com>
 * @date   Tue Dec 28 22:40:30 2010
 *
 * @brief
 *
 *
 */

#ifndef MAGIC_CUBE_GL_FUNC_H
#define MAGIC_CUBE_GL_FUNC_H

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <ctype.h>
#include "Cube.h"
#include "image.h"
//util functions
void InitGL(int width, int height);
void LoadTextures();

//glut callback functions
void glDisplay();
void glReshape(int width, int height);
void glKeyboard(unsigned char key, int x, int y);
void glMouse(int button, int state, int x, int y);
void glMotion(int x, int y);

//external functions
void DrawCube();
extern GLuint texture[1];
/* extern Cube g_cube; */
/* extern GLint g_viewport[4]; */
/* extern GLdouble g_modelMatrix[16]; */
/* extern GLdouble g_projMatrix[16]; */
/* extern GLdouble g_winX, g_winY, g_winZ; */
/* extern GLfloat g_angleX ; */
/* extern GLfloat g_angleY ; */
/* extern int g_mouseX, g_mouseY, g_mouseState, g_mouseButton; */
/* /\* extern const GLfloat step ; *\/ */
/* extern int g_selFace; */
/* extern GLfloat g_cubeRot; */
/* extern int g_rotDir; */
#endif
