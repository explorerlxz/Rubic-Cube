#include "stdafx.h"
/*
 * author      御坂美琴
 * E-mail      fmmmm@qq.com
 * filename    main.cpp
 * date        20110105
 */

#include "rubik.h"

typedef struct camera
{
    VECTOR3D pos, center, up;
}camera;

void camera_init(camera* This)
{
    This->pos = VECTOR3D(0.0f, 0.0f, 1.0f);
    This->pos.Rotate((float)(PI/4), 0.0f, 1.0f, 0.0f);
    This->pos.Rotate((float)(PI/4), -1.0f, 0.0f, 0.0f);
    This->center = VECTOR3D(0.0f, 0.0f, 0.0f);
    This->up = VECTOR3D(0.0f, 1.0f, 0.0f);
}

void camera_update(camera* This, float dx, float dy)
{
    if (dx != 0.0f || dy != 0.0f)
    {
        if (dy != 0.0f)
        {
            VECTOR3D rc = This->pos & This->up;
            This->pos.Rotate(dy, rc);
            This->pos.SetModule(1.0f);
            This->up = rc & This->pos;
            This->up.SetModule(1.0f);
        }
        if (dx != 0.0f)
        {
            This->pos.Rotate(dx, This->up);
            This->pos.SetModule(1.0f);
        }
    }
    CameraSet(&This->pos, &This->center, &This->up);
}

void sceneSelecesize(int* lmap)
{
    PIMAGE img_font;
    int nSel = 0;
    img_font = IMAGE::newimage(512, 128);
    lmap[0] = lmap[1] = lmap[2] = 0;
    for ( ; nSel < 3; delay_fps(60))
    {
        cleardevice();
        if (Begin2d() == 0)
        {
            if (kbhit())
            {
                int key = getch();
                if (key >= '2' && key <= '9')
                {
                    lmap[nSel++] = key - '0';
                }
            }
            {
                char str[128];
                img_font->cleardevice();
                img_font->setfont(32, 0, "宋体");
                sprintf(str, "请按下2-9的数字三次\n以确定魔方尺寸：\n %d x %d x %d", lmap[0], lmap[1], lmap[2]);
                img_font->outtextrect(0, 0, 800, 600, str);
            }
            {
                HTEXTURE tex;
                tex = CreateTexture(img_font);
                SelectTexture(tex);
                RenderTextureXYXYWH(100, 100);
                SelectTexture(NULL);
                ReleaseTexture(tex);
            }
            EndRender();
        }
    }
    img_font->release();
}

void mainScene()
{
    struct Rubik * r = (struct Rubik*) malloc(sizeof(struct Rubik));
    HTEXTURE ptex[6];
    PIMAGE img_surf[6];
    PIMAGE img_title;
    camera cma;
    camera_init(&cma);
    int colormap[] = {0xFFFF, 0x4000FF, 0xFF0000, 0xAF00, 0x60FF, 0xFFFFFF};
    int lmap [] = {3, 3, 3};
    float camera_z = 10.0f;
    POINT mousepos;
    int mouse_state = 0;
    int text_color = 0;
    int pick_ret = 0;

    sceneSelecesize(lmap);
    Rubik_Init(r, lmap[0], lmap[1], lmap[2]);
    img_title = IMAGE::newimage(1024, 64);
    img_title->setfont(32, 0, "宋体");

    for (int it = 0; it < 6; ++it)
    {
        img_surf[it] = IMAGE::newimage(64, 64);
        img_surf[it]->cleardevice();
        img_surf[it]->setfillstyle(colormap[it]);
        img_surf[it]->bar(4, 4, 60, 60);
        for ( int i = 0; i < 8; ++i)
        {
            img_surf[it]->imagefilter_blurring(0xFF, 0x100);
        }
        ptex[it] = CreateTexture(img_surf[it]);
    }

    for ( ; ; delay_fps(60))
    {
        VECTOR3D mouse_pos;
        int iLButton = 0;
        float mouse_dx = 0.0f, mouse_dy = 0.0f;

        text_color = (text_color + 1) % 360;
        if (kbhit())
        {
            int key = getch();
            if (key == 27) break; // ESC键退回
        }
        while (MouseHit())
        {
            MOUSEMSG msg = GetMouseMsg(0);
            if (msg.uMsg == WM_MOUSEWHEEL)
            {
                camera_z += (float)(msg.wheel / -120.0 * 0.5);
            }
            else if (msg.uMsg == WM_LBUTTONDOWN)
            {
                iLButton = 1;
                mouse_pos.x = msg.x;
                mouse_pos.y = msg.y;
            }
            else if (msg.uMsg == WM_LBUTTONUP)
            {
                iLButton = 2;
                mouse_pos.x = msg.x;
                mouse_pos.y = msg.y;
            }
            else if (msg.uMsg == WM_RBUTTONDOWN)
            {
                if (mouse_state == 0)
                {
                    mouse_state = 1;
                    mousepos.x = msg.x;
                    mousepos.y = msg.y;
                }
            }
            else if (msg.uMsg == WM_RBUTTONUP)
            {
                mouse_state = 0;
            }
            else if (msg.uMsg == WM_MOUSEMOVE)
            {
                if (mouse_state == 1)
                {
                    float div = 50.0f;
                    mouse_dx = (float)((msg.x - mousepos.x) / -div), mouse_dy = (float)((msg.y - mousepos.y) / div);
                    mousepos.x = msg.x;
                    mousepos.y = msg.y;
                }
            }
        }

        cleardevice();
        if (Begin3d(45.0f) == 0)
        {
            camera_update(&cma, mouse_dx, mouse_dy);
            CameraMove(0.0f, -0.0f, -camera_z);
            if (iLButton)
            {
                VECTOR3D start, end;
                GetPickRay(&mouse_pos, &start, &end);
                pick_ret = Rubik_pick(r, start, end, iLButton);
            }
            Rubik_Update(r);
            r->render(r, ptex);
            EndRender();
        }
        if (Begin2d() == 0)
        {
            char str[128];
            sprintf(str, "Super %d x %d x %d Rubik's Cube by 御坂美琴", lmap[0], lmap[1], lmap[2]);
            img_title->cleardevice();
            img_title->setcolor(HSVtoRGB((float)text_color, 1.0f, 1.0f));
            img_title->outtextxy(0, 0, str);
            HTEXTURE tex = CreateTexture(img_title, 0xFF000000);
            SelectTexture(tex);
            RenderTextureXYXYWH(20, 10);
            ReleaseTexture(tex);
            EndRender();
        }
    }
    img_title->release();
    for (it = 0; it < 6; ++it)
    {
        ReleaseTexture(ptex[it]);
        img_surf[it]->release();
    }
    Rubik_Release(r);
    free(r);
}

int main()
{
    initgraph(800, 600);

    randomize();

    Enable3D(EXST_CULL_FACE, TRUE);
    while (1)
    {
        mainScene();
    }

    closegraph();
    return 0;
}
