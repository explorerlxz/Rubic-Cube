#include "stdafx.h"
/*
 * author      御坂美琴
 * E-mail      fmmmm@qq.com
 * filename    cube.h
 * date        20110105
 */

#include "face.h"

/*
    0
     4
2  -/-  3
   1
    5

  2      1
3      0

  6      5
7      4

*/

struct Cube
{
    int (*render) (struct Cube* This, HTEXTURE* ptex);
    VECTOR3D pos[8];
    struct Face face[6];
    float rotate_x, rotate_y, rotate_z;
};

void Cube_Init(struct Cube* This, int _x, int _y, int _z, int px, int py, int pz);
void Cube_Rotate(struct Cube* This, char cmd, int v);
pick_info Cube_pick(struct Cube* This, VECTOR3D start, VECTOR3D end);
