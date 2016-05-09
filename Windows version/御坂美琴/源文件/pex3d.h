/*********************************************************
* PowerEasyX3D Library by rtGirl
* Version 2010 1201  V0.1.0 Alpha
* HomePage http://easyx.uueasy.com
*
* FileName: pex3d.h
* 在 VC 下模拟 Borland BGI 绘图库，实现简单的绘图之余，扩展3d图形
*
* 包含并使用本库时，最好不要包含conio.h头文件
* 使用本库，必须用C++编译，可支持的编译器：
* VC6/VC2005/VC2008/VC2010/MinGW3.4.5/MinGW4.4.1
*********************************************************/

/****************************************************************************
** 注意事项：
* ★如果需要显示控制台窗口，请在包含本文件的前面加一行define SHOW_CONSOLE
* ★调用Sleep这个API时，或者调用delay，实际均会转化为调用delay_ms
* ★delay_ms(0)能自行判断有没有更新的必要，连续多次但不大量的调用并不会产生帧率的影响
* ★调用delay_ms, delay_fps, getch, GetMouseMsg 时，窗口内容可能会更新，这些函数相当于内置了Render3d()，
*   如果你只需要更新窗口，而不想等待，最好直接调用delay_ms(0)或者Render3d();
* ★以下所有的3D图形函数，以数学线性代数用的右手坐标系，x向右，y向上，z向屏幕外（拇指顺z方向，其余四指表从x->y的旋转方向）
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
    EX3D_ANTIALIASING0      = 0, //不开抗锯齿
    EX3D_ANTIALIASING1      = 1, //2X抗锯齿
    EX3D_ANTIALIASING2      = 2, //4X抗锯齿
    EX3D_ANTIALIASING3      = 3, //8X抗锯齿
    EX3D_ANTIALIASINGMASK   = 3,
    EX3D_VSYNC              = 0x80000000, //开启垂直同步
}EX3D_SETTINGS;

typedef enum EXTEX_settings {
    EXTEX_ANISOTROPY0       = 0, //不开各向异性过滤
    EXTEX_ANISOTROPY1       = 1, //2X各向异性过滤（表最大值，实际值由硬件所支持决定）
    EXTEX_ANISOTROPY2       = 2, //3X各向异性过滤
    EXTEX_ANISOTROPY3       = 3, //4X各向异性过滤
    EXTEX_ANISOTROPY4       = 4, //5X各向异性过滤
    EXTEX_ANISOTROPY5       = 5, //6X各向异性过滤
    EXTEX_ANISOTROPY6       = 6, //7X各向异性过滤
    EXTEX_ANISOTROPY7       = 7, //8X各向异性过滤
    EXTEX_ANISOTROPY8       = 8, //9X各向异性过滤
    EXTEX_ANISOTROPY9       = 9, //10X各向异性过滤
    EXTEX_ANISOTROPYa       = 10, //11X各向异性过滤
    EXTEX_ANISOTROPYb       = 11, //12X各向异性过滤
    EXTEX_ANISOTROPYc       = 12, //13X各向异性过滤
    EXTEX_ANISOTROPYd       = 13, //14X各向异性过滤
    EXTEX_ANISOTROPYe       = 14, //15X各向异性过滤
    EXTEX_ANISOTROPYf       = 15, //16X各向异性过滤
    EXTEX_ANTIALIASINGMASK  = 0xF,
    EXTEX_MIPMAP            = 0x10, //开启mipmap
    EXTEX_LINEAR            = 0x20, //开启线性过滤，如同时开启EXTEX_MIPMAP_LINEAR即为三线性过滤
    EXTEX_MIPMAP_LINEAR     = 0x50, //开启纹理线性过滤，自动和EXTEX_MIPMAP同时使用
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

    //灯设置
    EXL_AMBIENT         = 0x1200, //环境光，color * 4 (R, G, B, A) 下同，默认(0,0,0,1)，A位必须为1，其它的相应亮度组合
    EXL_DIFFUSE         = 0x1201, //漫反射，color * 4 默认1,1,1,1
    EXL_SPECULAR        = 0x1202, //高光，color * 4 默认1,1,1,1

    EXL_POSITION        = 0x1203, //光源位置，pos * 4 (x, y, z, w) 默认0,0,1,0，若w为0表示用无衰减定向光，否则w为1

    EXL_SPOT_DIRECTION  = 0x1204, //光方向，vector * 3 (dx, dy, dz) (当EXL_SPOT_CUTOFF=180时使这无效) 默认0,0,1
    EXL_SPOT_EXPONENT   = 0x1205, //聚光指数 value * 1 (0-128)  默认0
    EXL_SPOT_CUTOFF     = 0x1206, //光照范围角度 angle * 1 (0-90, 180) 默认180，即点光源
    EXL_CONSTANT_ATTENUATION = 0x1207, //线性衰减常数 value * 1 (0-128)  默认1
    EXL_LINEAR_ATTENUATION   = 0x1208, //二次衰减常数 value * 1 (0-128)  默认1

    EXL_FINITY_FAR          = 0x0B51, //有限距离光源，bool * 1，false表示无限远（平行光，默认）
    EXL_MODEL_TWO_SIDE      = 0x0B52, //双面光源，允许背面光照，bool * 1，false表示非双面光（默认）
    EXL_LIGHT_AMBIENT       = 0x0B53, //环境光亮度占的比例，color*4，默认0xFF333333
    EXL_LIGHT_COLOR_CONTROL = 0x81F8,
}LIGHT_SETTING;

typedef enum material_setting {
    EXM_FRONT           = 0x0404,
    EXM_BACK            = 0x0405,
    EXM_FRONT_AND_BACK  = 0x0408,

    EXM_AMBIENT     = 0x1200, // 环境光，默认0xFF333333
    EXM_DIFFUSE     = 0x1201, // 反射光，默认0xFFCCCCCC
    EXM_SPECULAR    = 0x1202, // 高光，默认0xFF000000
    EXM_EMISSION    = 0x1600, // 自发光，默认0xFF000000
    EXM_SHININESS   = 0x1601, // 高光指数 0-128，默认0
}MATERIAL_SETTING;

typedef enum clear_flag {
    EXC_CLEAR_TARGET    = 0x4000,
    EXC_CLEAR_ZBUFFER   = 0x100,
    //EXC_CLEAR_STENCIL   = 4, //未用
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

// 颜色
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

// 填充模式
enum fill_patterns {
    EMPTY_FILL,
    NULL_FILL,          // 不填充
    SOLID_FILL,         // 用指定颜色完全填充
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

// 3d 计算辅助函数
DLLEXPORT void rotate_point3d_x(VECTOR3D  * pt, float r); //弧度，右手定则
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
    VECTOR3D& operator *= (float f); //缩放
    VECTOR3D operator * (float f) const; //缩放
    float operator * (const VECTOR3D& _fp) const; //点乘
    VECTOR3D operator & (const VECTOR3D& _fp) const; //叉乘
    VECTOR3D& operator &= (const VECTOR3D& _fp); //叉乘
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
    VECTOR3D& Rotate(float rad, const VECTOR3D& v); //绕任意轴旋转，右手定则，rad为弧度
    VECTOR3D& Rotate(float rad, float x, float y, float z) {
        VECTOR3D v(x, y, z);
        return Rotate(rad, v);
    }
    VECTOR3D& Rotate(const VECTOR3D& e, const VECTOR3D& s = VECTOR3D(0.0f, 0.0f, 1.0f)); //从s到e之间的夹角确定旋转
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
    UINT uMsg;              // 当前鼠标消息
    bool mkCtrl;            // Ctrl 键是否按下
    bool mkShift;           // Shift 键是否按下
    bool mkLButton;         // 鼠标左键是否按下
    bool mkMButton;         // 鼠标中键是否按下
    bool mkRButton;         // 鼠标右键是否按下
    short x;                // 当前鼠标 x 坐标
    short y;                // 当前鼠标 y 坐标
    short wheel;            // 鼠标滚轮滚动值(120的倍数)
}MOUSEMSG;




//--------------------------------------------------------------------------------------------------
// 绘图基本函数
DLLEXPORT void setinitmode(int mode, int x = CW_USEDEFAULT, int y = CW_USEDEFAULT); // 这个函数只能在initgraph调用前调用
DLLEXPORT void initgraph(int Width, int Height, int Flag = EX3D_ANTIALIASING0);
DLLEXPORT void closegraph();

DLLEXPORT int  cleardevice(CLEAR_FLAG flag = EXC_CLEAR_ALL); //清屏（或者所选入的纹理）。如果flag&1那就清屏幕，如果flag&2那就清zbuffer
DLLEXPORT int  SetScreenLogicalSize(int width, int height); // 重新调整窗口逻辑大小（不改变物理大小），不能在BeginXd/EndRender之间调用，否则后果不定
DLLEXPORT void SetClearColor(COLORREF color = 0xFF000000);

//绘图状态控制函数

DLLEXPORT int  Render3d(); //此函数一般不要直接调用

DLLEXPORT int  Begin2d(int dx = 0, int dy = 0); // 参数为定义左上角坐标
DLLEXPORT int  Begin3d(float viewdeg = 90.0f, float zNear = 0.1f, float zFar = 100.0f); //参数为摄像机可视角度(y方向)，近裁面，远裁面，均必须为正数
DLLEXPORT void EndRender();

DLLEXPORT int  SetViewPort(int x, int y, int w, int h); //在BeginXd/EndRender之间设置，其它地方无效

DLLEXPORT void Enable3D(int state, BOOL bSet); //见Enable3D_State

//绘图函数，以下函数只能在BeginXd/EndRender之间调用，否则绘图无效或者结果不确定

// 2d
DLLEXPORT int Line2D(COLORREF color1, float x1, float y1, COLORREF color2, float x2, float y2);
    //以下三函数需要配合SelectTexture
    //srcW,srcH如果为0.0f，则使用本身纹理的大小，若这时srcX或srcY不为默认值则会越界，越界会按比例裁剪
DLLEXPORT int RenderTextureXYXYWH(float destX, float destY, float falpha = 1.0f, float srcX = 0.0f, float srcY = 0.0f, float srcW = 0.0f, float srcH = 0.0f);
DLLEXPORT int RenderTextureXYWHXYWH(float destX, float destY, float destW, float destH,
                                    float falpha = 1.0f,
                                    float srcX = 0.0f, float srcY = 0.0f, float srcW = 0.0f, float srcH = 0.0f);
DLLEXPORT int RenderTextureXYRCXYWH(float destX, float destY, float rad,
                                    float srcCenterx, float srcCentery,
                                    float falpha = 1.0f, float scalex = 1.0f, float scaley = 1.0f,
                                    float srcX = 0.0f, float srcY = 0.0f, float srcW = 0.0f, float srcH = 0.0f);
DLLEXPORT int RenderImage(PIMAGE pimg, int flag = 0); //把IMAGE拉伸贴图到整个窗口里，不能在BeginXd/EndRender之间调用，flag参见SelectTexture
DLLEXPORT int RenderImage(PIMAGE pimg, int x, int y, int w = 0, int h = 0, int flag = 0); //把IMAGE指定区域拉伸贴图到整个窗口里，不能在BeginXd/EndRender之间调用

//3d
DLLEXPORT int Line3D(COLORREF color1, float x1, float y1, float z1, COLORREF color2, float x2, float y2, float z2);

//2d3d均可，2d时请把z设置为1.0f
DLLEXPORT int Point(COLORREF color, float x, float y, float z = 1.0f);
DLLEXPORT int Point(int nPoint, FPOINT3D *pt, int style = EXP_POINTS);
    //以下五函数需要配合SelectTexture
DLLEXPORT int RenderTextureTriangle(FTPOINT3D ftA, FTPOINT3D ftB, FTPOINT3D ftC);
DLLEXPORT int RenderTextureQuad(FTPOINT3D ftLT, FTPOINT3D ftLB, FTPOINT3D ftRB, FTPOINT3D ftRT);
DLLEXPORT int RenderTextureStrip(int nPoint, FTPOINT3D *pt, BOOL bLoop = FALSE);

DLLEXPORT int ScreenToTexture(int destX, int destY, int srcX, int srcY, int srcW, int srcH);
DLLEXPORT int TextureToImage(PIMAGE pimg, int destX, int destY, int srcX, int srcY, int srcW, int srcH);


//图形资源函数
DLLEXPORT HTEXTURE CreateTexture(PIMAGE pimg, COLORREF colorkey = 0x0); //colorkey = 0xFFFFFF using original alpha (fast). using 0xFF000000 set as black
DLLEXPORT int ReleaseTexture(HTEXTURE hTexture);
DLLEXPORT int SelectTexture(HTEXTURE hTexture, int flag = EXTEX_ANISOTROPY0); //flag使用EXTEX_settings中的表，对于同一纹理，多次select使用相同的flag，那么将得到加速
DLLEXPORT int GetTextureWidth (HTEXTURE hTexture);
DLLEXPORT int GetTextureHeight(HTEXTURE hTexture);


//摄像机变换函数
DLLEXPORT void CameraReset(float viewdeg = 90.0f, float zNear = 0.1f, float zFar = 100.0f);
DLLEXPORT void CameraScale(float x, float y, float z);
DLLEXPORT void CameraMove(float dx, float dy, float dz);
DLLEXPORT void CameraRotate (float rad, float x, float y, float z); //(x,y,z) 为旋转轴，右手定则，rad为弧度
DLLEXPORT void CameraRotateD(float angle, float x, float y, float z); //(x,y,z) 为旋转轴，右手定则，angle为角度
DLLEXPORT void CameraSet(VECTOR3D* pos, VECTOR3D* center, VECTOR3D* up);
DLLEXPORT void GetPickRay(const VECTOR3D* mouse, VECTOR3D* start, VECTOR3D* end);
DLLEXPORT void ObjectReset();
DLLEXPORT void ObjectScale(float x, float y, float z);
DLLEXPORT void ObjectMove(float dx, float dy, float dz);
DLLEXPORT void ObjectRotate (float rad, float x, float y, float z); //(x,y,z) 为旋转轴，右手定则，rad为弧度
DLLEXPORT void ObjectRotateD(float angle, float x, float y, float z); //(x,y,z) 为旋转轴，右手定则，angle为角度
DLLEXPORT void PushMatrix();
DLLEXPORT void PopMatrix();

//光照函数
//DLLEXPORT void LightEnable(BOOL set); //非必要不要手工调用，只要用LightTurnOn关掉即可
DLLEXPORT int  LightTurnOn(LIGHT_SETTING id, BOOL set);
DLLEXPORT int  LightTypeSet(LIGHT_SETTING id, LIGHT_SETTING type, float* param); //不支持EXL_FINITY_FAR和EXL_MODEL_TWO_SIDE
DLLEXPORT int  LightTypeSeti(LIGHT_SETTING id, LIGHT_SETTING type, COLORREF param); //只能使用EXL_AMBIENT,EXL_DIFFUSE,EXL_SPECULAR，相应字节为0xFF等于1.0f，格式ABGR
DLLEXPORT int  LightTypeSetf(LIGHT_SETTING id, LIGHT_SETTING type, float param); //只能使用EXL_SPOT_EXPONENT,EXL_SPOT_CUTOFF,EXL_CONSTANT_ATTENUATION,EXL_LINEAR_ATTENUATION
DLLEXPORT int  LightSet(LIGHT_SETTING type, unsigned int param); //对所有光源有效，使用EXL_FINITY_FAR和EXL_MODEL_TWO_SIDE，参数只取0/1，使用EXL_LIGHT_AMBIENT，参数表颜色
DLLEXPORT int  MaterialSet(MATERIAL_SETTING face, MATERIAL_SETTING type, COLORREF param); //设置材质
DLLEXPORT int  MaterialSpecular(BOOL set); //材质高反光效果









//-----------------------------------------------------------------------------------------------
// IMAGE(PEX模式)绘图环境设置

DLLEXPORT void cleardevice(PIMAGE pimg);                   // 清屏

DLLEXPORT void getviewport(PIMAGE pimg, int *pleft, int *ptop, int *pright, int *pbottom, int *pclip = 0); // 获取视图信息
DLLEXPORT void setviewport(PIMAGE pimg, int left, int top, int right, int bottom, int clip = 1);           // 设置视图
DLLEXPORT void clearviewport(PIMAGE pimg);                                                                     // 清空视图

DLLEXPORT void getlinestyle(PIMAGE pimg, int *plinestyle, WORD *pupattern = NULL, int *pthickness = NULL); // 获取当前线形
DLLEXPORT void setlinestyle(PIMAGE pimg, int linestyle, WORD upattern = NULL, int thickness = 1);          // 设置当前线形
//void getfillstyle(PIMAGE pimg, COLORREF *pcolor, int *ppattern = NULL, char *pupattern = NULL);          // 获取填充类型 ###
DLLEXPORT void setfillstyle(PIMAGE pimg, COLORREF color, int pattern = SOLID_FILL, const char *pupattern = NULL);  // 设置填充类型

DLLEXPORT void setwritemode(PIMAGE pimg, int mode);        // 设置绘图位操作模式

DLLEXPORT COLORREF getcolor(PIMAGE pimg);                  // 获取当前绘图前景色
DLLEXPORT void setcolor(PIMAGE pimg, COLORREF color);      // 设置当前绘图前景色
DLLEXPORT COLORREF getbkcolor(PIMAGE pimg);                // 获取当前绘图背景色
DLLEXPORT void setbkcolor(PIMAGE pimg, COLORREF color);    // 设置当前绘图背景色
DLLEXPORT void setbkcolor_f(PIMAGE pimg, COLORREF color);    // 设置当前绘图背景色
DLLEXPORT void setfontbkcolor(PIMAGE pimg, COLORREF color);// 设置当前文字背景色

DLLEXPORT void setbkmode(PIMAGE pimg, int iBkMode);        // 设置背景混合模式

// 颜色模型转换函数
DLLEXPORT COLORREF RGBtoGRAY(COLORREF rgb);
DLLEXPORT void     RGBtoHSL(COLORREF rgb, float *H, float *S, float *L);
DLLEXPORT void     RGBtoHSV(COLORREF rgb, float *H, float *S, float *V);
DLLEXPORT COLORREF HSLtoRGB(float H, float S, float L);
DLLEXPORT COLORREF HSVtoRGB(float H, float S, float V);

// IMAGE(PEX模式)绘图函数
DLLEXPORT COLORREF    getpixel  (PIMAGE pimg, int x, int y);                   // 获取点的颜色
DLLEXPORT void        putpixel  (PIMAGE pimg, int x, int y, COLORREF color);   // 画点
DLLEXPORT COLORREF    getpixel_f(PIMAGE pimg, int x, int y);                   // 获取点的颜色
DLLEXPORT void        putpixel_f(PIMAGE pimg, int x, int y, COLORREF color);   // 画点
DLLEXPORT void        putpixels  (PIMAGE pimg, int nPoint, int* pPoints);      // 批量画点
DLLEXPORT void        putpixels_f(PIMAGE pimg, int nPoint, int* pPoints);      // 批量画点

DLLEXPORT void moveto(PIMAGE pimg, int x, int y);                      // 移动当前点(绝对坐标)
DLLEXPORT void moverel(PIMAGE pimg, int dx, int dy);                   // 移动当前点(相对坐标)

DLLEXPORT void line(PIMAGE pimg, int x1, int y1, int x2, int y2);      // 画线
DLLEXPORT void linerel(PIMAGE pimg, int dx, int dy);                   // 画线(至相对坐标)
DLLEXPORT void lineto(PIMAGE pimg, int x, int y);                      // 画线(至绝对坐标)

DLLEXPORT void rectangle(PIMAGE pimg, int left, int top, int right, int bottom);   // 画矩形

//void getarccoords(int *px, int *py, int *pxstart, int *pystart, int *pxend, int *pyend);    // 获取圆弧坐标信息 ###
DLLEXPORT void arc(PIMAGE pimg, int x, int y, int stangle, int endangle, int radius);                   // 画圆弧
DLLEXPORT void circle(PIMAGE pimg, int x, int y, int radius);                                           // 画圆
DLLEXPORT void pieslice(PIMAGE pimg, int x, int y, int stangle, int endangle, int radius);              // 画填充圆扇形
DLLEXPORT void ellipse(PIMAGE pimg, int x, int y, int stangle, int endangle, int xradius, int yradius); // 画椭圆弧线
DLLEXPORT void fillellipse(PIMAGE pimg, int x, int y, int xradius, int yradius);                        // 画填充椭圆
DLLEXPORT void sector(PIMAGE pimg, int x, int y, int stangle, int endangle, int xradius, int yradius);  // 画填充椭圆扇形

DLLEXPORT void bar(PIMAGE pimg, int left, int top, int right, int bottom);                              // 画无边框填充矩形
DLLEXPORT void bar3d(PIMAGE pimg, int left, int top, int right, int bottom, int depth, int topflag);    // 画有边框三维填充矩形

DLLEXPORT void drawpoly(PIMAGE pimg, int numpoints, const int *polypoints);                             // 画多边形
DLLEXPORT void fillpoly(PIMAGE pimg, int numpoints, const int *polypoints);                             // 画填充的多边形
DLLEXPORT void floodfill(PIMAGE pimg, int x, int y, int border);                                        // 填充区域

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

// IMAGE(PEX模式)文字相关函数
DLLEXPORT void outtext(PIMAGE pimg, LPCSTR  textstring);                   // 在当前位置输出文字
DLLEXPORT void outtext(PIMAGE pimg, LPCWSTR textstring);                   // 在当前位置输出文字
DLLEXPORT void outtext(PIMAGE pimg, CHAR  c);                              // 在当前位置输出字符
DLLEXPORT void outtext(PIMAGE pimg, WCHAR c);                              // 在当前位置输出字符
DLLEXPORT void outtextxy(PIMAGE pimg, int x, int y, LPCSTR  textstring);   // 在指定位置输出文字
DLLEXPORT void outtextxy(PIMAGE pimg, int x, int y, LPCWSTR textstring);   // 在指定位置输出文字
DLLEXPORT void outtextxy(PIMAGE pimg, int x, int y, CHAR c);               // 在当前位置输出字符
DLLEXPORT void outtextxy(PIMAGE pimg, int x, int y, WCHAR c);              // 在当前位置输出字符
DLLEXPORT void outtextrect(PIMAGE pimg, int x, int y, int w, int h, LPCSTR  textstring); // 在指定矩形范围输出文字
DLLEXPORT void outtextrect(PIMAGE pimg, int x, int y, int w, int h, LPCWSTR textstring); // 在指定矩形范围输出文字

DLLEXPORT int  textwidth(PIMAGE pimg, LPCSTR  textstring);                 // 获取字符串占用的像素宽
DLLEXPORT int  textwidth(PIMAGE pimg, LPCWSTR textstring);                 // 获取字符串占用的像素宽
DLLEXPORT int  textwidth(PIMAGE pimg, CHAR  c);                            // 获取字符占用的像素宽
DLLEXPORT int  textwidth(PIMAGE pimg, WCHAR c);                            // 获取字符占用的像素宽
DLLEXPORT int  textheight(PIMAGE pimg, LPCSTR  textstring);                // 获取字符串占用的像素高
DLLEXPORT int  textheight(PIMAGE pimg, LPCWSTR textstring);                // 获取字符串占用的像素高
DLLEXPORT int  textheight(PIMAGE pimg, CHAR  c);                           // 获取字符占用的像素高
DLLEXPORT int  textheight(PIMAGE pimg, WCHAR c);                           // 获取字符占用的像素高
DLLEXPORT void settextjustify(PIMAGE pimg, int horiz, int vert);

// 设置当前字体样式(详见帮助)
//      nHeight: 字符的平均高度；
//      nWidth: 字符的平均宽度(0 表示自适应)；
//      lpszFace: 字体名称；
//      nEscapement: 字符串的书写角度(单位 0.1 度)；
//      nOrientation: 每个字符的书写角度(单位 0.1 度)；
//      nWeight: 字符的笔画粗细(0 表示默认粗细)；
//      bItalic: 是否斜体；
//      bUnderline: 是否下划线；
//      bStrikeOut: 是否删除线；
//      fbCharSet: 指定字符集；
//      fbOutPrecision: 指定文字的输出精度；
//      fbClipPrecision: 指定文字的剪辑精度；
//      fbQuality: 指定文字的输出质量；
//      fbPitchAndFamily: 指定以常规方式描述字体的字体系列。
DLLEXPORT void setfont(PIMAGE pimg, int nHeight, int nWidth, LPCSTR lpszFace);
DLLEXPORT void setfont(PIMAGE pimg, int nHeight, int nWidth, LPCWSTR lpszFace);
DLLEXPORT void setfont(PIMAGE pimg, int nHeight, int nWidth, LPCSTR lpszFace,  int nEscapement, int nOrientation, int nWeight, int bItalic, int bUnderline, int bStrikeOut);
DLLEXPORT void setfont(PIMAGE pimg, int nHeight, int nWidth, LPCWSTR lpszFace, int nEscapement, int nOrientation, int nWeight, int bItalic, int bUnderline, int bStrikeOut);
DLLEXPORT void setfont(PIMAGE pimg, int nHeight, int nWidth, LPCSTR lpszFace,  int nEscapement, int nOrientation, int nWeight, int bItalic, int bUnderline, int bStrikeOut, BYTE fbCharSet, BYTE fbOutPrecision, BYTE fbClipPrecision, BYTE fbQuality, BYTE fbPitchAndFamily);
DLLEXPORT void setfont(PIMAGE pimg, int nHeight, int nWidth, LPCWSTR lpszFace, int nEscapement, int nOrientation, int nWeight, int bItalic, int bUnderline, int bStrikeOut, BYTE fbCharSet, BYTE fbOutPrecision, BYTE fbClipPrecision, BYTE fbQuality, BYTE fbPitchAndFamily);
DLLEXPORT void setfont(PIMAGE pimg, const LOGFONTA *font); // 设置当前字体样式
DLLEXPORT void setfont(PIMAGE pimg, const LOGFONTW *font); // 设置当前字体样式
DLLEXPORT void getfont(PIMAGE pimg, LOGFONTA *font);       // 获取当前字体样式
DLLEXPORT void getfont(PIMAGE pimg, LOGFONTW *font);       // 获取当前字体样式

// 定义图像对象
#ifndef _GRAPH_LIB_BUILD_
class DLLEXPORT IMAGE
{
private:
    IMAGE();
    ~IMAGE();
public:
    // newimage 用于创建PIMAGE，release用于释放本身
    static PIMAGE newimage(int width, int height);
    static PIMAGE newimage(PIMAGE img);
    int  release();

    //贴图处理，但为了明确贴图源和目标，不建议使用。如实在需要，请定义_USE_IMGTHIS_CALL_在包含本头文件前
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

    // 基本图形函数
    void cleardevice();                 // 清屏

    void getviewport(int *pleft, int *ptop, int *pright, int *pbottom, int *pclip = 0); // 获取视图信息
    void setviewport(int left, int top, int right, int bottom, int clip = 1);           // 设置视图
    void clearviewport();                                                               // 清空视图

    void getlinestyle(int *plinestyle, WORD *pupattern = NULL, int *pthickness = NULL); // 获取当前线形
    void setlinestyle(int linestyle, WORD upattern = NULL, int thickness = 1);          // 设置当前线形
    void setfillstyle(COLORREF color, int pattern = SOLID_FILL, const char *pupattern = NULL);  // 设置填充类型

    void setwritemode(int mode);        // 设置绘图位操作模式

    COLORREF getcolor();                // 获取当前绘图前景色
    void setcolor(COLORREF color);      // 设置当前绘图前景色
    COLORREF getbkcolor();              // 获取当前绘图背景色
    void setbkcolor(COLORREF color);    // 设置当前绘图背景色
    void setbkcolor_f(COLORREF color);    // 设置当前绘图背景色
    void setfontbkcolor(COLORREF color);// 设置当前文字背景色
    void setbkmode(int iBkMode);        // 设置背景混合模式

    // 基本绘图函数
    COLORREF    getpixel  (int x, int y);                   // 获取点的颜色
    void        putpixel  (int x, int y, COLORREF color);   // 画点
    COLORREF    getpixel_f(int x, int y);                   // 获取点的颜色
    void        putpixel_f(int x, int y, COLORREF color);   // 画点
    void        putpixels  (int nPoint, int* pPoints);      // 批量画点
    void        putpixels_f(int nPoint, int* pPoints);      // 批量画点

    void moveto(int x, int y);                      // 移动当前点(绝对坐标)
    void moverel(int dx, int dy);                   // 移动当前点(相对坐标)

    void line(int x1, int y1, int x2, int y2);      // 画线
    void linerel(int dx, int dy);                   // 画线(至相对坐标)
    void lineto(int x, int y);                      // 画线(至绝对坐标)

    void rectangle(int left, int top, int right, int bottom);                       // 画矩形

    void arc(int x, int y, int stangle, int endangle, int radius);                  // 画圆弧
    void circle(int x, int y, int radius);                                          // 画圆
    void pieslice(int x, int y, int stangle, int endangle, int radius);             // 画填充圆扇形
    void ellipse(int x, int y, int stangle, int endangle, int xradius, int yradius);// 画椭圆弧线
    void fillellipse(int x, int y, int xradius, int yradius);                       // 画填充椭圆
    void sector(int x, int y, int stangle, int endangle, int xradius, int yradius); // 画填充椭圆扇形

    void bar(int left, int top, int right, int bottom);                             // 画无边框填充矩形
    void bar3d(int left, int top, int right, int bottom, int depth, int topflag);   // 画有边框三维填充矩形

    void drawpoly(int numpoints, const int *polypoints);                            // 画多边形
    void fillpoly(int numpoints, const int *polypoints);                            // 画填充的多边形
    void floodfill(int x, int y, int border);                                       // 填充区域

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

    void outtext(LPCSTR  textstring);                   // 在当前位置输出文字
    void outtext(LPCWSTR textstring);                   // 在当前位置输出文字
    void outtext(CHAR  c);                              // 在当前位置输出字符
    void outtext(WCHAR c);                              // 在当前位置输出字符
    void outtextxy(int x, int y, LPCSTR  textstring);   // 在指定位置输出文字
    void outtextxy(int x, int y, LPCWSTR textstring);   // 在指定位置输出文字
    void outtextxy(int x, int y, CHAR c);               // 在指定位置输出字符
    void outtextxy(int x, int y, WCHAR c);              // 在指定位置输出字符
    void outtextrect(int x, int y, int w, int h, LPCSTR  textstring); // 在指定矩形范围输出文字
    void outtextrect(int x, int y, int w, int h, LPCWSTR textstring); // 在指定矩形范围输出文字

    int  textwidth(LPCSTR  textstring);                 // 获取字符串占用的像素宽
    int  textwidth(LPCWSTR textstring);                 // 获取字符串占用的像素宽
    int  textwidth(CHAR  c);                            // 获取字符占用的像素宽
    int  textwidth(WCHAR c);                            // 获取字符占用的像素宽
    int  textheight(LPCSTR  textstring);                // 获取字符串占用的像素高
    int  textheight(LPCWSTR textstring);                // 获取字符串占用的像素高
    int  textheight(CHAR  c);                           // 获取字符占用的像素高
    int  textheight(WCHAR c);                           // 获取字符占用的像素高
    void settextjustify(int horiz, int vert);

    PDWORD getbuffer();
    int getwidth();         // 获取图片宽度
    int getheight();        // 获取图片高度
    int getx();             // 获取当前 x 坐标
    int gety();             // 获取当前 y 坐标

    void setfont(int nHeight, int nWidth, LPCSTR lpszFace);
    void setfont(int nHeight, int nWidth, LPCWSTR lpszFace);
    void setfont(int nHeight, int nWidth, LPCSTR lpszFace,  int nEscapement, int nOrientation, int nWeight, int bItalic, int bUnderline, int bStrikeOut);
    void setfont(int nHeight, int nWidth, LPCWSTR lpszFace, int nEscapement, int nOrientation, int nWeight, int bItalic, int bUnderline, int bStrikeOut);
    void setfont(int nHeight, int nWidth, LPCSTR lpszFace,  int nEscapement, int nOrientation, int nWeight, int bItalic, int bUnderline, int bStrikeOut, BYTE fbCharSet, BYTE fbOutPrecision, BYTE fbClipPrecision, BYTE fbQuality, BYTE fbPitchAndFamily);
    void setfont(int nHeight, int nWidth, LPCWSTR lpszFace, int nEscapement, int nOrientation, int nWeight, int bItalic, int bUnderline, int bStrikeOut, BYTE fbCharSet, BYTE fbOutPrecision, BYTE fbClipPrecision, BYTE fbQuality, BYTE fbPitchAndFamily);
    void setfont(const LOGFONTA *font); // 设置当前字体样式
    void setfont(const LOGFONTW *font); // 设置当前字体样式
    void getfont(LOGFONTA *font);       // 获取当前字体样式
    void getfont(LOGFONTW *font);       // 获取当前字体样式

};
#endif

DLLEXPORT void getimage(PIMAGE pDstImg, const PIMAGE pSrcImg, int srcX, int srcY, int srcWidth, int srcHeight);   // 从另一个 IMAGE 对象中获取图像
DLLEXPORT int  getimage(PIMAGE pDstImg, LPCSTR  pImgFile, int zoomWidth = 0, int zoomHeight = 0);     // 从图片文件获取图像(bmp/jpg/gif/emf/wmf)
DLLEXPORT int  getimage(PIMAGE pDstImg, LPCWSTR pImgFile, int zoomWidth = 0, int zoomHeight = 0);     // 从图片文件获取图像(bmp/jpg/gif/emf/wmf)
DLLEXPORT int  getimage(PIMAGE pDstImg, LPCSTR  pResType, LPCSTR  pResName, int zoomWidth = 0, int zoomHeight = 0);   // 从资源文件获取图像(bmp/jpg/gif/emf/wmf)
DLLEXPORT int  getimage(PIMAGE pDstImg, LPCWSTR pResType, LPCWSTR pResName, int zoomWidth = 0, int zoomHeight = 0);   // 从资源文件获取图像(bmp/jpg/gif/emf/wmf)
DLLEXPORT int  getimage(PIMAGE pDstImg, void* pMem, long size);
DLLEXPORT void putimage(PIMAGE pDstImg, int dstX, int dstY, const PIMAGE pSrcImg, DWORD dwRop = SRCCOPY);         // 绘制图像到另一图像中
DLLEXPORT void putimage(PIMAGE pDstImg, int dstX, int dstY, int dstWidth, int dstHeight, const PIMAGE pSrcImg, int srcX, int srcY, DWORD dwRop = SRCCOPY);    // 绘制图像到另一图像中(指定宽高)
DLLEXPORT void putimage(PIMAGE pDstImg, int dstX, int dstY, int dstWidth, int dstHeight, const PIMAGE pSrcImg, int srcX, int srcY, int srcWidth, int srcHeight, DWORD dwRop = SRCCOPY);   // 绘制图像到另一图像中(指定源宽高和目标宽高进行拉伸)
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


// 其它函数
#define getmaxx getwidth
#define getmaxy getheight
DLLEXPORT int getwidth(PIMAGE pimg = NULL);           // 获取图片宽度，为NULL则获取初始化时窗口宽度
DLLEXPORT int getheight(PIMAGE pimg = NULL);          // 获取图片高度，为NULL则获取初始化时窗口高度
DLLEXPORT int getx(PIMAGE pimg);               // 获取当前 x 坐标
DLLEXPORT int gety(PIMAGE pimg);               // 获取当前 y 坐标

//时间延迟函数，以下函数不能在BeginScene与EndScene之间调用（相当于调用了Render3d，但Render3d不能在BeginScene与EndScene之间）
DLLEXPORT void delay(int ms);
DLLEXPORT void delay_ms(int ms);
DLLEXPORT void delay_fps(int fps);

//其它时间函数（API和时间查询）
DLLEXPORT void   API_Sleep(DWORD dwMilliseconds);
DLLEXPORT double fclock();

DLLEXPORT HWND      GetHWnd();         // 获取绘图窗口句柄
DLLEXPORT HINSTANCE GetHInstance();
DLLEXPORT float     GetFPS(int flag = 1); // 获取当前帧率

//键盘处理函数，不能在BeginScene与EndScene之间调用
DLLEXPORT int getkey();
DLLEXPORT int getchEx(int flag = 0);  //flag参数如果为1（不能是其它值），表示把KEY_DOWN和KEY_UP事件取得，返回值会包含KEYMSG_DOWN或者KEYMSG_UP位
DLLEXPORT int kbhitEx(int flag = 0);  //(接上)而低四位是虚拟键码，与flag=0时特殊按键的键码会不相同
DLLEXPORT int keystate(int key);      //获得键码为key的键是否按下，全局有效

//#ifndef _GRAPH_LIB_BUILD_
#if !defined(_INC_CONIO) && !defined(_CONIO_H_)
#define _INC_CONIO
#define _CONIO_H_
#define getch getchEx
#define kbhit kbhitEx
#endif
//#endif

// 鼠标消息处理函数，不能在BeginScene与EndScene之间调用
// 支持如下消息：
//      WM_MOUSEMOVE        鼠标移动
//      WM_MOUSEWHEEL       鼠标滚轮拨动
//      WM_LBUTTONDOWN      左键按下
//      WM_LBUTTONUP        左键弹起
//      WM_LBUTTONDBLCLK    左键双击
//      WM_MBUTTONDOWN      中键按下
//      WM_MBUTTONUP        中键弹起
//      WM_MBUTTONDBLCLK    中键双击
//      WM_RBUTTONDOWN      右键按下
//      WM_RBUTTONUP        右键弹起
//      WM_RBUTTONDBLCLK    右键双击

DLLEXPORT int       MouseHit();                 // 检查是否存在鼠标消息
DLLEXPORT void      FlushMouseMsgBuffer();      // 清空鼠标消息缓冲区
DLLEXPORT MOUSEMSG  GetMouseMsg(int bLogicPos = 1);              // 获取一个鼠标消息。如果没有，就等待
DLLEXPORT int       ShowMouse(int bShow);       // 设置是否显示鼠标
DLLEXPORT int       GetMousePos(int *x, int *y, int bLogicPos = 1);

//随机函数（作为方便使用而独立的函数，不需要先initgraph直接可以调用）
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
