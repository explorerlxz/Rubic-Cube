/*********************************************************
* PowerEasyX3D Library by rtGirl
* Version 2010 1201  V0.1.0 Alpha
* HomePage http://easyx.uueasy.com
*
* FileName: pex3d.h
* �� VC ��ģ�� Borland BGI ��ͼ�⣬ʵ�ּ򵥵Ļ�ͼ֮�࣬��չ3dͼ��
*
* ������ʹ�ñ���ʱ����ò�Ҫ����conio.hͷ�ļ�
* ʹ�ñ��⣬������C++���룬��֧�ֵı�������
* VC6/VC2005/VC2008/VC2010/MinGW3.4.5/MinGW4.4.1
*********************************************************/

/****************************************************************************
** ע�����
* �������Ҫ��ʾ����̨���ڣ����ڰ������ļ���ǰ���һ��define SHOW_CONSOLE
* �����Sleep���APIʱ�����ߵ���delay��ʵ�ʾ���ת��Ϊ����delay_ms
* ��delay_ms(0)�������ж���û�и��µı�Ҫ��������ε��������ĵ��ò��������֡�ʵ�Ӱ��
* �����delay_ms, delay_fps, getch, GetMouseMsg ʱ���������ݿ��ܻ���£���Щ�����൱��������Render3d()��
*   �����ֻ��Ҫ���´��ڣ�������ȴ������ֱ�ӵ���delay_ms(0)����Render3d();
* ���������е�3Dͼ�κ���������ѧ���Դ����õ���������ϵ��x���ң�y���ϣ�z����Ļ�⣨Ĵָ˳z����������ָ���x->y����ת����
*       y
*       |__ x
*      /
*     z
****************************************************************************/

#ifndef _graph_easyx3d_h_
#define _graph_easyx3d_h_

#pragma once

#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif

#ifndef _CRT_SECURE_NO_DEPRECATE
#define _CRT_SECURE_NO_DEPRECATE
#endif

#include "windows.h"

#define _USE_MATH_DEFINES
#include <math.h>

#ifndef __cplusplus
#error You must use C++ compiler
#endif

#if defined(_INC_CONIO) || defined(_CONIO_H_)
#error can not include "conio.h" before "easyx3d.h"
#endif

#if !defined(_GRAPH_LIB_BUILD_) && !defined(_GRAPH_NO_LIB_)
    #ifdef _MSC_VER
        #ifdef _STATIC_LIB_
            #ifdef _DEBUG
                #pragma comment(lib,"libpex3dd.lib")
            #else
                #pragma comment(lib,"libpex3d.lib")
            #endif
            #if _MSC_VER >= 1200
                #ifdef _DEBUG
                    #pragma comment(linker, "/NODEFAULTLIB:libcmtd.lib")
                    #pragma comment(linker, "/NODEFAULTLIB:MSVCRT.lib")
                    #pragma comment(linker, "/NODEFAULTLIB:libcmt.lib")
                    #pragma comment(linker, "/NODEFAULTLIB:libc.lib")
                #elif _MSC_VER < 1400
                    #pragma comment(linker, "/NODEFAULTLIB:libcmt.lib")
                #endif
            #else
                #ifdef _DEBUG
                    #pragma comment(linker, "/NODEFAULTLIB:libc.lib")
                #endif
            #endif
        #else
            #pragma comment(lib,"pex3d.lib")
        #endif
    #endif
#endif


#define PI                  3.1415926535897932384626
#define GetAValue(rgb)      ((BYTE)((rgb)>>24))
#define RGBA(r, g, b, a)    (RGB(r, g, b) | ((COLORREF)(a) << 24))
#define RGBTOBGR(color)     ((((color) & 0xFF) << 16) | (((color) & 0xFF0000) >> 16) | ((color) & 0xFF00FF00))
#define INITGRAPH(x, y)     struct _initgraph_{_initgraph_(){initgraph(x, y);}\
    ~_initgraph_(){closegraph();}}_g_initgraph_
#define INITGRAPH3(x, y, f) struct _initgraph_{_initgraph_(){initgraph(x, y, f);}\
    ~_initgraph_(){closegraph();}}_g_initgraph_

#if defined(_GRAPH_NO_LIB_) || (defined(_GRAPH_LIB_BUILD_) && !defined(_USRDLL))
#define _STATIC_LIB_
#endif

#ifdef _STATIC_LIB_
    #define DLLEXPORT
#else
    #ifdef _GRAPH_LIB_BUILD_
        #define DLLEXPORT __declspec(dllexport)
    #else
        #define DLLEXPORT __declspec(dllimport)
    #endif
#endif


