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
#include <queue>

#define INF ((size_t) - 1) 

inline BOOL Circle(HDC hdc, size_t x, size_t y, size_t r)
{
	return Ellipse(hdc, x - r, y - r, x + r, y + r);
}

inline size_t getDistance(POINT & pt1, POINT & pt2)
{
	return abs(pt1.x - pt2.x) + abs(pt1.y - pt2.y);
}

inline size_t getDistance(int i1, int j1, int i2, int j2)
{
	return abs(i1 - i2) + abs(j1 - j2);
}

inline bool operator==(POINT & p1, POINT & p2)
{
	if (p1.x == p2.x && p1.y == p2.y)
		return true;
	return false;
}