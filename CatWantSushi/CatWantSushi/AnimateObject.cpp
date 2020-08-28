#include "AnimateObject.h"


void AnimateObject::render(HDC hdc, HDC backHDC)
{
	static UINT cnt = 0;
	cnt += 1;
	if(cnt == animation_change_frame_count_)
	{
		cnt = 0;
		++current_animaiton_frame_;
		if (current_animaiton_frame_ == animation_size_)
			current_animaiton_frame_ = 0;
		ObjectBitmapId = animation_bitmap_ids_[current_animaiton_frame_];
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
