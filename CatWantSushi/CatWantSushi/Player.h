#pragma once
#include "AnimateObject.h"
#include "GameOptionManager.h"
#include "InGameScene.h"
#include "list"

class Object;
struct tile;
enum animation_state
{
	player_idle = 0, player_walk, player_jump, player_guard, player_attacked
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

	UCHAR animation_attacked_size;
	UCHAR* animation_attacked_bitmap_ids;

	
	const CHAR max_x_fos_ = GameOptionManager::GetInstance()->GameCellSize * 0.4;
	FLOAT x_spd_;
	BOOL jump_key_pressed_;
	BOOL attack_key_pressed_;
	BOOL attack_success_;
	UCHAR jump_key_count_;
	FLOAT jump_power_;
	FLOAT gravity_;
	tile* map_;
	RECT_FLOAT player_rect_;
	Object* attackObject;
	Player* enemy_;
public:
	FLOAT x_fos_;
	FLOAT y_fos_;
	std::list<Object*> player_object;
	RECT collide_rect_;
	UINT score;
	Player(UINT object_id,const UCHAR& posX, const UCHAR& posY)
		: AnimateObject(
			object_id, NULL, posX, posY, 7, 7, FALSE, 0, 0,
			0, nullptr)
	{
		if (posX < GameOptionManager::GetInstance()->HorizontalGridCount * 0.5)
			direction_ = dir_right;
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
		attack_key_pressed_ = FALSE;
		attack_success_ = FALSE;
		jump_key_count_ = 0;
		score = 0;
		set_collider_rect();
	}

	void x_fos_update();
	void y_fos_update();
	void LoadPlayerBitmap(UINT player_type);
	void PlayerMove(CHAR dir);
	void PlayerJumpKeyPressed();
	void PlayerJump(FLOAT jump_strength = 1);
	void PlayerAttack();
	void PlayerAttacked();
	void set_animation(animation_state animation_state);
	void set_collider_rect();
	void update() override;
	void LoadMap(tile*** map);
	void LoadEnemy(Player* enemy);

	BOOL isOnLand();
	INT getVerticalDistanceToLand();
	INT getHorizontalDistanceToLand();
	Player();
	~Player();
};

