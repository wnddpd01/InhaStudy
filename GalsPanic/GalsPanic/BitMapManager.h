#pragma once

#include "resource.h"

extern HINSTANCE hInst;

class BitMapManager
{
	BitMapManager()
	{
		bitMapGangjinCharacter = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP_GangJinCharacter));
	}
public :
	HBITMAP bitMapGangjinCharacter;
	static BitMapManager* getInstance()
	{
		static BitMapManager ins;
		return &ins;
	}
};

