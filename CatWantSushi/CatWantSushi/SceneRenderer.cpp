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
		static BITMAP ui_bitmap;
		SelectObject(backHDC, bitmap_manager->imageMap.find(ui->UiBitmapId)->second);
		GetObject(bitmap_manager->imageMap.find(ui->UiBitmapId)->second, sizeof(BITMAP), &ui_bitmap);
		TransparentBlt(hdc, ui->UiRect.left, ui->UiRect.top, ui->UiRect.right - ui->UiRect.left, ui->UiRect.bottom - ui->UiRect.top, backHDC, 0, 0, ui_bitmap.bmWidth, ui_bitmap.bmHeight, RGB(255, 0, 255));
	}
	DeleteDC(backHDC);
}
