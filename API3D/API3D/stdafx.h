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
#include <cmath>

#define PI 3.1415982
// TODO: reference additional headers your program requires here
#include "cVector3.h"
#include "cMatrix.h"


inline double rad_to_deg(double radian)
{
	return radian * 180 / PI;
}
inline double deg_to_rad(double degree)
{
	return degree*PI / 180;
}

