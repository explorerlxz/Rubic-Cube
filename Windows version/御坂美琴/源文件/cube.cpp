#include "stdafx.h"
/*
 * author      御坂美琴
 * E-mail      fmmmm@qq.com
 * filename    cube.cpp
 * date        20110105
 */

#include "cube.h"

static int render(struct Cube* This, HTEXTURE* ptex)
{
    int imap[6][4] = 
    {
        {2, 3, 1, 0},
        {3, 7, 0, 4},
        {2, 6, 3, 7},
        {0, 4, 1, 5},
        {1, 5, 2, 6},
        {7, 6, 4, 5},
    };
    int f, it;
    FPOINT3D pt[4];
    for (f = 0; f < 6; ++f)
    {
        if (This->face[f].visable)
        {
            int bObjSet = 0;
            for (it = 0; it < 4; ++it)
            {
                memcpy(&pt[it], &This->pos[imap[f][it]], sizeof(VECTOR3D));
                pt[it].color = 0xFFFFFFFF;
            }
            PushMatrix();
            if (This->rotate_x != 0.0f)
            {
                ObjectRotate(This->rotate_x, 1.0f, 0.0f, 0.0f);
                bObjSet = 1;
            }
            if (This->rotate_y != 0.0f)
            {
                ObjectRotate(This->rotate_y, 0.0f, -1.0f, 0.0f);
                bObjSet = 1;
            }
            if (This->rotate_z != 0.0f)
            {
                ObjectRotate(This->rotate_z, 0.0f, 0.0f, -1.0f);
                bObjSet = 1;
            }
            This->face[f].render(&This->face[f], ptex, pt);
            PopMatrix();
        }
    }
    return 0 ;
}

void Cube_Init(struct Cube* This, int _x, int _y, int _z, int px, int py, int pz)
{
    float dsize = 0.0f;
    float tu = 64.0f, tv = 64.0f;
    This->render = render;
    This->rotate_x = This->rotate_y = This->rotate_z = 0.0f;
    {
        int it;
        for (it = 0; it < 6; ++it)
        {
            Face_Init(&This->face[it], 0.0f, 0.0f, 0.0f, 0.0f, -1, 0);
        }
    }
    {
        int x = pz, y = px, z = py;
        int tx = _z, ty = _x, tz = _y;
        int findex = 5;
        if (py == 0)
        {
            Face_Init(&This->face[findex], 0, 0, tu, tv, findex, 1);
        }
        {
            int imap[][4] = {{4, 5, 6, 7}, {0, 1, 2, 3}};
            int it;
            for (it = 0; it < 4; ++it)
            {
                This->pos[imap[0][it]].y = (-0.5f + 1.0f / tz * z) * tz + 1.0f / tz * dsize;
                This->pos[imap[1][it]].y = (-0.5f + 1.0f / tz * (z + 1)) * tz - 1.0f / tz * dsize;
            }
        }
    }
    {
        int x = pz, y = px;
        int tx = _z, ty = _x, tz = _y;
        int findex = 0;
        if (py + 1 == _y)
        {
            Face_Init(&This->face[findex], 0, 0, tu, tv, findex, 1);
        }
    }
    {
        int x = px, y = py, z = pz;
        int tx = _x, ty = _y, tz = _z;
        int findex = 4;
        if (pz == 0)
        {
            Face_Init(&This->face[findex], 0, 0, tu, tv, findex, 1);
        }
        {
            int imap[][4] = {{1, 2, 6, 5}, {0, 3, 7, 4}};
            int it;
            for (it = 0; it < 4; ++it)
            {
                This->pos[imap[0][it]].z = (-0.5f + 1.0f / tz * z) * tz + 1.0f / tz * dsize;
                This->pos[imap[1][it]].z = (-0.5f + 1.0f / tz * (z + 1)) * tz - 1.0f / tz * dsize;
            }
        }
    }
    {
        int x = px, y = py;
        int tx = _x, ty = _y, tz = _z;
        int findex = 1;
        if (pz + 1 == _z)
        {
            Face_Init(&This->face[findex], 0, 0, tu, tv, findex, 1);
        }
    }
    {
        int x = py, y = pz, z = px;
        int tx = _y, ty = _z, tz = _x;
        int findex = 2;
        if (px == 0)
        {
            Face_Init(&This->face[findex], 0, 0, tu, tv, findex, 1);
        }
        {
            int imap[][4] = {{2, 3, 6, 7}, {0, 1, 4, 5}};
            int it;
            for (it = 0; it < 4; ++it)
            {
                This->pos[imap[0][it]].x = (-0.5f + 1.0f / tz * z) * tz + 1.0f / tz * dsize;
                This->pos[imap[1][it]].x = (-0.5f + 1.0f / tz * (z + 1) ) * tz- 1.0f / tz * dsize;
            }
        }
    }
    {
        int x = py, y = pz;
        int tx = _y, ty = _z, tz = _x;
        int findex = 3;
        if (px + 1 == _x)
        {
            Face_Init(&This->face[findex], 0, 0, tu, tv, findex, 1);
        }
    }
}

