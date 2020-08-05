// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
// Windows Header Files:
#include <Windows.h>

// C RunTime Header Files
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>


// TODO: reference additional headers your program requires here
#include "GameManager.h"
#include "BitMapManager.h"
#include "Polygon.h"

#define WINDOW_WIDTH 1024
#define WINDOW_HEIGHT 768

inline bool isEqualPoint(const POINT& p1, const POINT& p2)
{
	if (p1.x == p2.x && p1.y == p2.y)
		return true;
	return false;
}

inline int getPointDirection(const POINT& p1, const POINT& p2)
{
	if (p2.x - p1.x == 0)
	{
		if (p2.y - p1.y > 0)
			return VK_DOWN;
		else if (p2.y - p1.y < 0)
			return VK_UP;
	}
	else if (p2.y - p1.y == 0)
	{
		if (p2.x - p1.x > 0)
			return VK_RIGHT;
		else if (p2.x - p1.x < 0)
			return VK_LEFT;
	}
	return 0;
}

inline double getPointDistance(const POINT& p1, const POINT& p2)
{
	return sqrt(pow(p2.x - p1.x, 2) + pow(p2.y - p1.y, 2));
}

inline bool BoundaryCheck(const POINT& p)
{
	if (p.x < 8 || p.x > WINDOW_WIDTH - 8|| p.y < 8 || p.y > WINDOW_HEIGHT - 8)
		return false;
	return true;
}

