// stdafx.h : 자주 사용하지만 자주 변경되지는 않는
// 표준 시스템 포함 파일 또는 프로젝트 관련 포함 파일이
// 들어 있는 포함 파일입니다.
//

#pragma once
#ifdef UNICODE
#pragma comment(linker, "/entry:wWinMainCRTStartup /subsystem:console")
#else
#pragma comment(linker, "/entry:WinMainCRTStartup /subsystem:console")
#endif
#include "targetver.h"

#define M_PI 3.14159782
#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
// Converts degrees to radians.
#define degToRad(angleDegrees) (angleDegrees * M_PI / 180.0)

// Converts radians to degrees.
#define radToDeg(angleRadians) (angleRadians * 180.0 / M_PI)

// Windows Header Files:
#include <windows.h>

// C RunTime Header Files
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
#include <iostream>
#include <string>
#include <time.h>
#include <vector>
#include <math.h>

using namespace std;
extern HWND g_hWnd;

// TODO: reference additional headers your program requires here
#include "cVector3.h"
#include "cMatrix.h"
#include "cMainGame.h"