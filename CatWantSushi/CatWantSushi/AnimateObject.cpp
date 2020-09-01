#include "AnimateObject.h"


void AnimateObject::render(HDC hdc, HDC backHDC)
{
	current_frame_++;
	if (current_animation_idx_ < animation_size_ && current_frame_ == animation_change_frame_count_)
	{
		ObjectBitmapId = animation_bitmap_ids_[current_animation_idx_];
		++current_animation_idx_;
		if (current_animation_idx_ == animation_size_ && is_loop_ == TRUE)
		{
			current_animation_idx_ = 0;
		}
		current_frame_ = 0;
	}
	Object::render(hdc, backHDC);
}

AnimateObject::AnimateObject()
{
}


AnimateObject::~AnimateObject()
{
	delete[] animation_bitmap_ids_;
}
