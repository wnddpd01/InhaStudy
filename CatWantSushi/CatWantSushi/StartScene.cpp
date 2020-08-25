#include "StartScene.h"
#include "BitmapManager.h"
#include "UI.h"
#include <Windows.h>

BitmapManager * bitmap_manager = BitmapManager::GetInstance();

void StartScene::DrawBackGround(HDC hdc, const LPRECT paint_rect)
{
	HDC backHDC = CreateCompatibleDC(hdc);
	HBITMAP hbitmapBackground = bitmap_manager->imageMap.find(BITMAP_START_SCENE_BACKGROUND)->second;
	BITMAP bitmap_background;
	GetObject(hbitmapBackground, sizeof(bitmap_background), &bitmap_background);
	SelectObject(backHDC, hbitmapBackground);
	StretchBlt(hdc, 0, 0, paint_rect->right, paint_rect->bottom, backHDC, 0, 0, bitmap_background.bmWidth, bitmap_background.bmHeight, SRCCOPY);
	DeleteDC(backHDC);
}

void StartScene::DrawUI(HDC hdc, const LPRECT paint_rect)
{
}

StartScene::StartScene()
{
}

StartScene::~StartScene()
{
}

void StartScene::init()
{
}

void StartScene::update()
{
}

void StartScene::render(HDC hdc, const LPRECT paint_rect)
{
	SetStretchBltMode(hdc, COLORONCOLOR);
	DrawBackGround(hdc, paint_rect);
}

void StartScene::free()
{
}
