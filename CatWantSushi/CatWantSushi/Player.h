#pragma once
#include "AnimateObject.h"
#include "GameOptionManager.h"

struct tile;
enum animation_state
{
	player_idle = 0, player_walk, player_jump, player_guard
};

enum player_type
{
	cat_blue = 0, cat_yellow
};

struct RECT_FLOAT
{
	FLOAT left;
	FLOAT right;
	FLOAT top;
	FLOAT bottom;
};

class Player :
	public AnimateObject
{
private:
	animation_state animation_state_;
	UCHAR animation_idle_size;
	UCHAR animation_idle_change_frame;
	UCHAR* animation_idle_bitmap_ids;
	UCHAR animation_walk_size;
	UCHAR animation_walk_change_frame;
	UCHAR* animation_walk_bitmap_ids;
	UCHAR animation_jump_size;
	UCHAR animation_jump_change_frame;
	UCHAR* animation_jump_bitmap_ids;
	const CHAR max_x_fos_ = GameOptionManager::GetInstance()->GameCellSize / 2;
	FLOAT x_fos_;
	FLOAT y_fos_;
	FLOAT x_spd_;
	BOOL jump_key_pressed_;
	UCHAR jump_key_count_;
	FLOAT jump_power_;
	FLOAT gravity_;
	tile** map_;
	RECT_FLOAT player_rect_;
public:
	Player(UINT object_id, UINT object_bitmap_id, const UCHAR& posX, const UCHAR& posY, const UCHAR& width,
		const UCHAR& height, BOOL is_loop, UCHAR animation_size, UCHAR current_animation_frame,
		UCHAR animation_change_frame_count, UCHAR* animation_bitmap_ids)
		: AnimateObject(
			object_id, object_bitmap_id, posX, posY, width, height, is_loop, animation_size, current_animation_frame,
			animation_change_frame_count, animation_bitmap_ids)
	{
		if (posX < GameOptionManager::GetInstance()->HorizontalGridCount * 0.5)
			direction_ = dir_right;
		else
			direction_ = dir_left;
		
		x_fos_ = y_fos_ = 0;
		x_spd_ = GameOptionManager::GetInstance()->GameCellSize * 0.2;
		gravity_ = GameOptionManager::GetInstance()->GameCellSize * 0.1;
		jump_power_ = GameOptionManager::GetInstance()->GameCellSize * 1.45;
		player_rect_.left = object_rect_.left;
		player_rect_.right = object_rect_.right;
		player_rect_.top = object_rect_.top;
		player_rect_.bottom = object_rect_.bottom;
		jump_key_pressed_ = FALSE;
		jump_key_count_ = 0;
		LoadPlayerBitmap(cat_blue);
		set_animation(player_idle);
	}

	void LoadPlayerBitmap(player_type player_type);
	void PlayerMove(UCHAR dir);
	void PlayerJumpKeyPressed();
	void PlayerJump(FLOAT jump_strength = 1);
	void set_animation(animation_state animation_state);
	void update() override;
	void LoadMap(tile** map);

	BOOL isOnLand();
	INT getDistanceToLand();
	Player();
	~Player();
};

