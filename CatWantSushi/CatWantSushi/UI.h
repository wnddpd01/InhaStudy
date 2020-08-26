#pragma once
#include <Windows.h>

class UI
{
	UINT ui_id_;
	RECT ui_rect_;
#pragma region GetterAndSetter
public:
	UINT ui_id() const
	{
		return ui_id_;
	}

	void set_ui_id(UINT ui_id)
	{
		ui_id_ = ui_id;
	}

	__declspec(property(get = ui_id, put = set_ui_id)) UINT UiId;
	RECT ui_rect() const
	{
		return ui_rect_;
	}

	void set_ui_hrgn(RECT ui_rect)
	{
		ui_rect_ = ui_rect;
	}

	__declspec(property(get = ui_rect, put = set_ui_rect)) RECT UiRect;
#pragma endregion

public:
	UI(UINT ui_id, RECT ui_rect)
		: ui_id_(ui_id),
		ui_rect_(ui_rect)
	{
	}
	UI();
	~UI();

	BOOL PointInUI(POINT & pt)
	{
		return PtInRect(&ui_rect_, pt);
	}
	void (*ui_proc)();
	void(*ui_on_mouse)();
};

