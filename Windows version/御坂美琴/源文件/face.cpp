#include "stdafx.h"
/*
 * author      御坂美琴
 * E-mail      fmmmm@qq.com
 * filename    face.cpp
 * date        20110105
 */

#include "face.h"

static int render(struct Face* This, HTEXTURE* ptex, FPOINT3D * pt)
{
    int it;
    FTPOINT3D pf[4];
    if (This->visable == 0) return 0;
    for (it = 0; it < 4; ++it)
    {
        memcpy(&pf[it], &pt[it], sizeof(FPOINT3D));
    }
    pf[0].u = This->ltu;
    pf[0].v = This->ltv;
    pf[1].u = This->ltu;
    pf[1].v = This->rbv;
    pf[2].u = This->rbu;
    pf[2].v = This->ltv;
    pf[3].u = This->rbu;
    pf[3].v = This->rbv;
    SelectTexture(ptex[This->faceindex], EXTEX_LINEAR);

    return RenderTextureStrip(4, pf);
}

void Face_Init(struct Face* This, float ltu, float ltv, float rbu, float rbv, int findex, int visable)
{
    This->render = render;
    This->ltu = ltu;
    This->ltv = ltv;
    This->rbu = rbu;
    This->rbv = rbv;
    This->faceindex = findex;
    This->visable = visable;
}
