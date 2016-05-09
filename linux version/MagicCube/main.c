/**
 * @file   main.c
 * @author Devil Wang <wxjeacen@gmail.com>
 * @date   Tue Dec 28 23:09:43 2010
 * 
 * @brief  
 * 
 * 
 */

#include <stdio.h>
#include <stdlib.h>

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

#include "glFunc.h"

int main(int argc, char *argv[])
{
	glutInit(&argc, argv);

	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_ALPHA | GLUT_DEPTH);
	glutInitWindowSize(800, 600);
	glutInitWindowPosition(0, 0);
	
	glutCreateWindow("MagicCube");

	glutReshapeFunc(glReshape);
	glutDisplayFunc(glDisplay);
	glutIdleFunc(glDisplay);
	glutKeyboardFunc(glKeyboard);
	glutMouseFunc(glMouse);
	glutMotionFunc(glMotion);

	InitGL(800, 600);

	glutMainLoop();

	return 1;
}
