#include "StartScene.h"
#include "BitmapManager.h"
#include "GameOptionManager.h"
#include "UI.h"
#include <Windows.h>

BitmapManager * bitmap_manager = BitmapManager::GetInstance();

void StartScene::DrawBackGround(HDC hdc, const LPRECT paint_rect)
{
	HDC backHDC = CreateCompatibleDC(hdc);
	static HBITMAP hbitmapBackground = bitmap_manager->imageMap.find(BITMAP_START_SCENE_BACKGROUND)->second;
	BITMAP bitmap_background;
	GetObject(hbitmapBackground, sizeof(bitmap_background), &bitmap_background);
	SelectObject(backHDC, hbitmapBackground);
	StretchBlt(hdc, 0, 0, paint_rect->right, paint_rect->bottom, backHDC, 0, 0, bitmap_background.bmWidth, bitmap_background.bmHeight, SRCCOPY);
	DeleteDC(backHDC);
}

void StartScene::DrawUI(HDC hdc, const LPRECT paint_rect)
{
	HDC backHDC = CreateCompatibleDC(hdc);
	for(UI * ui : scene_UI_)
	{
		static BITMAP ui_bitmap;
		SelectObject(backHDC, bitmap_manager->imageMap.find(ui->UiId)->second);
		GetObject(bitmap_manager->imageMap.find(ui->UiId)->second, sizeof(BITMAP), &ui_bitmap);
		TransparentBlt(hdc, ui->UiRect.left, ui->UiRect.top, ui->UiRect.right - ui->UiRect.left, ui->UiRect.bottom - ui->UiRect.top, backHDC, 0, 0, ui_bitmap.bmWidth, ui_bitmap.bmHeight, RGB(255,0,255));
	}
	DeleteDC(backHDC);
}

StartScene::StartScene()
{
	GameOptionManager *game_option_manager = GameOptionManager::GetInstance();
	size_t cellSize = game_option_manager->GameHeight / game_option_manager->VerticalGridCount;
	LONG startBtnLeft = cellSize * (game_option_manager->HorizontalGridCount / 2 - 2);
	LONG startBtnTop = cellSize * (game_option_manager->VerticalGridCount / 2 + 3);
	RECT startBtnRect = { startBtnLeft, startBtnTop, startBtnLeft + cellSize * 4, startBtnTop + cellSize * 2 };
	UI * startBtn = new UI(BITMAP_START_BTN, startBtnRect);
	startBtn->ui_proc = StartBtnClick;
	scene_UI_.push_back(startBtn);
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
	DrawUI(hdc, paint_rect);
}

void StartScene::free()
{
}

void StartScene::StartBtnClick()
{
}
