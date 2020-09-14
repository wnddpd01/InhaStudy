#include "AnimateObject.h"

#include <iostream>


void AnimateObject::animateRender(HDC hdc, HDC backHDC)
{
	if(current_animation_idx_ < animation_size_)
		current_frame_++;
	if (current_frame_ > animation_change_frame_count_)
	{
		++current_animation_idx_;
		if (!(current_animation_idx_ < animation_size_))
		{
			if(is_loop_ == TRUE)
				current_animation_idx_ = 0;
			else
			{
				current_frame_ = 0;
				Object::basicRender(hdc, backHDC);
				return;
			}
		}
		ObjectBitmapId = animation_bitmap_ids_[current_animation_idx_];
		current_frame_ = 0;
	}
	Object::basicRender(hdc, backHDC);
}


AnimateObject::AnimateObject()
{
	current_frame_ = 0;
}


AnimateObject::~AnimateObject()
{
	delete[] animation_bitmap_ids_;
}