namespace PEX3D {

enum graphics_errors {      /* graphresult error return codes */
    grOk                =   0,
    grNoInitGraph       =  -1,
    grNotDetected       =  -2,
    grFileNotFound      =  -3,
    grInvalidDriver     =  -4,
    grNoLoadMem         =  -5,
    grNoScanMem         =  -6,
    grNoFloodMem        =  -7,
    grFontNotFound      =  -8,
    grNoFontMem         =  -9,
    grInvalidMode       = -10,
    grError             = -11,   /* generic error */
    grIOerror           = -12,
    grInvalidFont       = -13,
    grInvalidFontNum    = -14,
    grInvalidVersion    = -18,
    grException         = 0x10,  /* ege error */
    grParamError        = 0x11,
    grInvalidRegion     = 0x12,
    grOutOfMemory       = 0x13,
    grNullPointer       = 0x14,
    grAllocError        = 0x15,
    grMultiSample       = 0xFF,
    grInvalidMemory     = 0xCDCDCDCD,
};

typedef enum EX3D_settings {
    EX3D_ANTIALIASING0      = 0, //���������
    EX3D_ANTIALIASING1      = 1, //2X�����
    EX3D_ANTIALIASING2      = 2, //4X�����
    EX3D_ANTIALIASING3      = 3, //8X�����
    EX3D_ANTIALIASINGMASK   = 3,
    EX3D_VSYNC              = 0x80000000, //������ֱͬ��
}EX3D_SETTINGS;

typedef enum EXTEX_settings {
    EXTEX_ANISOTROPY0       = 0, //�����������Թ���
    EXTEX_ANISOTROPY1       = 1, //2X�������Թ��ˣ������ֵ��ʵ��ֵ��Ӳ����֧�־�����
    EXTEX_ANISOTROPY2       = 2, //3X�������Թ���
    EXTEX_ANISOTROPY3       = 3, //4X�������Թ���
    EXTEX_ANISOTROPY4       = 4, //5X�������Թ���
    EXTEX_ANISOTROPY5       = 5, //6X�������Թ���
    EXTEX_ANISOTROPY6       = 6, //7X�������Թ���
    EXTEX_ANISOTROPY7       = 7, //8X�������Թ���
    EXTEX_ANISOTROPY8       = 8, //9X�������Թ���
    EXTEX_ANISOTROPY9       = 9, //10X�������Թ���
    EXTEX_ANISOTROPYa       = 10, //11X�������Թ���
    EXTEX_ANISOTROPYb       = 11, //12X�������Թ���
    EXTEX_ANISOTROPYc       = 12, //13X�������Թ���
    EXTEX_ANISOTROPYd       = 13, //14X�������Թ���
    EXTEX_ANISOTROPYe       = 14, //15X�������Թ���
    EXTEX_ANISOTROPYf       = 15, //16X�������Թ���
    EXTEX_ANTIALIASINGMASK  = 0xF,
    EXTEX_MIPMAP            = 0x10, //����mipmap
    EXTEX_LINEAR            = 0x20, //�������Թ��ˣ���ͬʱ����EXTEX_MIPMAP_LINEAR��Ϊ�����Թ���
    EXTEX_MIPMAP_LINEAR     = 0x50, //�����������Թ��ˣ��Զ���EXTEX_MIPMAPͬʱʹ��
}EXTEX_SETTINGS;


typedef enum points_style {
    EXP_POINTS          = 0,
    EXP_LINES           = 1,
    EXP_LINESTRIP       = 3,
    EXP_TRIANGLES       = 4,
    EXP_TRIANGLESTRIP   = 5,
    EXP_TRIANGLEFAN     = 6,
}POINTS_STYLE;

typedef enum light_setting {
    EXL_LIGHT0          = 0x4000,
    EXL_LIGHT1          = 0x4001,
    EXL_LIGHT2          = 0x4002,
    EXL_LIGHT3          = 0x4003,
    EXL_LIGHT4          = 0x4004,
    EXL_LIGHT5          = 0x4005,
    EXL_LIGHT6          = 0x4006,
    EXL_LIGHT7          = 0x4007,

    //������
    EXL_AMBIENT         = 0x1200, //�����⣬color * 4 (R, G, B, A) ��ͬ��Ĭ��(0,0,0,1)��Aλ����Ϊ1����������Ӧ�������
    EXL_DIFFUSE         = 0x1201, //�����䣬color * 4 Ĭ��1,1,1,1
    EXL_SPECULAR        = 0x1202, //�߹⣬color * 4 Ĭ��1,1,1,1

    EXL_POSITION        = 0x1203, //��Դλ�ã�pos * 4 (x, y, z, w) Ĭ��0,0,1,0����wΪ0��ʾ����˥������⣬����wΪ1

    EXL_SPOT_DIRECTION  = 0x1204, //�ⷽ��vector * 3 (dx, dy, dz) (��EXL_SPOT_CUTOFF=180ʱʹ����Ч) Ĭ��0,0,1
    EXL_SPOT_EXPONENT   = 0x1205, //�۹�ָ�� value * 1 (0-128)  Ĭ��0
    EXL_SPOT_CUTOFF     = 0x1206, //���շ�Χ�Ƕ� angle * 1 (0-90, 180) Ĭ��180�������Դ
    EXL_CONSTANT_ATTENUATION = 0x1207, //����˥������ value * 1 (0-128)  Ĭ��1
    EXL_LINEAR_ATTENUATION   = 0x1208, //����˥������ value * 1 (0-128)  Ĭ��1

    EXL_FINITY_FAR          = 0x0B51, //���޾����Դ��bool * 1��false��ʾ����Զ��ƽ�й⣬Ĭ�ϣ�
    EXL_MODEL_TWO_SIDE      = 0x0B52, //˫���Դ����������գ�bool * 1��false��ʾ��˫��⣨Ĭ�ϣ�
    EXL_LIGHT_AMBIENT       = 0x0B53, //����������ռ�ı�����color*4��Ĭ��0xFF333333
    EXL_LIGHT_COLOR_CONTROL = 0x81F8,
}LIGHT_SETTING;

typedef enum material_setting {
    EXM_FRONT           = 0x0404,
    EXM_BACK            = 0x0405,
    EXM_FRONT_AND_BACK  = 0x0408,

    EXM_AMBIENT     = 0x1200, // �����⣬Ĭ��0xFF333333
    EXM_DIFFUSE     = 0x1201, // ����⣬Ĭ��0xFFCCCCCC
    EXM_SPECULAR    = 0x1202, // �߹⣬Ĭ��0xFF000000
    EXM_EMISSION    = 0x1600, // �Է��⣬Ĭ��0xFF000000
    EXM_SHININESS   = 0x1601, // �߹�ָ�� 0-128��Ĭ��0
}MATERIAL_SETTING;

typedef enum clear_flag {
    EXC_CLEAR_TARGET    = 0x4000,
    EXC_CLEAR_ZBUFFER   = 0x100,
    //EXC_CLEAR_STENCIL   = 4, //δ��
    EXC_CLEAR_ALL       = 3,
}CLEAR_FLAG;

enum key_msg_flag {
    KEYMSG_CHAR_FLAG    = 2,
    KEYMSG_DOWN_FLAG    = 1,
    KEYMSG_UP_FLAG      = 1,

    KEYMSG_CHAR         = 0x40000,
    KEYMSG_DOWN         = 0x10000,
    KEYMSG_UP           = 0x20000,
    KEYMSG_FIRSTDOWN    = 0x80000,
};

// ��ɫ
enum COLORS {
    BLACK           = 0,
    BLUE            = 0xA80000,
    GREEN           = 0x00A800,
    CYAN            = 0xA8A800,
    RED             = 0x0000A8,
    MAGENTA         = 0xA800A8,
    BROWN           = 0x0054A8,
    LIGHTGRAY       = 0xA8A8A8,
    DARKGRAY        = 0x545454,
    LIGHTBLUE       = 0xFC5454,
    LIGHTGREEN      = 0x54FC54,
    LIGHTCYAN       = 0xFCFC54,
    LIGHTRED        = 0x5454FC,
    LIGHTMAGENTA    = 0xFC54FC,
    YELLOW          = 0x54FCFC,
    WHITE           = 0xFCFCFC
};

// ���ģʽ
enum fill_patterns {
    EMPTY_FILL,
    NULL_FILL,          // �����
    SOLID_FILL,         // ��ָ����ɫ��ȫ���
};

enum text_just {        /* Horizontal and vertical justification
                        for settextjustify */
    LEFT_TEXT   = 0,
    CENTER_TEXT = 1,
    RIGHT_TEXT  = 2,