void Cube_Rotate(struct Cube* This, char cmd, int v)
{
    int rotatemap[3][4] = {
        {1, 3, 4, 2},
        {0, 2, 5, 3},
        {0, 4, 5, 1},
    };
    struct Face temp;
    int rmi = -1, it;
    if      (cmd == 'U') rmi = 0;
    else if (cmd == 'F') rmi = 1;
    else if (cmd == 'R') rmi = 2;
    if (v <= 0)
    {
        temp = This->face[rotatemap[rmi][3]];
        for (it = 3; it > 0; --it)
        {
            This->face[rotatemap[rmi][it]] = This->face[rotatemap[rmi][it - 1]];
        }
        This->face[rotatemap[rmi][0]] = temp;
    }
    else
    {
        temp = This->face[rotatemap[rmi][0]];
        for (it = 0; it < 3; ++it)
        {
            This->face[rotatemap[rmi][it]] = This->face[rotatemap[rmi][it + 1]];
        }
        This->face[rotatemap[rmi][3]] = temp;
    }
}

float lineCrossTriangle(VECTOR3D start, VECTOR3D end, VECTOR3D t1, VECTOR3D t2, VECTOR3D t3)
{
    float s, t, ret;
    end -= start;
    t1 -= start; t2 -= start; t3 -= start;
    start = VECTOR3D(0.0f, 0.0f, 1.0f);
    t1.Rotate(start, end);
    t2.Rotate(start, end);
    t3.Rotate(start, end);
    ret = t1.z + t2.z + t3.z;
    t1.z = 0; t2.z = 0; t3.z = 0;
    s = (t1 & t2).z;
    t = (t2 & t3).z;
    if ((s < 0) ^ (t < 0)) return -1.0f;
    s = (t3 & t1).z;
    if ((s < 0) ^ (t < 0)) return -1.0f;

    return ret;
}

pick_info Cube_pick(struct Cube* This, VECTOR3D start, VECTOR3D end)
{
    pick_info ret = {-1, 0.0f};
    pick_info retMin = {-1, 0.0f};
    int imap[6][4] =
    {
        {2, 3, 1, 0},
        {3, 7, 0, 4},
        {2, 6, 3, 7},
        {0, 4, 1, 5},
        {1, 5, 2, 6},
        {7, 6, 4, 5},
    };
    int f;
    for (f = 0; f < 6; ++f)
    {
        if (This->face[f].visable)
        {
            ret.z = lineCrossTriangle(start, end, This->pos[imap[f][0]], This->pos[imap[f][1]], This->pos[imap[f][2]]);
            if (ret.z >= 0)
            {
                ret.info = f;
                if (retMin.info < 0 || retMin.z > ret.z)
                {
                    retMin = ret;
                }
            }
            ret.z = lineCrossTriangle(start, end, This->pos[imap[f][1]], This->pos[imap[f][2]], This->pos[imap[f][3]]);
            if (ret.z >= 0)
            {
                ret.info = f;
                if (retMin.info < 0 || retMin.z > ret.z)
                {
                    retMin = ret;
                }
            }
        }
    }
    return retMin;
}
