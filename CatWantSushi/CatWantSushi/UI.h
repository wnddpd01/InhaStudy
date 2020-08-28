#pragma once
#include <Windows.h>
#include "GameOptionManager.h"
#include "Object.h"

class UI : public Object
{
	//UINT ui_id_;
	//UINT ui_bitmap_id_;
	//POINT_UCHAR ui_pos_;
	//POINT_UCHAR ui_width_height_;
	//RECT ui_rect_;

	//void set_ui_rect()
	//{
	//	GameOptionManager  * game_option_manager = GameOptionManager::GetInstance();

	//	ui_rect_.left = game_option_manager->GameCellSize * ui_pos_.x;
	//	ui_rect_.top = game_option_manager->GameCellSize * ui_pos_.y;
	//	ui_rect_.right = game_option_manager->GameCellSize * ui_width_height_.x + ui_rect_.left;
	//	ui_rect_.bottom = game_option_manager->GameCellSize * ui_width_height_.y + ui_rect_.top;
	//}
//#pragma region GetterAndSetter
//public:
//	UINT ui_id() const
//	{
//		return ui_id_;
//	}
//
//	void set_ui_id(UINT ui_id)
//	{
//		ui_id_ = ui_id;
//	}
//
//	__declspec(property(get = ui_id, put = set_ui_id)) UINT UiId;
//	UINT ui_bitmap_id() const
//	{
//		return ui_bitmap_id_;
//	}
//
//	void set_ui_bitmap_id(UINT ui_bitmap_id)
//	{
//		ui_bitmap_id_ = ui_bitmap_id;
//	}
//
//	__declspec(property(get = ui_bitmap_id, put = set_ui_bitmap_id)) UINT UiBitmapId;
//	
//	POINT_UCHAR ui_pos() const
//	{
//		return ui_pos_;
//	}
//
//	void set_ui_pos(const POINT_UCHAR& ui_pos)
//	{
//		ui_pos_ = ui_pos;
//	}
//
//	__declspec(property(get = ui_pos, put = set_ui_pos)) POINT_UCHAR UiPos;
//
//	POINT_UCHAR ui_width_height() const
//	{
//		return ui_width_height_;
//	}
//
//	void set_ui_width_height(const POINT_UCHAR& ui_width_height)
//	{
//		ui_width_height_ = ui_width_height;
//	}
//
//	__declspec(property(get = ui_width_height, put = set_ui_width_height)) POINT_UCHAR UiWidthHeight;
//
//	RECT ui_rect() const
//	{
//		return ui_rect_;
//	}
//
//#pragma endregion

public:
	UI(UINT ui_id, UINT ui_bitmap_id, const UCHAR &posX, const UCHAR &posY, const UCHAR &width, const UCHAR &height) :
	Object(ui_id, ui_bitmap_id, posX, posY, width, height)
	{
	}
	UI();
	virtual ~UI() = default;

	BOOL PointInUI(POINT & pt)
	{
		return PtInRect(&ObjectRect, pt);
	}
	void (*ui_mouse_click)();
	void (*ui_on_mouse)();
};