    BOTTOM_TEXT = 0,
    /* CENTER_TEXT = 1,  already defined above */
    TOP_TEXT    = 2
};

enum line_styles {      /* Line styles for get/setlinestyle */
    SOLID_LINE   = 0,
    DOTTED_LINE  = 1,
    CENTER_LINE  = 2,
    DASHED_LINE  = 3,
    USERBIT_LINE = 4,   /* User defined line style */
};

enum Enable3D_State {
    EXST_ZTEST          = 0x0B71,
    EXST_CULL_FACE      = 0x0B44,
    EXST_ALPHABLEND     = 0x0BE2,
    EXST_MULTISAMPLE    = 0x809D,
    EXST_VSYNC          = 0xF011,
};

class DLLEXPORT IMAGE;
typedef IMAGE  *PIMAGE;

struct viewporttype {
    int left;
    int top;
    int right;
    int bottom;
    int clipflag;
};

struct textsettingstype {
    int font;
    int direction;
    int charsize;
    int horiz;
    int vert;
};

struct linestyletype {
    int linestyle;
    unsigned short upattern;
    int thickness;
};

struct DLLEXPORT TEXTURE;

typedef UINT HTEXTURE;

struct VECTOR3D;

// 3d ���㸨������
DLLEXPORT void rotate_point3d_x(VECTOR3D  * pt, float r); //���ȣ����ֶ���
DLLEXPORT void rotate_point3d_y(VECTOR3D  * pt, float r);
DLLEXPORT void rotate_point3d_z(VECTOR3D  * pt, float r);

struct DLLEXPORT VECTOR3D {
    float x, y, z;
    VECTOR3D() {
        x = 0; y = 0; z = 0;
    }
    VECTOR3D(float _x, float _y) {
        x = _x; y = _y; z = 0;
    }
    VECTOR3D(float _x, float _y, float _z) {
        x = _x; y = _y; z = _z;
    }
    VECTOR3D& operator = (const VECTOR3D& _fp)  {
        x = _fp.x; y = _fp.y; z = _fp.z;
        return *this;
    }
    VECTOR3D& operator += (const VECTOR3D& _fp);
    VECTOR3D& operator -= (const VECTOR3D& _fp);
    VECTOR3D operator + (const VECTOR3D& _fp) const;
    VECTOR3D operator - (const VECTOR3D& _fp) const;
    VECTOR3D& operator *= (float f); //����
    VECTOR3D operator * (float f) const; //����
    float operator * (const VECTOR3D& _fp) const; //���
    VECTOR3D operator & (const VECTOR3D& _fp) const; //���
    VECTOR3D& operator &= (const VECTOR3D& _fp); //���
    float GetModule() const {
        return (float)sqrt(x*x + y*y + z*z);
    }
    float GetSqrModule() const {
        return float(x*x + y*y + z*z);
    }
    VECTOR3D& SetModule(float m) {
        float t = m / GetModule();
        *this *= t;
        return *this;
    }
    VECTOR3D& Rotate(float rad, const VECTOR3D& v); //����������ת�����ֶ���radΪ����
    VECTOR3D& Rotate(float rad, float x, float y, float z) {
        VECTOR3D v(x, y, z);
        return Rotate(rad, v);
    }
    VECTOR3D& Rotate(const VECTOR3D& e, const VECTOR3D& s = VECTOR3D(0.0f, 0.0f, 1.0f)); //��s��e֮��ļн�ȷ����ת
    static float GetAngel(const VECTOR3D& e, const VECTOR3D& s = VECTOR3D(0.0f, 0.0f, 1.0f));
};

struct DLLEXPORT FPOINT3D : public VECTOR3D {
    COLORREF color;
    FPOINT3D() : VECTOR3D() {
        color = 0;
    }
    FPOINT3D(float _x, float _y) : VECTOR3D(_x, _y) {
        color = 0;
    }
    FPOINT3D(float _x, float _y, float _z) : VECTOR3D(_x, _y, _z) {
        color = 0;
    }
    FPOINT3D(float _x, float _y, float _z, COLORREF _c) : VECTOR3D(_x, _y, _z) {
        color = _c;
    }
    FPOINT3D(const FPOINT3D& _fp) {
        x = _fp.x;
        y = _fp.y;
        z = _fp.z;
        color = _fp.color;
    }
    FPOINT3D& operator = (const FPOINT3D& _fp) {
        x = _fp.x;
        y = _fp.y;
        z = _fp.z;
        color = _fp.color;
        return *this;
    }
};

struct DLLEXPORT FTPOINT3D : public FPOINT3D{
    float u, v;
    FTPOINT3D() : FPOINT3D() {
        color = 0;
        u = v = 0.0f;
    }
    FTPOINT3D(float _x, float _y, float _z) : FPOINT3D(_x, _y, _z) {
        color = 0;
        u = v = 0.0f;
    }
    FTPOINT3D(float _x, float _y, float _z, COLORREF _c) : FPOINT3D(_x, _y, _z, _c) {
        color = 0;
        u = v = 0.0f;
    }
    FTPOINT3D(float _x, float _y, float _z, COLORREF _c, float _u, float _v) : FPOINT3D(_x, _y, _z, _c) {
        u = _u; v = _v;
    }
    FTPOINT3D(const FTPOINT3D& _fp) {
        x = _fp.x;
        y = _fp.y;
        z = _fp.z;
        color = _fp.color;
        u = _fp.u;
        v = _fp.v;
    }
    FTPOINT3D& operator = (const FTPOINT3D& _fp) {
        x = _fp.x;
        y = _fp.y;
        z = _fp.z;
        color = _fp.color;
        u = _fp.u;
        v = _fp.v;
        return *this;
    }
};

DLLEXPORT FPOINT3D  LinearPoint3D (FPOINT3D  fpD, FPOINT3D  fpS, float alpha);
DLLEXPORT FTPOINT3D LinearTPoint3D(FTPOINT3D fpD, FTPOINT3D fpS, float alpha);


typedef VECTOR3D    *PVECTOR3D;
typedef FPOINT3D    *PFPOINT3D;
typedef FTPOINT3D   *PFTPOINT3D;


typedef struct MOUSEMSG {
    UINT uMsg;              // ��ǰ�����Ϣ
    bool mkCtrl;            // Ctrl ���Ƿ���
    bool mkShift;           // Shift ���Ƿ���
    bool mkLButton;         // �������Ƿ���
    bool mkMButton;         // ����м��Ƿ���
    bool mkRButton;         // ����Ҽ��Ƿ���
    short x;                // ��ǰ��� x ����
    short y;                // ��ǰ��� y ����
    short wheel;            // �����ֹ���ֵ(120�ı���)
}MOUSEMSG;




//--------------------------------------------------------------------------------------------------
// ��ͼ��������
DLLEXPORT void setinitmode(int mode, int x = CW_USEDEFAULT, int y = CW_USEDEFAULT); // �������ֻ����initgraph����ǰ����
DLLEXPORT void initgraph(int Width, int Height, int Flag = EX3D_ANTIALIASING0);
DLLEXPORT void closegraph();

DLLEXPORT int  cleardevice(CLEAR_FLAG flag = EXC_CLEAR_ALL); //������������ѡ������������flag&1�Ǿ�����Ļ�����flag&2�Ǿ���zbuffer
DLLEXPORT int  SetScreenLogicalSize(int width, int height); // ���µ��������߼���С�����ı������С����������BeginXd/EndRender֮����ã�����������
DLLEXPORT void SetClearColor(COLORREF color = 0xFF000000);

//��ͼ״̬���ƺ���

DLLEXPORT int  Render3d(); //�˺���һ�㲻Ҫֱ�ӵ���

DLLEXPORT int  Begin2d(int dx = 0, int dy = 0); // ����Ϊ�������Ͻ�����
DLLEXPORT int  Begin3d(float viewdeg = 90.0f, float zNear = 0.1f, float zFar = 100.0f); //����Ϊ��������ӽǶ�(y����)�������棬Զ���棬������Ϊ����
DLLEXPORT void EndRender();

DLLEXPORT int  SetViewPort(int x, int y, int w, int h); //��BeginXd/EndRender֮�����ã������ط���Ч

DLLEXPORT void Enable3D(int state, BOOL bSet); //��Enable3D_State

//��ͼ���������º���ֻ����BeginXd/EndRender֮����ã������ͼ��Ч���߽����ȷ��

// 2d
DLLEXPORT int Line2D(COLORREF color1, float x1, float y1, COLORREF color2, float x2, float y2);
    //������������Ҫ���SelectTexture
    //srcW,srcH���Ϊ0.0f����ʹ�ñ�������Ĵ�С������ʱsrcX��srcY��ΪĬ��ֵ���Խ�磬Խ��ᰴ�����ü�
DLLEXPORT int RenderTextureXYXYWH(float destX, float destY, float falpha = 1.0f, float srcX = 0.0f, float srcY = 0.0f, float srcW = 0.0f, float srcH = 0.0f);
DLLEXPORT int RenderTextureXYWHXYWH(float destX, float destY, float destW, float destH,
                                    float falpha = 1.0f,
                                    float srcX = 0.0f, float srcY = 0.0f, float srcW = 0.0f, float srcH = 0.0f);
DLLEXPORT int RenderTextureXYRCXYWH(float destX, float destY, float rad,
                                    float srcCenterx, float srcCentery,
                                    float falpha = 1.0f, float scalex = 1.0f, float scaley = 1.0f,
                                    float srcX = 0.0f, float srcY = 0.0f, float srcW = 0.0f, float srcH = 0.0f);
DLLEXPORT int RenderImage(PIMAGE pimg, int flag = 0); //��IMAGE������ͼ�����������������BeginXd/EndRender֮����ã�flag�μ�SelectTexture
DLLEXPORT int RenderImage(PIMAGE pimg, int x, int y, int w = 0, int h = 0, int flag = 0); //��IMAGEָ������������ͼ�����������������BeginXd/EndRender֮�����

//3d
DLLEXPORT int Line3D(COLORREF color1, float x1, float y1, float z1, COLORREF color2, float x2, float y2, float z2);

//2d3d���ɣ�2dʱ���z����Ϊ1.0f
DLLEXPORT int Point(COLORREF color, float x, float y, float z = 1.0f);
DLLEXPORT int Point(int nPoint, FPOINT3D *pt, int style = EXP_POINTS);
    //�����庯����Ҫ���SelectTexture
DLLEXPORT int RenderTextureTriangle(FTPOINT3D ftA, FTPOINT3D ftB, FTPOINT3D ftC);
DLLEXPORT int RenderTextureQuad(FTPOINT3D ftLT, FTPOINT3D ftLB, FTPOINT3D ftRB, FTPOINT3D ftRT);
DLLEXPORT int RenderTextureStrip(int nPoint, FTPOINT3D *pt, BOOL bLoop = FALSE);

DLLEXPORT int ScreenToTexture(int destX, int destY, int srcX, int srcY, int srcW, int srcH);
DLLEXPORT int TextureToImage(PIMAGE pimg, int destX, int destY, int srcX, int srcY, int srcW, int srcH);


//ͼ����Դ����
DLLEXPORT HTEXTURE CreateTexture(PIMAGE pimg, COLORREF colorkey = 0x0); //colorkey = 0xFFFFFF using original alpha (fast). using 0xFF000000 set as black
DLLEXPORT int ReleaseTexture(HTEXTURE hTexture);
DLLEXPORT int SelectTexture(HTEXTURE hTexture, int flag = EXTEX_ANISOTROPY0); //flagʹ��EXTEX_settings�еı�����ͬһ�������selectʹ����ͬ��flag����ô���õ�����
DLLEXPORT int GetTextureWidth (HTEXTURE hTexture);
DLLEXPORT int GetTextureHeight(HTEXTURE hTexture);


//������任����
DLLEXPORT void CameraReset(float viewdeg = 90.0f, float zNear = 0.1f, float zFar = 100.0f);
DLLEXPORT void CameraScale(float x, float y, float z);
DLLEXPORT void CameraMove(float dx, float dy, float dz);
DLLEXPORT void CameraRotate (float rad, float x, float y, float z); //(x,y,z) Ϊ��ת�ᣬ���ֶ���radΪ����
DLLEXPORT void CameraRotateD(float angle, float x, float y, float z); //(x,y,z) Ϊ��ת�ᣬ���ֶ���angleΪ�Ƕ�
DLLEXPORT void CameraSet(VECTOR3D* pos, VECTOR3D* center, VECTOR3D* up);
DLLEXPORT void GetPickRay(const VECTOR3D* mouse, VECTOR3D* start, VECTOR3D* end);
DLLEXPORT void ObjectReset();
DLLEXPORT void ObjectScale(float x, float y, float z);
DLLEXPORT void ObjectMove(float dx, float dy, float dz);
DLLEXPORT void ObjectRotate (float rad, float x, float y, float z); //(x,y,z) Ϊ��ת�ᣬ���ֶ���radΪ����
DLLEXPORT void ObjectRotateD(float angle, float x, float y, float z); //(x,y,z) Ϊ��ת�ᣬ���ֶ���angleΪ�Ƕ�
DLLEXPORT void PushMatrix();
DLLEXPORT void PopMatrix();

//���պ���
//DLLEXPORT void LightEnable(BOOL set); //�Ǳ�Ҫ��Ҫ�ֹ����ã�ֻҪ��LightTurnOn�ص�����
DLLEXPORT int  LightTurnOn(LIGHT_SETTING id, BOOL set);
DLLEXPORT int  LightTypeSet(LIGHT_SETTING id, LIGHT_SETTING type, float* param); //��֧��EXL_FINITY_FAR��EXL_MODEL_TWO_SIDE
DLLEXPORT int  LightTypeSeti(LIGHT_SETTING id, LIGHT_SETTING type, COLORREF param); //ֻ��ʹ��EXL_AMBIENT,EXL_DIFFUSE,EXL_SPECULAR����Ӧ�ֽ�Ϊ0xFF����1.0f����ʽABGR
DLLEXPORT int  LightTypeSetf(LIGHT_SETTING id, LIGHT_SETTING type, float param); //ֻ��ʹ��EXL_SPOT_EXPONENT,EXL_SPOT_CUTOFF,EXL_CONSTANT_ATTENUATION,EXL_LINEAR_ATTENUATION
DLLEXPORT int  LightSet(LIGHT_SETTING type, unsigned int param); //�����й�Դ��Ч��ʹ��EXL_FINITY_FAR��EXL_MODEL_TWO_SIDE������ֻȡ0/1��ʹ��EXL_LIGHT_AMBIENT����������ɫ
DLLEXPORT int  MaterialSet(MATERIAL_SETTING face, MATERIAL_SETTING type, COLORREF param); //���ò���
DLLEXPORT int  MaterialSpecular(BOOL set); //���ʸ߷���Ч��









//-----------------------------------------------------------------------------------------------
// IMAGE(PEXģʽ)��ͼ��������

DLLEXPORT void cleardevice(PIMAGE pimg);                   // ����

DLLEXPORT void getviewport(PIMAGE pimg, int *pleft, int *ptop, int *pright, int *pbottom, int *pclip = 0); // ��ȡ��ͼ��Ϣ
DLLEXPORT void setviewport(PIMAGE pimg, int left, int top, int right, int bottom, int clip = 1);           // ������ͼ
DLLEXPORT void clearviewport(PIMAGE pimg);                                                                     // �����ͼ

DLLEXPORT void getlinestyle(PIMAGE pimg, int *plinestyle, WORD *pupattern = NULL, int *pthickness = NULL); // ��ȡ��ǰ����
DLLEXPORT void setlinestyle(PIMAGE pimg, int linestyle, WORD upattern = NULL, int thickness = 1);          // ���õ�ǰ����
//void getfillstyle(PIMAGE pimg, COLORREF *pcolor, int *ppattern = NULL, char *pupattern = NULL);          // ��ȡ������� ###
DLLEXPORT void setfillstyle(PIMAGE pimg, COLORREF color, int pattern = SOLID_FILL, const char *pupattern = NULL);  // �����������

DLLEXPORT void setwritemode(PIMAGE pimg, int mode);        // ���û�ͼλ����ģʽ

DLLEXPORT COLORREF getcolor(PIMAGE pimg);                  // ��ȡ��ǰ��ͼǰ��ɫ
DLLEXPORT void setcolor(PIMAGE pimg, COLORREF color);      // ���õ�ǰ��ͼǰ��ɫ
DLLEXPORT COLORREF getbkcolor(PIMAGE pimg);                // ��ȡ��ǰ��ͼ����ɫ
DLLEXPORT void setbkcolor(PIMAGE pimg, COLORREF color);    // ���õ�ǰ��ͼ����ɫ
DLLEXPORT void setbkcolor_f(PIMAGE pimg, COLORREF color);    // ���õ�ǰ��ͼ����ɫ
DLLEXPORT void setfontbkcolor(PIMAGE pimg, COLORREF color);// ���õ�ǰ���ֱ���ɫ

DLLEXPORT void setbkmode(PIMAGE pimg, int iBkMode);        // ���ñ������ģʽ

// ��ɫģ��ת������
DLLEXPORT COLORREF RGBtoGRAY(COLORREF rgb);
DLLEXPORT void     RGBtoHSL(COLORREF rgb, float *H, float *S, float *L);
DLLEXPORT void     RGBtoHSV(COLORREF rgb, float *H, float *S, float *V);
DLLEXPORT COLORREF HSLtoRGB(float H, float S, float L);
DLLEXPORT COLORREF HSVtoRGB(float H, float S, float V);

// IMAGE(PEXģʽ)��ͼ����
DLLEXPORT COLORREF    getpixel  (PIMAGE pimg, int x, int y);                   // ��ȡ�����ɫ
DLLEXPORT void        putpixel  (PIMAGE pimg, int x, int y, COLORREF color);   // ����
DLLEXPORT COLORREF    getpixel_f(PIMAGE pimg, int x, int y);                   // ��ȡ�����ɫ
DLLEXPORT void        putpixel_f(PIMAGE pimg, int x, int y, COLORREF color);   // ����
DLLEXPORT void        putpixels  (PIMAGE pimg, int nPoint, int* pPoints);      // ��������
DLLEXPORT void        putpixels_f(PIMAGE pimg, int nPoint, int* pPoints);      // ��������

DLLEXPORT void moveto(PIMAGE pimg, int x, int y);                      // �ƶ���ǰ��(��������)
DLLEXPORT void moverel(PIMAGE pimg, int dx, int dy);                   // �ƶ���ǰ��(�������)

DLLEXPORT void line(PIMAGE pimg, int x1, int y1, int x2, int y2);      // ����
DLLEXPORT void linerel(PIMAGE pimg, int dx, int dy);                   // ����(���������)
DLLEXPORT void lineto(PIMAGE pimg, int x, int y);                      // ����(����������)

DLLEXPORT void rectangle(PIMAGE pimg, int left, int top, int right, int bottom);   // ������

//void getarccoords(int *px, int *py, int *pxstart, int *pystart, int *pxend, int *pyend);    // ��ȡԲ��������Ϣ ###
DLLEXPORT void arc(PIMAGE pimg, int x, int y, int stangle, int endangle, int radius);                   // ��Բ��
DLLEXPORT void circle(PIMAGE pimg, int x, int y, int radius);                                           // ��Բ
DLLEXPORT void pieslice(PIMAGE pimg, int x, int y, int stangle, int endangle, int radius);              // �����Բ����
DLLEXPORT void ellipse(PIMAGE pimg, int x, int y, int stangle, int endangle, int xradius, int yradius); // ����Բ����
DLLEXPORT void fillellipse(PIMAGE pimg, int x, int y, int xradius, int yradius);                        // �������Բ
DLLEXPORT void sector(PIMAGE pimg, int x, int y, int stangle, int endangle, int xradius, int yradius);  // �������Բ����

DLLEXPORT void bar(PIMAGE pimg, int left, int top, int right, int bottom);                              // ���ޱ߿�������
DLLEXPORT void bar3d(PIMAGE pimg, int left, int top, int right, int bottom, int depth, int topflag);    // ���б߿���ά������

DLLEXPORT void drawpoly(PIMAGE pimg, int numpoints, const int *polypoints);                             // �������
DLLEXPORT void fillpoly(PIMAGE pimg, int numpoints, const int *polypoints);                             // �����Ķ����
DLLEXPORT void floodfill(PIMAGE pimg, int x, int y, int border);                                        // �������

DLLEXPORT void line_g(IMAGE* pimg, float x0, float y0, float x1, float y1);
DLLEXPORT void drawpoly_g(IMAGE* pimg, int numpoints, float *polypoints);
DLLEXPORT void fillpoly_g(IMAGE* pimg, int numpoints, float *polypoints);
DLLEXPORT void ellipse_g(IMAGE* pimg, float x1, float y1, float x2, float y2);
DLLEXPORT void fillellipse_g(IMAGE* pimg, float x1, float y1, float x2, float y2);
DLLEXPORT void circle_g(IMAGE* pimg, float x, float y, float r);
DLLEXPORT void arc_g(IMAGE* pimg, float x, float y, float stangle, float endangle, float r);
DLLEXPORT void garc_g(IMAGE* pimg, float x1, float y1, float x2, float y2, float stangle, float sweepAngle);
DLLEXPORT void rectangle_g(IMAGE* pimg, float x1, float y1, float x2, float y2);
DLLEXPORT void fillrectangle_g(IMAGE* pimg, float x1, float y1, float x2, float y2);

// IMAGE(PEXģʽ)������غ���
DLLEXPORT void outtext(PIMAGE pimg, LPCSTR  textstring);                   // �ڵ�ǰλ���������
DLLEXPORT void outtext(PIMAGE pimg, LPCWSTR textstring);                   // �ڵ�ǰλ���������
DLLEXPORT void outtext(PIMAGE pimg, CHAR  c);                              // �ڵ�ǰλ������ַ�
DLLEXPORT void outtext(PIMAGE pimg, WCHAR c);                              // �ڵ�ǰλ������ַ�
DLLEXPORT void outtextxy(PIMAGE pimg, int x, int y, LPCSTR  textstring);   // ��ָ��λ���������
DLLEXPORT void outtextxy(PIMAGE pimg, int x, int y, LPCWSTR textstring);   // ��ָ��λ���������
DLLEXPORT void outtextxy(PIMAGE pimg, int x, int y, CHAR c);               // �ڵ�ǰλ������ַ�
DLLEXPORT void outtextxy(PIMAGE pimg, int x, int y, WCHAR c);              // �ڵ�ǰλ������ַ�
DLLEXPORT void outtextrect(PIMAGE pimg, int x, int y, int w, int h, LPCSTR  textstring); // ��ָ�����η�Χ�������
DLLEXPORT void outtextrect(PIMAGE pimg, int x, int y, int w, int h, LPCWSTR textstring); // ��ָ�����η�Χ�������

DLLEXPORT int  textwidth(PIMAGE pimg, LPCSTR  textstring);                 // ��ȡ�ַ���ռ�õ����ؿ�
DLLEXPORT int  textwidth(PIMAGE pimg, LPCWSTR textstring);                 // ��ȡ�ַ���ռ�õ����ؿ�
DLLEXPORT int  textwidth(PIMAGE pimg, CHAR  c);                            // ��ȡ�ַ�ռ�õ����ؿ�
DLLEXPORT int  textwidth(PIMAGE pimg, WCHAR c);                            // ��ȡ�ַ�ռ�õ����ؿ�
DLLEXPORT int  textheight(PIMAGE pimg, LPCSTR  textstring);                // ��ȡ�ַ���ռ�õ����ظ�
DLLEXPORT int  textheight(PIMAGE pimg, LPCWSTR textstring);                // ��ȡ�ַ���ռ�õ����ظ�
DLLEXPORT int  textheight(PIMAGE pimg, CHAR  c);                           // ��ȡ�ַ�ռ�õ����ظ�
DLLEXPORT int  textheight(PIMAGE pimg, WCHAR c);                           // ��ȡ�ַ�ռ�õ����ظ�
DLLEXPORT void settextjustify(PIMAGE pimg, int horiz, int vert);

// ���õ�ǰ������ʽ(�������)
//      nHeight: �ַ���ƽ���߶ȣ�
//      nWidth: �ַ���ƽ�����(0 ��ʾ����Ӧ)��
//      lpszFace: �������ƣ�
//      nEscapement: �ַ�������д�Ƕ�(��λ 0.1 ��)��
//      nOrientation: ÿ���ַ�����д�Ƕ�(��λ 0.1 ��)��
//      nWeight: �ַ��ıʻ���ϸ(0 ��ʾĬ�ϴ�ϸ)��
//      bItalic: �Ƿ�б�壻
//      bUnderline: �Ƿ��»��ߣ�
//      bStrikeOut: �Ƿ�ɾ���ߣ�
//      fbCharSet: ָ���ַ�����
//      fbOutPrecision: ָ�����ֵ�������ȣ�
//      fbClipPrecision: ָ�����ֵļ������ȣ�
//      fbQuality: ָ�����ֵ����������
//      fbPitchAndFamily: ָ���Գ��淽ʽ�������������ϵ�С�
DLLEXPORT void setfont(PIMAGE pimg, int nHeight, int nWidth, LPCSTR lpszFace);
DLLEXPORT void setfont(PIMAGE pimg, int nHeight, int nWidth, LPCWSTR lpszFace);
DLLEXPORT void setfont(PIMAGE pimg, int nHeight, int nWidth, LPCSTR lpszFace,  int nEscapement, int nOrientation, int nWeight, int bItalic, int bUnderline, int bStrikeOut);
DLLEXPORT void setfont(PIMAGE pimg, int nHeight, int nWidth, LPCWSTR lpszFace, int nEscapement, int nOrientation, int nWeight, int bItalic, int bUnderline, int bStrikeOut);
DLLEXPORT void setfont(PIMAGE pimg, int nHeight, int nWidth, LPCSTR lpszFace,  int nEscapement, int nOrientation, int nWeight, int bItalic, int bUnderline, int bStrikeOut, BYTE fbCharSet, BYTE fbOutPrecision, BYTE fbClipPrecision, BYTE fbQuality, BYTE fbPitchAndFamily);
DLLEXPORT void setfont(PIMAGE pimg, int nHeight, int nWidth, LPCWSTR lpszFace, int nEscapement, int nOrientation, int nWeight, int bItalic, int bUnderline, int bStrikeOut, BYTE fbCharSet, BYTE fbOutPrecision, BYTE fbClipPrecision, BYTE fbQuality, BYTE fbPitchAndFamily);
DLLEXPORT void setfont(PIMAGE pimg, const LOGFONTA *font); // ���õ�ǰ������ʽ
DLLEXPORT void setfont(PIMAGE pimg, const LOGFONTW *font); // ���õ�ǰ������ʽ
DLLEXPORT void getfont(PIMAGE pimg, LOGFONTA *font);       // ��ȡ��ǰ������ʽ
DLLEXPORT void getfont(PIMAGE pimg, LOGFONTW *font);       // ��ȡ��ǰ������ʽ

// ����ͼ�����
#ifndef _GRAPH_LIB_BUILD_
class DLLEXPORT IMAGE
{
private:
    IMAGE();
    ~IMAGE();
public:
    // newimage ���ڴ���PIMAGE��release�����ͷű���
    static PIMAGE newimage(int width, int height);
    static PIMAGE newimage(PIMAGE img);
    int  release();

