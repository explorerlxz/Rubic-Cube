#include "stdafx.h"
/*
 * author      御坂美琴
 * E-mail      fmmmm@qq.com
 * filename    stdafx.cpp
 * date        20110105
 */

int add(pti3d* a, pti3d* b)
{
    a->x += b->x;
    a->y += b->y;
    a->z += b->z;
    return 0;
}
