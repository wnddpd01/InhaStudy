#include "SceneManager.h"
#include "StartScene.h"


SceneManager::SceneManager()
{
	cur_scene_ = start_scene_ = new StartScene;
}

void SceneManager::render(LPPAINTSTRUCT ps)
{
	HDC backHDC = CreateCompatibleDC(ps->hdc);
	HBITMAP backHBit = CreateCompatibleBitmap(ps->hdc, ps->rcPaint.right - ps->rcPaint.left, ps->rcPaint.bottom - ps->rcPaint.top);
	HBITMAP oldHBit = (HBITMAP)SelectObject(backHDC, backHBit);
	cur_scene_->render(backHDC, &ps->rcPaint);
	BitBlt(ps->hdc, ps->rcPaint.left, ps->rcPaint.top, ps->rcPaint.right - ps->rcPaint.left, ps->rcPaint.bottom - ps->rcPaint.top, backHDC, 0, 0, SRCCOPY);
	SelectObject(backHDC, oldHBit);
	DeleteObject(backHBit);
	DeleteDC(backHDC);
}

void SceneManager::scene_change(WPARAM next_scene)
{
}


SceneManager::~SceneManager() 
{
}