    //��ͼ������Ϊ����ȷ��ͼԴ��Ŀ�꣬������ʹ�á���ʵ����Ҫ���붨��_USE_IMGTHIS_CALL_�ڰ�����ͷ�ļ�ǰ
#ifdef _USE_IMGTHIS_CALL_
    void getimage(const PIMAGE pSrcImg, int srcX, int srcY, int srcWidth, int srcHeight);
    int  getimage(LPCSTR pImgFile, int zoomWidth = 0, int zoomHeight = 0);
    int  getimage(LPCWSTR pImgFile, int zoomWidth = 0, int zoomHeight = 0);
    int  getimage(LPCSTR pResType, LPCSTR pResName, int zoomWidth = 0, int zoomHeight = 0);
    int  getimage(LPCWSTR pResType, LPCWSTR pResName, int zoomWidth = 0, int zoomHeight = 0);
    int  getimage(void* pMem, long size);
    void putimage(int dstX, int dstY, DWORD dwRop = SRCCOPY) const;
    void putimage(int dstX, int dstY, int dstWidth, int dstHeight, int srcX, int srcY, DWORD dwRop = SRCCOPY) const;
    void putimage(PIMAGE pDstImg, int dstX, int dstY, DWORD dwRop = SRCCOPY) const;
    void putimage(PIMAGE pDstImg, int dstX, int dstY, int dstWidth, int dstHeight, int srcX, int srcY, DWORD dwRop = SRCCOPY) const;
    void putimage(PIMAGE pDstImg, int dstX, int dstY, int dstWidth, int dstHeight, int srcX, int srcY, int srcWidth, int srcHeight, DWORD dwRop = SRCCOPY) const;

