#pragma once
#include <iostream>
#include <Windows.h>
#include "GameOptionManager.h"
#include "Object.h"
#include <string>
#include <atlstr.h>
class UI : public Object
{

public:
	UINT* ui_data;
	RECT data_draw_rect;
	
	UI(UINT ui_id, UINT ui_bitmap_id, const UCHAR &posX, const UCHAR &posY, const UCHAR &width, const UCHAR &height, direction dir = dir_right) :
	Object(ui_id, ui_bitmap_id, posX, posY, width, height, dir)
	{
		data_draw_rect = ObjectRect;
	}
	UI();
	virtual ~UI() = default;

	BOOL PointInUI(POINT & pt)
	{
		return PtInRect(&ObjectRect, pt);
	}
	void (*ui_mouse_click)();
	void (*ui_on_mouse)();

	void render_dataUI(HDC hdc, HDC backHDC)
	{
		static TCHAR time_string[4];
		memset(time_string, 0, sizeof(TCHAR) * 4);
		wsprintfW(time_string, TEXT("%d"),*ui_data);
		basicRender(hdc, backHDC);
		DrawText(hdc, time_string, -1, &data_draw_rect, DT_SINGLELINE | DT_VCENTER | DT_CENTER);
	}
};

