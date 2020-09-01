#pragma once
#include "AnimateObject.h"
#include "GameOptionManager.h"

enum animation_state
{
	player_idle = 0, player_walk, player_jump, player_guard
};

enum player_type
{
	cat_blue = 0, cat_yellow
};

class Player :
	public AnimateObject
{
private:
	BOOL is_moved_;
	animation_state animation_state_;
	UCHAR animation_idle_size;
	UCHAR animation_idle_change_frame;
	UCHAR* animation_idle_bitmap_ids;
	UCHAR animation_walk_size;
	UCHAR animation_walk_change_frame;
	UCHAR* animation_walk_bitmap_ids;
	const static CHAR max_x_fos = 10;
	CHAR x_fos_;
	CHAR y_fos_;
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
			direction_ = dir_left;
		
		x_fos_ = y_fos_ = 0;
		
		LoadPlayerBitmap(cat_blue);
		set_animation(player_idle);
	}

	void LoadPlayerBitmap(player_type player_type);
	void PlayerMove(UCHAR dir);
	void PlayerJump();
	void set_animation(animation_state animation_state);
	void update() override;

	BOOL isOnLand();
	Player();
	~Player();
};