    int
    putimage_transparent(
        PIMAGE imgdest,         // handle to dest
        int nXOriginDest,       // x-coord of destination upper-left corner
        int nYOriginDest,       // y-coord of destination upper-left corner
        COLORREF crTransparent, // color to make transparent
        int nXOriginSrc = 0,    // x-coord of source upper-left corner
        int nYOriginSrc = 0,    // y-coord of source upper-left corner
        int nWidthSrc = 0,      // width of source rectangle
        int nHeightSrc = 0      // height of source rectangle
    );
    int
    putimage_alphablend(
        PIMAGE imgdest,         // handle to dest
        int nXOriginDest,       // x-coord of destination upper-left corner
        int nYOriginDest,       // y-coord of destination upper-left corner
        unsigned char alpha,    // alpha
        int nXOriginSrc = 0,    // x-coord of source upper-left corner
        int nYOriginSrc = 0,    // y-coord of source upper-left corner
        int nWidthSrc = 0,      // width of source rectangle
        int nHeightSrc = 0      // height of source rectangle
    );
    int
    putimage_alphatransparent(
        PIMAGE imgdest,         // handle to dest
        int nXOriginDest,       // x-coord of destination upper-left corner
        int nYOriginDest,       // y-coord of destination upper-left corner
        COLORREF crTransparent, // color to make transparent
        unsigned char alpha,    // alpha
        int nXOriginSrc = 0,    // x-coord of source upper-left corner
        int nYOriginSrc = 0,    // y-coord of source upper-left corner
        int nWidthSrc = 0,      // width of source rectangle
        int nHeightSrc = 0      // height of source rectangle
    );
    int
    imagefilter_blurring_4 (
        int intensity,
        int alpha,
        int nXOriginDest,
        int nYOriginDest,
        int nWidthDest,
        int nHeightDest
    );
    int
    imagefilter_blurring_8 (
        int intensity,
        int alpha,
        int nXOriginDest,
        int nYOriginDest,
        int nWidthDest,
        int nHeightDest
    );
    int
    imagefilter_blurring (
        int intensity,
        int alpha,
        int nXOriginDest = 0,
        int nYOriginDest = 0,
        int nWidthDest = 0,
        int nHeightDest = 0
    );

#endif

