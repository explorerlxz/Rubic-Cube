#include "stdafx.h"
/*
 * author      御坂美琴
 * E-mail      fmmmm@qq.com
 * filename    layer.h
 * date        20110105
 */

#include "cube.h"

struct Layer
{
    struct Cube** pCubes;
    int sx, sy;
    int *lturn;
    int *rturn;
    int twice;
    char cmd;
    int style;
};

void Layer_Init(struct Layer* This, struct Cube* pallcube, int _x, int _y, int _z, int style, int lz);
void Layer_Rotate(struct Layer* This, int v);
void Layer_Release(struct Layer* This);
int Layer_FindCube(struct Layer* This, struct Cube* pcube, int face);
int Layer_Update(struct Layer* This);
