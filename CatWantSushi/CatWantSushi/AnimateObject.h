#pragma once
#include "Object.h"
class AnimateObject :
	public Object
{
protected:
	void animateRender(HDC hdc, HDC backHDC);
	BOOL is_loop_;
	UCHAR animation_size_;
	UCHAR current_frame_;
	UCHAR current_animation_idx_;
	UCHAR animation_change_frame_count_;
	UCHAR * animation_bitmap_ids_;


public:
	AnimateObject(UINT object_id, UINT object_bitmap_id, const UCHAR& posX, const UCHAR& posY, const UCHAR& width,
		const UCHAR& height, BOOL is_loop, UCHAR animation_size, UCHAR current_animation_idx,
		UCHAR animation_change_frame_count, UCHAR* animation_bitmap_ids)
		: Object(object_id, object_bitmap_id, posX, posY, width, height),
		  is_loop_(is_loop),
		  animation_size_(animation_size),
		  current_animation_idx_(current_animation_idx),
		  animation_change_frame_count_(animation_change_frame_count),
		  animation_bitmap_ids_(animation_bitmap_ids)
	{
		current_frame_ = 0;
		render = static_cast<void( Object::*)(HDC hdc, HDC backHDC)>(&AnimateObject::animateRender);
	}

	AnimateObject();
	~AnimateObject();
	
};