    // ����ͼ�κ���
    void cleardevice();                 // ����

    void getviewport(int *pleft, int *ptop, int *pright, int *pbottom, int *pclip = 0); // ��ȡ��ͼ��Ϣ
    void setviewport(int left, int top, int right, int bottom, int clip = 1);           // ������ͼ
    void clearviewport();                                                               // �����ͼ

    void getlinestyle(int *plinestyle, WORD *pupattern = NULL, int *pthickness = NULL); // ��ȡ��ǰ����
    void setlinestyle(int linestyle, WORD upattern = NULL, int thickness = 1);          // ���õ�ǰ����
    void setfillstyle(COLORREF color, int pattern = SOLID_FILL, const char *pupattern = NULL);  // �����������

    void setwritemode(int mode);        // ���û�ͼλ����ģʽ

    COLORREF getcolor();                // ��ȡ��ǰ��ͼǰ��ɫ
    void setcolor(COLORREF color);      // ���õ�ǰ��ͼǰ��ɫ
    COLORREF getbkcolor();              // ��ȡ��ǰ��ͼ����ɫ
    void setbkcolor(COLORREF color);    // ���õ�ǰ��ͼ����ɫ
    void setbkcolor_f(COLORREF color);    // ���õ�ǰ��ͼ����ɫ
    void setfontbkcolor(COLORREF color);// ���õ�ǰ���ֱ���ɫ
    void setbkmode(int iBkMode);        // ���ñ������ģʽ

