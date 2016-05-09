/*
 * author      御坂美琴
 * E-mail      fmmmm@qq.com
 * filename    stdafx.h
 * date        20110105
 */
#ifndef WINVER
#define WINVER 0x0501
#endif

#ifndef _WIN32_WINNT
#define _Win32_WINNT 0x0501
#endif

#ifndef _WIN32_WINDOWS
#define _WIN32_WINDOWS 0x0501
#endif

#if !defined(__STDAFX_H__)
#define __STDAFX_H__
#pragma once

#define _USE_IMGTHIS_CALL_
#include "pex3d.h"
using namespace PEX3D;

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


typedef struct pointint3d
{
    int x, y, z;
}pti3d;
int add(pti3d* a, pti3d* b);

typedef struct pick_info
{
    int info;
    float z;
}pick_info;

#endif //__STDAFX_H__
