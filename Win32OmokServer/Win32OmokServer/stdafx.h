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
#include <map>
#include <string>
#include <WinSock2.h>
#include "../../MyPacket/MyPacket/MyPacket.h"
#pragma comment(lib, "ws2_32.lib")
#pragma comment(lib, "../../MyPacket/Debug/MyPacket.lib")
#define WM_ASYNC WM_USER + 2
