#pragma once

#include "resource.h"

extern HINSTANCE hInst;

class BitMapManager
{
	BitMapManager()
	{
		bitMapGangjinCharacter	= LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP_GangJinCharacter));
		bitMapMadeInAbyss		= LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP_MadeInAbyss));
		bitMapForMee			= LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP_ForMee));
		bitMapCostco			= LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP_Costco));
		bitMapMountain			= LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP_Mountain));
	}
public :
	HBITMAP bitMapGangjinCharacter;
	HBITMAP bitMapMadeInAbyss;
	HBITMAP bitMapForMee;
	HBITMAP bitMapCostco;
	HBITMAP bitMapMountain;
	static BitMapManager* getInstance()
	{
		static BitMapManager ins;
		return &ins;
	}
};

