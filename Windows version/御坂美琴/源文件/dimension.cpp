#include "stdafx.h"
/*
 * author      御坂美琴
 * E-mail      fmmmm@qq.com
 * filename    dimension.cpp
 * date        20110105
 */

#include "dimension.h"

void Dimension_Init(struct Dimension* This, struct Cube* pallcube, int _x, int _y, int _z, int style)
{
    int *px, *py, *pz;
    int z;
    if      (style == 0)
    {
        px = &_z, py = &_x, pz = &_y;
    }
    else if (style == 1)
    {
        px = &_x, py = &_y, pz = &_z;
    }
    else if (style == 2)
    {
        px = &_y, py = &_z, pz = &_x;
    }
    This->nLayer = *pz;
    This->pLayer = (struct Layer*) malloc (sizeof(struct Layer) * *pz);
    for (z = 0; z < *pz; ++z)
    {
        Layer_Init(&This->pLayer[z], pallcube, _x, _y, _z, style, z);
    }
}

void Dimension_Rotate(struct Dimension* This, int ilayer, int v)
{
    Layer_Rotate(&This->pLayer[ilayer], v);
}

void Dimension_Release(struct Dimension* This)
{
    int it;
    for (it = 0; it < This->nLayer; ++it)
    {
        Layer_Release(&This->pLayer[it]);
    }
    free(This->pLayer);
}

int Dimension_findlayer(struct Dimension* This, struct Cube* pcube, int face, float* retfaceval)
{
    int it;
    for (it = 0; it < This->nLayer; ++it)
    {
        int fid;
        fid = Layer_FindCube(&This->pLayer[it], pcube, face);
        if (fid >= 0)
        {
            *retfaceval = (float)((float)fid / (This->pLayer->sx + This->pLayer->sy) / 256);
            return it;
        }
    }
    return -1;
}

int Dimension_Update(struct Dimension* This)
{
    int it;
    for (it = 0; it < This->nLayer; ++it)
    {
        Layer_Update(&This->pLayer[it]);
    }
    return 0;
}
