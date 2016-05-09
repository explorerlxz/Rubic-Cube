#include "stdafx.h"
/*
 * author      御坂美琴
 * E-mail      fmmmm@qq.com
 * filename    dimension.h
 * date        20110105
 */

#include "layer.h"

struct Dimension
{
    struct Layer* pLayer;
    int nLayer;
};

void Dimension_Init(struct Dimension* This, struct Cube* pallcube, int _x, int _y, int _z, int style);
void Dimension_Rotate(struct Dimension* This, int ilayer, int v);
void Dimension_Release(struct Dimension* This);
int Dimension_findlayer(struct Dimension* This, struct Cube* pcube, int face, float* retfaceval);
int Dimension_Update(struct Dimension* This);
