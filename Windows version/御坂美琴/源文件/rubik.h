#include "stdafx.h"
/*
 * author      御坂美琴
 * E-mail      fmmmm@qq.com
 * filename    rubik.h
 * date        20110105
 */

#include "dimension.h"

typedef struct dim_layer
{
    int ndim;
    int nlayer;
    float faceval;
}dim_layer;

struct Rubik
{
    int (*render)(struct Rubik* This, HTEXTURE* ptex);
    struct Dimension d_x, d_y, d_z;
    struct Cube* pAllCube;
    int xyz;
    int mx, my, mz;
    int ncnt;
    int pick_st_id, pick_st_f;
    int pick_ed_id, pick_ed_f;
};

void Rubik_Init(struct Rubik* This, int _x, int _y, int _z);
void Rubik_Rotate(struct Rubik* This, char cmd, int ilayer, int v);
void Rubik_Release(struct Rubik* This);
int Rubik_pick(struct Rubik* This, VECTOR3D start, VECTOR3D end, int style);
int Rubik_finddim(struct Rubik* This, struct Cube* pcube, int face, dim_layer* retd);
int Rubik_Update(struct Rubik* This);
