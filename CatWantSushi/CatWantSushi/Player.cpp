#include "Player.h"
#include "InGameScene.h"



void Player::LoadPlayerBitmap(player_type player_type)
{
	GameOptionManager &game_option_manager = *GameOptionManager::GetInstance();
	animation_idle_size = 1;
	animation_idle_change_frame = 0;
	animation_idle_bitmap_ids = new UCHAR[animation_idle_size];

	animation_walk_size = 3;
	animation_walk_bitmap_ids = new UCHAR[animation_walk_size];
	animation_walk_change_frame = 5;

	animation_jump_size = 3;
	animation_jump_bitmap_ids = new UCHAR[animation_jump_size];
	animation_jump_change_frame = game_option_manager.Frame / 10;
	if(player_type == cat_blue)
	{
		animation_idle_bitmap_ids[0] = BITMAP_CAT_BLUE_IDLE;

		animation_walk_bitmap_ids[0] = BITMAP_CAT_BLUE_WALK_1;
		animation_walk_bitmap_ids[1] = BITMAP_CAT_BLUE_WALK_2;
		animation_walk_bitmap_ids[2] = BITMAP_CAT_BLUE_WALK_3;

		animation_jump_bitmap_ids[0] = BITMAP_CAT_BLUE_JUMP_1;
		animation_jump_bitmap_ids[1] = BITMAP_CAT_BLUE_JUMP_2;
		animation_jump_bitmap_ids[2] = BITMAP_CAT_BLUE_JUMP_3;
	}
}

void Player::PlayerMove(UCHAR dir)
{
	if(dir != direction_)
		direction_ = direction(dir);
	if(dir == dir_left)
	{
		if (x_fos_ > -max_x_fos_)
		{
			x_fos_ -= x_spd_;
		}
		else
			x_fos_ = -max_x_fos_;
	}
	else if(dir == dir_right)
	{
		if (x_fos_ < max_x_fos_)
		{
			x_fos_ += x_spd_;
		}
		else
			x_fos_ = max_x_fos_;
	}
	
}

void Player::PlayerJumpKeyPressed()
{
	if (isOnLand() == TRUE && y_fos_ == 0)
	{
		jump_key_pressed_ = TRUE;
		jump_key_count_++;
	}
}

void Player::PlayerJump(FLOAT jump_strength)
{
	
	y_fos_ = jump_power_ * jump_strength;
	set_animation(player_jump);
}

void Player::update()
{
	static DOUBLE max_jump_key_count = GameOptionManager::GetInstance()->Frame * 0.15;
	if((jump_key_pressed_ == FALSE && jump_key_count_ > 0) || jump_key_count_ == max_jump_key_count)
	{
		DOUBLE jump_strength = (DOUBLE)jump_key_count_ / max_jump_key_count;
		if (jump_strength < 0.4)
			jump_strength = 0.55;
		else if (jump_strength < 0.65)
			jump_strength = 0.8;
		else
			jump_strength = 1;
		PlayerJump(jump_strength);
		jump_key_count_ = 0;
	}
	if(jump_key_pressed_ == TRUE)
		jump_key_pressed_ = FALSE;
	
	if (isOnLand() == TRUE && !(y_fos_ > 0))
	{
		if (INT(x_fos_) == 0)
		{
			x_fos_ = 0;
			set_animation(player_idle);
		}
	}
	else
	{
		if (animation_state_ != player_jump)
			set_animation(player_jump);
		INT dis = getDistanceToLand();
		if (abs(dis) < abs(y_fos_))
		{
			player_rect_.top += dis;
			player_rect_.bottom += dis;
			y_fos_ = 0;
		}
		else
		{
			player_rect_.top -= y_fos_;
			player_rect_.bottom -= y_fos_;
			y_fos_ -= gravity_;
		}
		object_rect_.top = player_rect_.top;
		object_rect_.bottom = player_rect_.bottom;
		set_object_pos();
		if (isOnLand() == TRUE)
		{
			y_fos_ = 0;
			set_animation(player_idle);
		}
	}

	if(x_fos_ != 0)
	{
		if (animation_state_ != player_walk && animation_state_ != player_jump)
		{
			set_animation(player_walk);
		}
		player_rect_.left += x_fos_;
		player_rect_.right += x_fos_;
		object_rect_.left = player_rect_.left;
		object_rect_.right = player_rect_.right;
		x_fos_ -= x_fos_ * 0.1;
	}

	set_object_pos();
}

void Player::LoadMap(tile_state** map)
{
	map_ = map;
}

BOOL Player::isOnLand()
{
	if (ObjectPos.y < 0)
		return FALSE;
	if (map_[ObjectPos.y + ObjectWidthHeight.y][ObjectPos.x + 4] != TILE_NULL || map_[ObjectPos.y + ObjectWidthHeight.y][ObjectPos.x + 3] != TILE_NULL)
		return TRUE;
	return FALSE;
}

INT Player::getDistanceToLand()
{
	GameOptionManager* game_option_manager = GameOptionManager::GetInstance();
	LONG search_idx = 0;
	CHAR dir = 0;
	INT cell_rest_ = 0;
	if (y_fos_ < 0)
	{
		cell_rest_ = game_option_manager->GameCellSize - ObjectRect.bottom % game_option_manager->GameCellSize;
		search_idx = ObjectPos.y + ObjectWidthHeight.y + 1;
		dir = 1;
	}
	else
	{
		cell_rest_ = game_option_manager->GameCellSize - ObjectRect.top % game_option_manager->GameCellSize;
		search_idx = ObjectPos.y;
		dir = -1;
	}
	LONG cnt = 0;
	while (search_idx + cnt * dir < game_option_manager->VerticalGridCount && search_idx + cnt * dir > -1
		&&(map_[search_idx + cnt * dir][ObjectPos.x + 4] == TILE_NULL && map_[search_idx + cnt * dir][ObjectPos.x + 3] == TILE_NULL) )
		cnt++;

	if (search_idx + cnt * dir == -1)
		return INT_MAX;
	
	return (cnt * game_option_manager->GameCellSize + cell_rest_) * dir;
}

void Player::set_animation(animation_state animation_state)
{
	animation_state_ = animation_state;
	current_animation_idx_ = 0;
	switch (animation_state)
	{
	case player_idle:
	{
		animation_bitmap_ids_ = animation_idle_bitmap_ids;
		animation_size_ = animation_idle_size;
		break;
	}
	case player_walk:
	{
		animation_bitmap_ids_ = animation_walk_bitmap_ids;
		animation_change_frame_count_ = animation_walk_change_frame;
		is_loop_ = true;
		animation_size_ = animation_walk_size;
		break;
	}
	case player_jump:
	{
		animation_bitmap_ids_ = animation_jump_bitmap_ids;
		animation_change_frame_count_ = animation_jump_change_frame;
		is_loop_ = false;
		animation_size_ = animation_jump_size;
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
