#include "stdafx.h"
/*
 * author      御坂美琴
 * E-mail      fmmmm@qq.com
 * filename    face.h
 * date        20110105
 */

struct Face
{
    int (*render) (struct Face* This, HTEXTURE* ptex, FPOINT3D * pt);
    float ltu, ltv, rbu, rbv;
    int visable;
    int faceindex;
};

void Face_Init(struct Face* This, float ltu, float ltv, float rbu, float rbv, int findex, int visable);
void setRender(int n);
int getRender();
