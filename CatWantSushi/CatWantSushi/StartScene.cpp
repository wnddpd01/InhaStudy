#include "StartScene.h"
#include <Windows.h>
#include "resource.h"
#include "GameOptionManager.h"
#include "GlobalValue.h"
#include "UI.h"

extern SceneRenderer scene_renderer_;

//void StartScene::DrawBackGround(HDC hdc, const LPRECT paint_rect)
//{
//	HDC backHDC = CreateCompatibleDC(hdc);
//	static HBITMAP hbitmapBackground = bitmap_manager->imageMap.find(BITMAP_START_SCENE_BACKGROUND)->second;
//	BITMAP bitmap_background;
//	GetObject(hbitmapBackground, sizeof(bitmap_background), &bitmap_background);
//	SelectObject(backHDC, hbitmapBackground);
//	StretchBlt(hdc, 0, 0, paint_rect->right, paint_rect->bottom, backHDC, 0, 0, bitmap_background.bmWidth, bitmap_background.bmHeight, SRCCOPY);
//	DeleteDC(backHDC);
//}

void StartScene::DrawUI(HDC hdc, const LPRECT paint_rect)
{
	/*HDC backHDC = CreateCompatibleDC(hdc);
	for(UI * ui : scene_UI_)
	{
		static BITMAP ui_bitmap;
		SelectObject(backHDC, bitmap_manager->imageMap.find(ui->UiId)->second);
		GetObject(bitmap_manager->imageMap.find(ui->UiId)->second, sizeof(BITMAP), &ui_bitmap);
		TransparentBlt(hdc, ui->UiRect.left, ui->UiRect.top, ui->UiRect.right - ui->UiRect.left, ui->UiRect.bottom - ui->UiRect.top, backHDC, 0, 0, ui_bitmap.bmWidth, ui_bitmap.bmHeight, RGB(255,0,255));
	}
	DeleteDC(backHDC);*/
}

void StartScene::CreateUI()
{
	GameOptionManager *game_option_manager = GameOptionManager::GetInstance();
	LONG startBtnLeft =(game_option_manager->HorizontalGridCount / 2 - 4);
	LONG startBtnTop = (game_option_manager->VerticalGridCount / 2 + 6);
	UI * startBtn = new UI(BTN_START,BITMAP_START_BTN, startBtnLeft, startBtnTop, 8 , 4);
	startBtn->ui_mouse_click = StartBtnClick;
	scene_uis_.push_back(startBtn);
}

StartScene::StartScene()
{
	init();
}

StartScene::~StartScene()
{
}

void StartScene::init()
{
	CreateUI();
}

void StartScene::update()
{
}

void StartScene::render(HDC hdc, const LPRECT paint_rect)
{
	scene_renderer_.DrawSceneBackground(hdc, paint_rect, BITMAP_START_SCENE_BACKGROUND);
	scene_renderer_.DrawSceneUI(hdc, paint_rect, scene_uis_);
	//scene_renderer_.DrawGrid(hdc, paint_rect);
}

void StartScene::free()
{
}

void StartScene::mouse_left_up(POINT & pt)
{
	for(UI * ui : scene_uis_)
	{
		if (ui->PointInUI(pt))
			ui->ui_mouse_click();
	}
}

void StartScene::mouse_move(POINT& pt)
{
}

void StartScene::StartBtnClick()
{
	PostMessage(GetActiveWindow(), WM_SCENE_CHANGE, SCENE_INGAME, SCENE_INGAME);
}
