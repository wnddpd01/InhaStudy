#include "SceneManager.h"

#include <iostream>

#include "StartScene.h"
#include "InGameScene.h"
#include "GlobalValue.h"

SceneManager::SceneManager()
{
	cur_scene_ = start_scene_ = new StartScene;
	in_game_scene_ = new InGameScene;
}

void SceneManager::render(LPPAINTSTRUCT ps)
{
	HDC backHDC = CreateCompatibleDC(ps->hdc);
	HBITMAP backHBit = CreateCompatibleBitmap(ps->hdc, ps->rcPaint.right - ps->rcPaint.left, ps->rcPaint.bottom - ps->rcPaint.top);
	HBITMAP oldHBit = (HBITMAP)SelectObject(backHDC, backHBit);
	SetStretchBltMode(backHDC, COLORONCOLOR);
	SetBkMode(backHDC, TRANSPARENT);
	cur_scene_->render(backHDC, &ps->rcPaint);
	BitBlt(ps->hdc, ps->rcPaint.left, ps->rcPaint.top, ps->rcPaint.right - ps->rcPaint.left, ps->rcPaint.bottom - ps->rcPaint.top, backHDC, 0, 0, SRCCOPY);
	SelectObject(backHDC, oldHBit);
	DeleteObject(backHBit);
	DeleteDC(backHDC);
}

void SceneManager::scene_change(WPARAM next_scene)
{
	delete cur_scene_;
	switch (next_scene)
	{
	case SCENE_START:
	{
		cur_scene_ = start_scene_;
		break;
	}
	case SCENE_INGAME:
	{
		cur_scene_ = in_game_scene_;
		break;
	}
	default:
		break;
	}
}

void SceneManager::keyboard_input_handle(UCHAR key)
{
	cur_scene_->keyInputHandle(key);
}

void SceneManager::mouse_event_handle(WPARAM event_name, POINT& pt)
{
	switch (event_name)
	{
	case WM_LBUTTONUP:
	{
		cur_scene_->mouse_left_up(pt);
		break;
	}
	default:
		break;
	}
}

void SceneManager::update()
{
	cur_scene_->update();
}


SceneManager::~SceneManager()
{
}
