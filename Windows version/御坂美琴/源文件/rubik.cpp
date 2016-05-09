#include "stdafx.h"
/*
 * author      御坂美琴
 * E-mail      fmmmm@qq.com
 * filename    rubik.cpp
 * date        20110105
 */

#include "rubik.h"

static int render(struct Rubik* This, HTEXTURE* ptex)
{
    int x, y, z, it = 0;
    This->ncnt = 0;
    for (z = 0; z < This->mz; ++z)
    {
        for (y = 0; y < This->my; ++y)
        {
            for (x = 0; x < This->mx; ++x)
            {
                if (x == 0 || x + 1 == This->mx || y == 0 || y + 1 == This->my || z == 0 || z + 1 == This->mz)
                {
                    This->pAllCube[it].render(&This->pAllCube[it], ptex);
                    This->ncnt ++;
                }
                it ++;
            }
        }
    }
    return 0;
}

void Rubik_Init(struct Rubik* This, int _x, int _y, int _z)
{
    int x, y, z, it = 0;
    This->render = render;
    This->mx = _x;
    This->my = _y;
    This->mz = _z;
    This->xyz = _x * _y * _z;
    This->pAllCube = (struct Cube*) malloc (sizeof(struct Cube) * This->xyz);
    for (z = 0; z < _z; ++z)
    {
        for (y = 0; y < _y; ++y)
        {
            for (x = 0; x < _x; ++x)
            {
                if (x == 0 || x + 1 == _x || y == 0 || y + 1 == _y || z == 0 || z + 1 == _z)
                {
                    Cube_Init(&This->pAllCube[it], _x, _y, _z, x, y, z);
                }
                it ++;
            }
        }
    }
    Dimension_Init(&This->d_y, This->pAllCube, _x, _y, _z, 0);
    Dimension_Init(&This->d_z, This->pAllCube, _x, _y, _z, 1);
    Dimension_Init(&This->d_x, This->pAllCube, _x, _y, _z, 2);
}

void Rubik_Rotate(struct Rubik* This, char cmd, int ilayer, int v)
{
    if      (cmd == 'U')
    {
        Dimension_Rotate(&This->d_y, ilayer, v);
    }
    else if (cmd == 'F')
    {
        Dimension_Rotate(&This->d_z, ilayer, v);
    }
    else if (cmd == 'R')
    {
        Dimension_Rotate(&This->d_x, ilayer, v);
    }
}

void Rubik_Release(struct Rubik* This)
{
    Dimension_Release(&This->d_x);
    Dimension_Release(&This->d_y);
    Dimension_Release(&This->d_z);
    free(This->pAllCube);
}

int Rubik_pick(struct Rubik* This, VECTOR3D start, VECTOR3D end, int style)
{
    int x, y, z, it = 0;
    int rit = -1;
    pick_info pmin ={-1, -100.0f};

    for (z = 0; z < This->mz; ++z)
    {
        for (y = 0; y < This->my; ++y)
        {
            for (x = 0; x < This->mx; ++x)
            {
                if (x == 0 || x + 1 == This->mx || y == 0 || y + 1 == This->my || z == 0 || z + 1 == This->mz)
                {
                    pick_info r;
                    r = Cube_pick(&This->pAllCube[it], start, end);
                    if (r.info >= 0 && (pmin.z < 0 || pmin.z > r.z && r.z >= 0))
                    {
                        pmin = r;
                        rit = it;
                    }
                }
                it ++;
            }
        }
    }
    if (pmin.info >= 0)
    {
        if (style == 1)
        {
            This->pick_st_id = rit;
            This->pick_st_f  = pmin.info;
        }
        else
        {
            This->pick_ed_id = rit;
            This->pick_ed_f  = pmin.info;
            if (This->pick_st_id != This->pick_ed_id || This->pick_st_f != This->pick_ed_f)
            {
                int s, e;
                int is, ie;
                dim_layer sd[8] = {0}, ed[8] = {0};
                s = Rubik_finddim(This, &This->pAllCube[This->pick_st_id], This->pick_st_f, sd);
                e = Rubik_finddim(This, &This->pAllCube[This->pick_ed_id], This->pick_ed_f, ed);
                for (is = 0; sd[is].ndim > 0; ++is)
                {
                    for (ie = 0; ed[ie].ndim > 0; ++ie)
                    {
                        if (sd[is].ndim == ed[ie].ndim && sd[is].nlayer == ed[ie].nlayer)
                        {
                            char cmd[] = " RUF";
                            int v;
                            float df;
                            df = ed[ie].faceval - sd[is].faceval;
                            if (df > 1.0f) df -= 2.0f;
                            if (df < -1.0f) df += 2.0f;
                            if (df >= 0) v = 1;
                            else v = 0;
                            Rubik_Rotate(This, cmd[sd[is].ndim], sd[is].nlayer, v);

                            sd[is + 1].ndim = 0; // for break
                            break;
                        }
                    }
                }
            }
        }
    }
    return rit;
}

int Rubik_finddim(struct Rubik* This, struct Cube* pcube, int face, dim_layer* retd)
{
    int retL, reti = 0;
    float faceval;
    if (face == 1 || face == 4)
    {
        retL = Dimension_findlayer(&This->d_y, pcube, face, &faceval);
        if (retL >= 0)
        {
            retd[reti].ndim = 2;
            retd[reti].nlayer = retL;
            retd[reti].faceval = faceval;
            reti ++;
        }
        retL = Dimension_findlayer(&This->d_x, pcube, face, &faceval);
        if (retL >= 0)
        {
            retd[reti].ndim = 1;
            retd[reti].nlayer = retL;
            retd[reti].faceval = faceval;
            reti ++;
        }
    }
    else if (face == 0 || face == 5)
    {
        retL = Dimension_findlayer(&This->d_x, pcube, face, &faceval);
        if (retL >= 0)
        {
            retd[reti].ndim = 1;
            retd[reti].nlayer = retL;
            retd[reti].faceval = faceval;
            reti ++;
        }
        retL = Dimension_findlayer(&This->d_z, pcube, face, &faceval);
        if (retL >= 0)
        {
            retd[reti].ndim = 3;
            retd[reti].nlayer = retL;
            retd[reti].faceval = faceval;
            reti ++;
        }
    }
    else if (face == 2 || face == 3)
    {
        retL = Dimension_findlayer(&This->d_y, pcube, face, &faceval);
        if (retL >= 0)
        {
            retd[reti].ndim = 2;
            retd[reti].nlayer = retL;
            retd[reti].faceval = faceval;
            reti ++;
        }
        retL = Dimension_findlayer(&This->d_z, pcube, face, &faceval);
        if (retL >= 0)
        {
            retd[reti].ndim = 3;
            retd[reti].nlayer = retL;
            retd[reti].faceval = faceval;
            reti ++;
        }
    }
    return 0;
}

int Rubik_Update(struct Rubik* This)
{
    Dimension_Update(&This->d_x);
    Dimension_Update(&This->d_y);
    Dimension_Update(&This->d_z);
    return 0;
}
