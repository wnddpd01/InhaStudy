#pragma once
#include "resource.h"
#include <Windows.h>
#include <map>

class BitmapManager
{
	BitmapManager()
	{
		HINSTANCE hInstance = GetModuleHandle(NULL);
		imageMap.insert(std::pair<UINT, HBITMAP>(BITMAP_START_SCENE_BACKGROUND, LoadBitmap(hInstance, MAKEINTRESOURCE(BITMAP_START_SCENE_BACKGROUND))));
		imageMap.insert(std::pair<UINT, HBITMAP>(BITMAP_START_BTN, LoadBitmap(hInstance, MAKEINTRESOURCE(BITMAP_START_BTN))));
	}
public:
	std::map<UINT, HBITMAP> imageMap;
	static BitmapManager * GetInstance()
	{
		static BitmapManager ins;
		return &ins;
	}
	
	~BitmapManager()
	{
	}
};