    // ������ͼ����
    COLORREF    getpixel  (int x, int y);                   // ��ȡ�����ɫ
    void        putpixel  (int x, int y, COLORREF color);   // ����
    COLORREF    getpixel_f(int x, int y);                   // ��ȡ�����ɫ
    void        putpixel_f(int x, int y, COLORREF color);   // ����
    void        putpixels  (int nPoint, int* pPoints);      // ��������
    void        putpixels_f(int nPoint, int* pPoints);      // ��������

    void moveto(int x, int y);                      // �ƶ���ǰ��(��������)
    void moverel(int dx, int dy);                   // �ƶ���ǰ��(�������)

    void line(int x1, int y1, int x2, int y2);      // ����
    void linerel(int dx, int dy);                   // ����(���������)
    void lineto(int x, int y);                      // ����(����������)

    void rectangle(int left, int top, int right, int bottom);                       // ������

    void arc(int x, int y, int stangle, int endangle, int radius);                  // ��Բ��
    void circle(int x, int y, int radius);                                          // ��Բ
    void pieslice(int x, int y, int stangle, int endangle, int radius);             // �����Բ����
    void ellipse(int x, int y, int stangle, int endangle, int xradius, int yradius);// ����Բ����
    void fillellipse(int x, int y, int xradius, int yradius);                       // �������Բ
    void sector(int x, int y, int stangle, int endangle, int xradius, int yradius); // �������Բ����

    void bar(int left, int top, int right, int bottom);                             // ���ޱ߿�������
    void bar3d(int left, int top, int right, int bottom, int depth, int topflag);   // ���б߿���ά������

    void drawpoly(int numpoints, const int *polypoints);                            // �������
    void fillpoly(int numpoints, const int *polypoints);                            // �����Ķ����
    void floodfill(int x, int y, int border);                                       // �������

    void line_g(float x0, float y0, float x1, float y1);
    void drawpoly_g(int numpoints, float *polypoints);
    void fillpoly_g(int numpoints, float *polypoints);
    void ellipse_g(float x1, float y1, float x2, float y2);
    void fillellipse_g(float x1, float y1, float x2, float y2);
    void circle_g(float x, float y, float r);
    void arc_g(float x, float y, float stangle, float endangle, float r);
    void garc_g(float x1, float y1, float x2, float y2, float stangle, float sweepAngle);
    void rectangle_g(float x1, float y1, float x2, float y2);
    void fillrectangle_g(float x1, float y1, float x2, float y2);

    void outtext(LPCSTR  textstring);                   // �ڵ�ǰλ���������
    void outtext(LPCWSTR textstring);                   // �ڵ�ǰλ���������
    void outtext(CHAR  c);                              // �ڵ�ǰλ������ַ�
    void outtext(WCHAR c);                              // �ڵ�ǰλ������ַ�
    void outtextxy(int x, int y, LPCSTR  textstring);   // ��ָ��λ���������
    void outtextxy(int x, int y, LPCWSTR textstring);   // ��ָ��λ���������
    void outtextxy(int x, int y, CHAR c);               // ��ָ��λ������ַ�
    void outtextxy(int x, int y, WCHAR c);              // ��ָ��λ������ַ�
    void outtextrect(int x, int y, int w, int h, LPCSTR  textstring); // ��ָ�����η�Χ�������
    void outtextrect(int x, int y, int w, int h, LPCWSTR textstring); // ��ָ�����η�Χ�������

    int  textwidth(LPCSTR  textstring);                 // ��ȡ�ַ���ռ�õ����ؿ�
    int  textwidth(LPCWSTR textstring);                 // ��ȡ�ַ���ռ�õ����ؿ�
    int  textwidth(CHAR  c);                            // ��ȡ�ַ�ռ�õ����ؿ�
    int  textwidth(WCHAR c);                            // ��ȡ�ַ�ռ�õ����ؿ�
    int  textheight(LPCSTR  textstring);                // ��ȡ�ַ���ռ�õ����ظ�
    int  textheight(LPCWSTR textstring);                // ��ȡ�ַ���ռ�õ����ظ�
    int  textheight(CHAR  c);                           // ��ȡ�ַ�ռ�õ����ظ�
    int  textheight(WCHAR c);                           // ��ȡ�ַ�ռ�õ����ظ�
    void settextjustify(int horiz, int vert);

    PDWORD getbuffer();
    int getwidth();         // ��ȡͼƬ���
    int getheight();        // ��ȡͼƬ�߶�
    int getx();             // ��ȡ��ǰ x ����
    int gety();             // ��ȡ��ǰ y ����

