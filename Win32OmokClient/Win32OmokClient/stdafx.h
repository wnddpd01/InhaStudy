// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
// Windows Header Files:
#include <windows.h>

// C RunTime Header Files
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>


// TODO: reference additional headers your program requires here
#include <vector>
#include <WinSock2.h>
#include <Windowsx.h>
#include "BitmapManager.h"
#include "MyPacket.h"
#pragma comment(lib, "ws2_32.lib")
#pragma comment(lib, "MyPacket.lib")
#define WM_ASYNC WM_USER + 2

inline BOOL __stdcall Circle(HDC hdc, int x, int y, int r)
{
	return Ellipse(hdc, x - r, y - r, x + r, y + r);
}

inline BOOL __stdcall PtInRect(LPRECT rect, POINTS &pt)
{
	if (pt.x >= rect->left && pt.x <= rect->right && pt.y >= rect->top && pt.y <= rect->bottom)
		return true;
	return false;
}