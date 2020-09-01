#include "Player.h"


void Player::LoadPlayerBitmap(player_type player_type)
{
	if(player_type == cat_blue)
	{
		animation_idle_size = 1;
		animation_idle_change_frame = 0;
		animation_idle_bitmap_ids = new UCHAR[animation_idle_size];
		animation_idle_bitmap_ids[0] = BITMAP_CAT_BLUE_IDLE;

		animation_walk_size = 3;
		animation_walk_bitmap_ids = new UCHAR[animation_walk_size];
		animation_walk_change_frame = 5;
		animation_walk_bitmap_ids[0] = BITMAP_CAT_BLUE_WALK_1;
		animation_walk_bitmap_ids[1] = BITMAP_CAT_BLUE_WALK_2;
		animation_walk_bitmap_ids[2] = BITMAP_CAT_BLUE_WALK_3;
	}
}

void Player::PlayerMove(UCHAR dir)
{
	is_moved_ = TRUE;
	if(animation_state_ != player_walk)
	{
		set_animation(player_walk);
	}
	direction_ = direction(dir);
	if(dir == dir_left)
	{
		if (x_fos_ > -max_x_fos)
		{
			x_fos_ -= 2;
		}
		else
			x_fos_ = -max_x_fos;
	}
	else if(dir == dir_right)
	{
		if (x_fos_ < max_x_fos)
		{
			x_fos_ += 2;
		}
		else
			x_fos_ = max_x_fos;
	}
	
}

void Player::PlayerJump()
{
	if (isOnLand() == TRUE)
	{
		y_fos_ = 10;
	}
}

void Player::update()
{
	if (is_moved_ == FALSE)
	{
		x_fos_ = 0;
		set_animation(player_idle);
	}
	is_moved_ = FALSE;

	if(x_fos_ != 0)
	{
		object_rect_.left += x_fos_;
		object_rect_.right += x_fos_;
	}

	if(isOnLand() == FALSE)
	{
		object_rect_.top -= y_fos_;
		object_rect_.bottom -= y_fos_;
		y_fos_--;
	}
}

BOOL Player::isOnLand()
{
	return TRUE;
}

void Player::set_animation(animation_state animation_state)
{
	animation_state_ = animation_state;
	current_animation_idx_ = 0;
	switch(animation_state)
	{
	case player_idle :
	{
		animation_bitmap_ids_ = animation_idle_bitmap_ids;
		animation_size_ = animation_idle_size;
		break;
	}
	case player_walk :
	{
		animation_bitmap_ids_ = animation_walk_bitmap_ids;
		animation_change_frame_count_ = animation_walk_change_frame;
		is_loop_ = true;
		animation_size_ = animation_walk_size;
		break;
	}
	}
	current_frame_ = animation_change_frame_count_ - 1;
}

Player::Player()
{
}


Player::~Player()
{
}