    void setfont(int nHeight, int nWidth, LPCSTR lpszFace);
    void setfont(int nHeight, int nWidth, LPCWSTR lpszFace);
    void setfont(int nHeight, int nWidth, LPCSTR lpszFace,  int nEscapement, int nOrientation, int nWeight, int bItalic, int bUnderline, int bStrikeOut);
    void setfont(int nHeight, int nWidth, LPCWSTR lpszFace, int nEscapement, int nOrientation, int nWeight, int bItalic, int bUnderline, int bStrikeOut);
    void setfont(int nHeight, int nWidth, LPCSTR lpszFace,  int nEscapement, int nOrientation, int nWeight, int bItalic, int bUnderline, int bStrikeOut, BYTE fbCharSet, BYTE fbOutPrecision, BYTE fbClipPrecision, BYTE fbQuality, BYTE fbPitchAndFamily);
    void setfont(int nHeight, int nWidth, LPCWSTR lpszFace, int nEscapement, int nOrientation, int nWeight, int bItalic, int bUnderline, int bStrikeOut, BYTE fbCharSet, BYTE fbOutPrecision, BYTE fbClipPrecision, BYTE fbQuality, BYTE fbPitchAndFamily);
    void setfont(const LOGFONTA *font); // ���õ�ǰ������ʽ
    void setfont(const LOGFONTW *font); // ���õ�ǰ������ʽ
    void getfont(LOGFONTA *font);       // ��ȡ��ǰ������ʽ
    void getfont(LOGFONTW *font);       // ��ȡ��ǰ������ʽ

};
#endif

DLLEXPORT void getimage(PIMAGE pDstImg, const PIMAGE pSrcImg, int srcX, int srcY, int srcWidth, int srcHeight);   // ����һ�� IMAGE �����л�ȡͼ��
DLLEXPORT int  getimage(PIMAGE pDstImg, LPCSTR  pImgFile, int zoomWidth = 0, int zoomHeight = 0);     // ��ͼƬ�ļ���ȡͼ��(bmp/jpg/gif/emf/wmf)
DLLEXPORT int  getimage(PIMAGE pDstImg, LPCWSTR pImgFile, int zoomWidth = 0, int zoomHeight = 0);     // ��ͼƬ�ļ���ȡͼ��(bmp/jpg/gif/emf/wmf)
DLLEXPORT int  getimage(PIMAGE pDstImg, LPCSTR  pResType, LPCSTR  pResName, int zoomWidth = 0, int zoomHeight = 0);   // ����Դ�ļ���ȡͼ��(bmp/jpg/gif/emf/wmf)
DLLEXPORT int  getimage(PIMAGE pDstImg, LPCWSTR pResType, LPCWSTR pResName, int zoomWidth = 0, int zoomHeight = 0);   // ����Դ�ļ���ȡͼ��(bmp/jpg/gif/emf/wmf)
DLLEXPORT int  getimage(PIMAGE pDstImg, void* pMem, long size);
DLLEXPORT void putimage(PIMAGE pDstImg, int dstX, int dstY, const PIMAGE pSrcImg, DWORD dwRop = SRCCOPY);         // ����ͼ����һͼ����
DLLEXPORT void putimage(PIMAGE pDstImg, int dstX, int dstY, int dstWidth, int dstHeight, const PIMAGE pSrcImg, int srcX, int srcY, DWORD dwRop = SRCCOPY);    // ����ͼ����һͼ����(ָ�����)
DLLEXPORT void putimage(PIMAGE pDstImg, int dstX, int dstY, int dstWidth, int dstHeight, const PIMAGE pSrcImg, int srcX, int srcY, int srcWidth, int srcHeight, DWORD dwRop = SRCCOPY);   // ����ͼ����һͼ����(ָ��Դ��ߺ�Ŀ���߽�������)
DLLEXPORT int putimage_transparent(
    PIMAGE imgdest,         // handle to dest
    PIMAGE imgsrc,          // handle to source
    int nXOriginDest,       // x-coord of destination upper-left corner
    int nYOriginDest,       // y-coord of destination upper-left corner
    COLORREF crTransparent, // color to make transparent
    int nXOriginSrc = 0,    // x-coord of source upper-left corner
    int nYOriginSrc = 0,    // y-coord of source upper-left corner
    int nWidthSrc = 0,      // width of source rectangle
    int nHeightSrc = 0      // height of source rectangle
);
DLLEXPORT int putimage_alphablend(
    PIMAGE imgdest,         // handle to dest
    PIMAGE imgsrc,          // handle to source
    int nXOriginDest,       // x-coord of destination upper-left corner
    int nYOriginDest,       // y-coord of destination upper-left corner
    unsigned char alpha,    // alpha
    int nXOriginSrc = 0,    // x-coord of source upper-left corner
    int nYOriginSrc = 0,    // y-coord of source upper-left corner
    int nWidthSrc = 0,      // width of source rectangle
    int nHeightSrc = 0      // height of source rectangle
);
DLLEXPORT int putimage_alphatransparent(
    PIMAGE imgdest,         // handle to dest
    PIMAGE imgsrc,          // handle to source
    int nXOriginDest,       // x-coord of destination upper-left corner
    int nYOriginDest,       // y-coord of destination upper-left corner
    COLORREF crTransparent, // color to make transparent
    unsigned char alpha,    // alpha
    int nXOriginSrc = 0,    // x-coord of source upper-left corner
    int nYOriginSrc = 0,    // y-coord of source upper-left corner
    int nWidthSrc = 0,      // width of source rectangle
    int nHeightSrc = 0      // height of source rectangle
);
DLLEXPORT int imagefilter_blurring (
    PIMAGE imgdest,         // handle to dest
    int intensity,
    int alpha,
    int nXOriginDest = 0,
    int nYOriginDest = 0,
    int nWidthDest = 0,
    int nHeightDest = 0
);


// ��������
#define getmaxx getwidth
#define getmaxy getheight
DLLEXPORT int getwidth(PIMAGE pimg = NULL);           // ��ȡͼƬ��ȣ�ΪNULL���ȡ��ʼ��ʱ���ڿ��
DLLEXPORT int getheight(PIMAGE pimg = NULL);          // ��ȡͼƬ�߶ȣ�ΪNULL���ȡ��ʼ��ʱ���ڸ߶�
DLLEXPORT int getx(PIMAGE pimg);               // ��ȡ��ǰ x ����
DLLEXPORT int gety(PIMAGE pimg);               // ��ȡ��ǰ y ����

//ʱ���ӳٺ��������º���������BeginScene��EndScene֮����ã��൱�ڵ�����Render3d����Render3d������BeginScene��EndScene֮�䣩
DLLEXPORT void delay(int ms);
DLLEXPORT void delay_ms(int ms);
DLLEXPORT void delay_fps(int fps);

//����ʱ�亯����API��ʱ���ѯ��
DLLEXPORT void   API_Sleep(DWORD dwMilliseconds);
DLLEXPORT double fclock();

DLLEXPORT HWND      GetHWnd();         // ��ȡ��ͼ���ھ��
DLLEXPORT HINSTANCE GetHInstance();
DLLEXPORT float     GetFPS(int flag = 1); // ��ȡ��ǰ֡��

//���̴�������������BeginScene��EndScene֮�����
DLLEXPORT int getkey();
DLLEXPORT int getchEx(int flag = 0);  //flag�������Ϊ1������������ֵ������ʾ��KEY_DOWN��KEY_UP�¼�ȡ�ã�����ֵ�����KEYMSG_DOWN����KEYMSG_UPλ
DLLEXPORT int kbhitEx(int flag = 0);  //(����)������λ��������룬��flag=0ʱ���ⰴ���ļ���᲻��ͬ
DLLEXPORT int keystate(int key);      //��ü���Ϊkey�ļ��Ƿ��£�ȫ����Ч

//#ifndef _GRAPH_LIB_BUILD_
#if !defined(_INC_CONIO) && !defined(_CONIO_H_)
#define _INC_CONIO
#define _CONIO_H_
#define getch getchEx
#define kbhit kbhitEx
#endif
//#endif

// �����Ϣ��������������BeginScene��EndScene֮�����
// ֧��������Ϣ��
//      WM_MOUSEMOVE        ����ƶ�
//      WM_MOUSEWHEEL       �����ֲ���
//      WM_LBUTTONDOWN      �������
//      WM_LBUTTONUP        �������
//      WM_LBUTTONDBLCLK    ���˫��
//      WM_MBUTTONDOWN      �м�����
//      WM_MBUTTONUP        �м�����
//      WM_MBUTTONDBLCLK    �м�˫��
//      WM_RBUTTONDOWN      �Ҽ�����
//      WM_RBUTTONUP        �Ҽ�����
//      WM_RBUTTONDBLCLK    �Ҽ�˫��

DLLEXPORT int       MouseHit();                 // ����Ƿ���������Ϣ
DLLEXPORT void      FlushMouseMsgBuffer();      // ��������Ϣ������
DLLEXPORT MOUSEMSG  GetMouseMsg(int bLogicPos = 1);              // ��ȡһ�������Ϣ�����û�У��͵ȴ�
DLLEXPORT int       ShowMouse(int bShow);       // �����Ƿ���ʾ���
DLLEXPORT int       GetMousePos(int *x, int *y, int bLogicPos = 1);

//�����������Ϊ����ʹ�ö������ĺ���������Ҫ��initgraphֱ�ӿ��Ե��ã�
DLLEXPORT void          randomize();
DLLEXPORT unsigned int  random(unsigned int n);
DLLEXPORT double        randomf();


typedef void (CALLBACK_PROC)();
typedef CALLBACK_PROC       * LPCALLBACK_PROC;

int SetCloseHandler(LPCALLBACK_PROC func);

} // namespace PEX3D

#ifndef _GRAPH_LIB_BUILD_

#if defined(_MSC_VER) && (defined(HIDE_CONSOLE) || !defined(SHOW_CONSOLE))
#pragma comment( linker, "/subsystem:\"windows\" /entry:\"mainCRTStartup\"" )
#endif

#define Sleep(ms) delay_ms(ms)

#endif

#if !defined(_MSC_VER)
#define WinMain(...) WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow)
#elif defined(_CONSOLE)
#if (_MSC_VER > 1200)
#define WinMain(...) main(int argc, char* argv[])
#else
#define WinMain() main(int argc, char* argv[])
#endif
#endif


#endif //_graph_easyx3d_h_
