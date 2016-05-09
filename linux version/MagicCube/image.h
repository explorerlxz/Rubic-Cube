/**
 * @file   image.h
 * @author Devil Wang <wxjeacen@gmail.com>
 * @date   Tue Dec 28 22:46:50 2010
 *
 * @brief
 *
 *
 */

#ifndef MAGIC_CUBE_LOAD_BMP_H_
#define MAGIC_CUBE_LOAD_BMP_H_

#define BMP_HEADER_SIZE 18

typedef struct _image
{
	unsigned long sizeX;
	unsigned long sizeY;
	char *data;
}Image;

int ImageLoad(char *filename, Image *img);

#endif
