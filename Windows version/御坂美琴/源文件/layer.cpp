#include "stdafx.h"
/*
 * author      御坂美琴
 * E-mail      fmmmm@qq.com
 * filename    layer.cpp
 * date        20110105
 */

#include "layer.h"

void Layer_Init(struct Layer* This, struct Cube* pallcube, int _x, int _y, int _z, int style, int lz)
{
    int *px, *py, *pz;
    pti3d dx, dy, beg, bx, by;
    int x, y, xy, it;

    if      (style == 0)
    {
        px = &_z, py = &_x, pz = &_y;
        dx.x = 0, dx.y = 0, dx.z = 1;
        dy.x = -1, dy.y = 0, dy.z = 0;
        beg.x = _x-1, beg.y = lz, beg.z = 0;
        This->cmd = 'U';
        This->style = 0;
    }
    else if (style == 1)
    {
        px = &_x, py = &_y, pz = &_z;
        dx.x = -1, dx.y = 0, dx.z = 0;
        dy.x = 0, dy.y = 1, dy.z = 0;
        beg.x = _x-1, beg.y = 0, beg.z = lz;
        This->cmd = 'F';
        This->style = 1;
    }
    else if (style == 2)
    {
        px = &_y, py = &_z, pz = &_x;
        dx.x = 0, dx.y = 1, dx.z = 0;
        dy.x = 0, dy.y = 0, dy.z = 1;
        beg.x = lz, beg.y = 0, beg.z = 0;
        This->cmd = 'R';
        This->style = 2;
    }
    xy = *px * *py;

    This->sx = *px;
    This->sy = *py;
    This->pCubes = (struct Cube**) malloc(sizeof(struct Cube*) * xy);

    for (by = beg, y = 0; y < *py; ++y, add(&by, &dy))
    {
        for (bx = by, x = 0; x < *px; ++x, add(&bx, &dx))
        {
            This->pCubes[y * *px + x] = &pallcube[bx.z * _x * _y + bx.y * _x + bx.x];
        }
    }

    This->lturn = (int*) malloc (sizeof(int) * xy);
    This->rturn = (int*) malloc (sizeof(int) * xy);

    if ( *px == *py)
    {
        for (x = 0; x < *px; ++x)
        {
            it = (x << 16);
            for (y = *py - 1; y >= 0; --y)
            {
                This->lturn[y * *px + x] = it;
                it ++;
            }
        }
        for (x = *px - 1; x >= 0; --x)
        {
            it = ((*px - x - 1) << 16);
            for (y = 0; y < *py; ++y)
            {
                This->rturn[y * *px + x] = it;
                it ++;
            }
        }
        This->twice = 0;
    }
    else
    {
        for (y = *py - 1; y >= 0; --y)
        {
            it = ((*py - y - 1) << 16);
            for (x = *px - 1; x >= 0; --x)
            {
                This->lturn[y * *px + x] = it;
                it ++;
            }
        }
        memcpy(This->rturn, This->lturn, sizeof(int) * xy);
        This->twice = 1;
    }
}

void minusRad(float* rad, float df)
{
    if (fabs(*rad) < 0.1f) *rad = 0.0f;
    else if (*rad < 0) *rad += df;
    else *rad -= df;
}

int Layer_Update(struct Layer* This)
{
    int x, y, it = 0;
    if (This->style) return 0;
    for (y = 0; y < This->sy; ++y)
    {
        for (x = 0; x < This->sx; ++x)
        {
            minusRad(&This->pCubes[it]->rotate_x, 0.1f);
            minusRad(&This->pCubes[it]->rotate_y, 0.1f);
            minusRad(&This->pCubes[it]->rotate_z, 0.1f);
            it ++;
        }
    }
    return 0;
}

void Layer_Rotate(struct Layer* This, int v)
{
    int x, y, xy, i, it, itn;
    int* t;
    float rrad;
    struct Cube* pCubes;

    xy = This->sx * This->sy;
    pCubes = (struct Cube*) malloc(sizeof(struct Cube) * xy);
    for (it = 0; it < xy; ++it)
    memcpy(&pCubes[it], This->pCubes[it], sizeof(struct Cube));

    if (v > 0) t = This->lturn;
    else t = This->rturn;

    if (This->twice) rrad = (float)PI;
    else rrad = (float)(PI / 2);

    if (v > 0) rrad = -rrad;

    it = 0;
    for (y = 0; y < This->sy; ++y)
    {
        for (x = 0; x < This->sx; ++x)
        {
            int nx = (t[y * This->sx + x] & 0xFFFF);
            int ny = (t[y * This->sx + x] >> 16);
            itn = ny * This->sx + nx;
            memcpy(&This->pCubes[itn]->face, &pCubes[it].face, sizeof(struct Face[6]));
            for ( i = 0; i <= This->twice; ++i)
            {
                Cube_Rotate(This->pCubes[itn], This->cmd, v);
            }
            if (This->style == 0)
            {
                This->pCubes[itn]->rotate_y = rrad;
            }
            else if (This->style == 1)
            {
                This->pCubes[itn]->rotate_z = rrad;
            }
            else if (This->style == 2)
            {
                This->pCubes[itn]->rotate_x = rrad;
            }
            it ++;
        }
    }

    free(pCubes);
}

void Layer_Release(struct Layer* This)
{
    free(This->lturn);
    free(This->rturn);
    free(This->pCubes);
}

int Layer_FindCube(struct Layer* This, struct Cube* pcube, int face)
{
    int x, y, xy, it = 0;
    xy = This->sx * This->sy;
    for (y = 0; y < This->sy; ++y)
    {
        for (x = 0; x < This->sx; ++x)
        {
            if (This->pCubes[it] == pcube)
            {
                int facei[][4] = 
                {
                    {3, 1, 2, 4},
                    {5, 2, 0, 3},
                    {4, 0, 1, 5},
                };
                int fit;
                int item;
                for (fit = 0; fit < 4; ++fit)
                {
                    if (facei[This->style][fit] == face) break;
                }
                if ( x == 0 || y == 0 )
                {
                    if (x == 0) item = This->sy - y - 1;
                    else item = This->sy + x;
                    if (x == 0 && y == 0 && fit == 3) fit = -1;
                    item = (item << 8) + fit;
                }
                else
                {
                    if (x + 1 == This->sx) item = y;
                    else item = This->sy + (This->sx - x - 1);
                    item = ((item + This->sx + This->sy + 1) << 8) + fit;
                }
                return item;
            }
            it ++;
        }
    }
    return -1;
}
