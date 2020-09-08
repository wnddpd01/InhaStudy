#include "SceneRenderer.h"
#include "BitmapManager.h"
#include "resource.h"
#include "UI.h"


BitmapManager * bitmap_manager = BitmapManager::GetInstance();

SceneRenderer::SceneRenderer()
{
}


SceneRenderer::~SceneRenderer()
{
}

void SceneRenderer::DrawSceneBackground(HDC hdc, const LPRECT paint_rect, UINT bitmap_id)
{
	BitmapManager * bitmap_manager = BitmapManager::GetInstance();
	HDC backHDC = CreateCompatibleDC(hdc);
	HBITMAP hbitmapBackground = bitmap_manager->imageMap.find(bitmap_id)->second;
	BITMAP bitmap_background;
	GetObject(hbitmapBackground, sizeof(bitmap_background), &bitmap_background);
	SelectObject(backHDC, hbitmapBackground);
	StretchBlt(hdc, 0, 0, paint_rect->right, paint_rect->bottom, backHDC, 0, 0, bitmap_background.bmWidth, bitmap_background.bmHeight, SRCCOPY);
	DeleteDC(backHDC);
}

void SceneRenderer::DrawSceneUI(HDC hdc, const LPRECT paint_rect, std::vector<UI*> scene_uis)
{
	BitmapManager * bitmap_manager = BitmapManager::GetInstance();
	HDC backHDC = CreateCompatibleDC(hdc);
	for (UI * ui : scene_uis)
	{
		(ui->*ui->render)(hdc, backHDC);
	}
	DeleteDC(backHDC);
}

void SceneRenderer::DrawSceneObeject(HDC hdc, const LPRECT paint_rect, std::vector<Object*> scene_objects)
{
	BitmapManager * bitmap_manager = BitmapManager::GetInstance();
	HDC backHDC = CreateCompatibleDC(hdc);
	for(Object * object : scene_objects)
	{
		(object->*object->render)(hdc, backHDC);
	}
	DeleteDC(backHDC);
}

void SceneRenderer::DrawGrid(HDC hdc, const LPRECT paint_rect)
{
	GameOptionManager * game_option_manager = GameOptionManager::GetInstance();
	for (size_t i = 0; i < game_option_manager->HorizontalGridCount; i++)
	{
		MoveToEx(hdc, i * game_option_manager->GameCellSize, 0, NULL);
		LineTo(hdc, i * game_option_manager->GameCellSize, game_option_manager->GameHeight);
	}
	for (size_t i = 0; i < game_option_manager->VerticalGridCount; i++)
	{
		MoveToEx(hdc, 0, i * game_option_manager->GameCellSize, NULL);
		LineTo(hdc, game_option_manager->GameWidth, i * game_option_manager->GameCellSize);
	}
}
