#pragma once
#include "AnimateObject.h"
#include "GameOptionManager.h"
#include "InGameScene.h"

struct tile;
enum animation_state
{
	player_idle = 0, player_walk, player_jump, player_guard
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
	const CHAR max_x_fos_ = GameOptionManager::GetInstance()->GameCellSize * 0.4;
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
	Player(UINT object_id,const UCHAR& posX, const UCHAR& posY)
		: AnimateObject(
			object_id, NULL, posX, posY, 7, 7, FALSE, 0, 0,
			0, nullptr)
	{
		if (posX < GameOptionManager::GetInstance()->HorizontalGridCount * 0.5)
			direction_ = dir_left;
		else
			direction_ = dir_left;

		LoadPlayerBitmap(object_id);
		set_animation(player_idle);
		x_fos_ = y_fos_ = 0;
		x_spd_ = GameOptionManager::GetInstance()->GameCellSize * 0.1;
		gravity_ = GameOptionManager::GetInstance()->GameCellSize * 0.05;
		jump_power_ = GameOptionManager::GetInstance()->GameCellSize * 1.0;
		player_rect_.left = object_rect_.left;
		player_rect_.right = object_rect_.right;
		player_rect_.top = object_rect_.top;
		player_rect_.bottom = object_rect_.bottom;
		jump_key_pressed_ = FALSE;
		jump_key_count_ = 0;
	}

	void LoadPlayerBitmap(UINT player_type);
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

