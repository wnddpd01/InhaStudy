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
#include <iostream>
#include <string>

enum class SceneState : size_t
{
	start, ingame, end
};

enum TimerID : UINT_PTR
{
	frameTimer, updateGameTimer, enemyCreateTimer, timeTimer
};
// TODO: reference additional headers your program requires here

#include "SceneManager.h"
#include "Scene.h"
#include "Singleton.h"
