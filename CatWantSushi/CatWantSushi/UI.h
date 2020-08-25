#pragma once
#include <Windows.h>
class UI
{
	POINT pos_point_;
	UINT width_;
	UINT height_;
	HBITMAP ui_hbitmap_;
public:
	UI(const POINT& pos_point, UINT width, UINT height, HBITMAP ui_hbitmap, void(* ui_proc)())
		: pos_point_(pos_point),
		  width_(width),
		  height_(height),
		  ui_hbitmap_(ui_hbitmap),
		  ui_proc(ui_proc)
	{
	}

	UI();
	~UI();

	void (*ui_proc)();
};

