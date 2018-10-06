#pragma once

#define WM_SPRITE_DEFAULT_IMAGE (WM_USER)
#define WM_SPRITE_LOADING (WM_USER+1)
#define WM_SPRITE_UPDATING (WM_USER+2)

#define VK_W 0x57
#define VK_A 0x41
#define VK_S 0x53
#define VK_D 0x44
#define VK_Q 0x51
#define VK_E 0x45
#define VK_H 0x48
#define VK_V 0x56
#define VK_L 0x4c

#include <windows.h>
#include <windowsx.h>
#include <gdiplus.h>
#pragma comment(lib,"gdiplus.lib")

#define DEFAULT_BACKGROUND_COLOR GetSysColor(COLOR_3DDKSHADOW)