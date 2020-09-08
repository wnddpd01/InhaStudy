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
		imageMap.insert(std::pair<UINT, HBITMAP>(BITMAP_INGAME_SCENE_BACKGROUND, LoadBitmap(hInstance, MAKEINTRESOURCE(BITMAP_INGAME_SCENE_BACKGROUND))));
		imageMap.insert(std::pair<UINT, HBITMAP>(BITMAP_UI_TIMER, LoadBitmap(hInstance, MAKEINTRESOURCE(BITMAP_UI_TIMER))));
		imageMap.insert(std::pair<UINT, HBITMAP>(BITMAP_INGAME_SCENE_UI_CAT_YELLOW, LoadBitmap(hInstance, MAKEINTRESOURCE(BITMAP_INGAME_SCENE_UI_CAT_YELLOW))));
		imageMap.insert(std::pair<UINT, HBITMAP>(BITMAP_INGAME_SCENE_UI_CAT_BLUE, LoadBitmap(hInstance, MAKEINTRESOURCE(BITMAP_INGAME_SCENE_UI_CAT_BLUE))));
		imageMap.insert(std::pair<UINT, HBITMAP>(BITMAP_OBJECT_PLATFORM, LoadBitmap(hInstance, MAKEINTRESOURCE(BITMAP_OBJECT_PLATFORM))));
		imageMap.insert(std::pair<UINT, HBITMAP>(BITMAP_OBJECT_RAIL_1, LoadBitmap(hInstance, MAKEINTRESOURCE(BITMAP_OBJECT_RAIL_1))));
		imageMap.insert(std::pair<UINT, HBITMAP>(BITMAP_OBJECT_RAIL_2, LoadBitmap(hInstance, MAKEINTRESOURCE(BITMAP_OBJECT_RAIL_2))));
		imageMap.insert(std::pair<UINT, HBITMAP>(BITMAP_TEXT_1, LoadBitmap(hInstance, MAKEINTRESOURCE(BITMAP_TEXT_1))));
		imageMap.insert(std::pair<UINT, HBITMAP>(BITMAP_TEXT_2, LoadBitmap(hInstance, MAKEINTRESOURCE(BITMAP_TEXT_2))));
		imageMap.insert(std::pair<UINT, HBITMAP>(BITMAP_TEXT_3, LoadBitmap(hInstance, MAKEINTRESOURCE(BITMAP_TEXT_3))));
		
		imageMap.insert(std::pair<UINT, HBITMAP>(BITMAP_CAT_BLUE_IDLE, LoadBitmap(hInstance, MAKEINTRESOURCE(BITMAP_CAT_BLUE_IDLE))));
		imageMap.insert(std::pair<UINT, HBITMAP>(BITMAP_CAT_BLUE_WALK_1, LoadBitmap(hInstance, MAKEINTRESOURCE(BITMAP_CAT_BLUE_WALK_1))));
		imageMap.insert(std::pair<UINT, HBITMAP>(BITMAP_CAT_BLUE_WALK_2, LoadBitmap(hInstance, MAKEINTRESOURCE(BITMAP_CAT_BLUE_WALK_2))));
		imageMap.insert(std::pair<UINT, HBITMAP>(BITMAP_CAT_BLUE_WALK_3, LoadBitmap(hInstance, MAKEINTRESOURCE(BITMAP_CAT_BLUE_WALK_3))));
		imageMap.insert(std::pair<UINT, HBITMAP>(BITMAP_CAT_BLUE_JUMP_1, LoadBitmap(hInstance, MAKEINTRESOURCE(BITMAP_CAT_BLUE_JUMP_1))));
		imageMap.insert(std::pair<UINT, HBITMAP>(BITMAP_CAT_BLUE_JUMP_2, LoadBitmap(hInstance, MAKEINTRESOURCE(BITMAP_CAT_BLUE_JUMP_2))));
		imageMap.insert(std::pair<UINT, HBITMAP>(BITMAP_CAT_BLUE_JUMP_3, LoadBitmap(hInstance, MAKEINTRESOURCE(BITMAP_CAT_BLUE_JUMP_3))));

		imageMap.insert(std::pair<UINT, HBITMAP>(BITMAP_CAT_YELLOW_IDLE, LoadBitmap(hInstance, MAKEINTRESOURCE(BITMAP_CAT_YELLOW_IDLE))));
		imageMap.insert(std::pair<UINT, HBITMAP>(BITMAP_CAT_YELLOW_WALK_1, LoadBitmap(hInstance, MAKEINTRESOURCE(BITMAP_CAT_YELLOW_WALK_1))));
		imageMap.insert(std::pair<UINT, HBITMAP>(BITMAP_CAT_YELLOW_WALK_2, LoadBitmap(hInstance, MAKEINTRESOURCE(BITMAP_CAT_YELLOW_WALK_2))));
		imageMap.insert(std::pair<UINT, HBITMAP>(BITMAP_CAT_YELLOW_WALK_3, LoadBitmap(hInstance, MAKEINTRESOURCE(BITMAP_CAT_YELLOW_WALK_3))));
		imageMap.insert(std::pair<UINT, HBITMAP>(BITMAP_CAT_YELLOW_JUMP_1, LoadBitmap(hInstance, MAKEINTRESOURCE(BITMAP_CAT_YELLOW_JUMP_1))));
		imageMap.insert(std::pair<UINT, HBITMAP>(BITMAP_CAT_YELLOW_JUMP_2, LoadBitmap(hInstance, MAKEINTRESOURCE(BITMAP_CAT_YELLOW_JUMP_2))));
		imageMap.insert(std::pair<UINT, HBITMAP>(BITMAP_CAT_YELLOW_JUMP_3, LoadBitmap(hInstance, MAKEINTRESOURCE(BITMAP_CAT_YELLOW_JUMP_3))));
		
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

