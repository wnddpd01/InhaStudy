#pragma once
#include "AnimateObject.h"
#include "GameOptionManager.h"

class Player :
	public AnimateObject
{
public:
	Player(UINT object_id, UINT object_bitmap_id, const UCHAR& posX, const UCHAR& posY, const UCHAR& width,
		const UCHAR& height, BOOL is_loop, UCHAR animation_size, UCHAR current_animation_frame,
		UCHAR animation_change_frame_count, UCHAR* animation_bitmap_ids)
		: AnimateObject(
			object_id, object_bitmap_id, posX, posY, width, height, is_loop, animation_size, current_animation_frame,
			animation_change_frame_count, animation_bitmap_ids)
	{
		if (posX < GameOptionManager::GetInstance()->HorizontalGridCount / 2)
			direction_ = dir_right;
		else
			direction_ = dir_right;

	}

	Player();
	~Player();
};

