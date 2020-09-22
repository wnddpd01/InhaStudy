#include "Player.h"

#include <iostream>

#include "InGameScene.h"
#include "Object.h"
extern GameOptionManager* game_option_manager;

void Player::x_fos_update()
{
	if (animation_state_ == player_idle && animation_state_ != player_walk && isOnLand() == TRUE)
	{
		set_animation(player_walk);
	}
	INT dis = getHorizontalDistanceToLand();
	if (abs(FLOAT(dis)) < abs(x_fos_))
	{
		player_rect_.left += dis;
		player_rect_.right += dis;
		x_fos_ = 0;
	}
	else
	{
		player_rect_.left += x_fos_;
		player_rect_.right += x_fos_;
		x_fos_ -= x_fos_ * 0.1;
	}
	object_rect_.left = player_rect_.left;
	object_rect_.right = player_rect_.right;
	collide_rect_.left = object_rect_.left + game_option_manager->GameCellSize * 1;
	collide_rect_.right = collide_rect_.left + game_option_manager->GameCellSize * 3;
}

void Player::y_fos_update()
{
	if (animation_state_ != player_jump)
		set_animation(player_jump);
	INT dis = getVerticalDistanceToLand();
	if (FLOAT(abs(dis)) < abs(y_fos_))
	{
		player_rect_.top += dis;
		player_rect_.bottom += dis;
		set_animation(player_idle);
		y_fos_ = 0;
	}
	else
	{
		player_rect_.top -= y_fos_;
		player_rect_.bottom -= y_fos_;
		y_fos_ -= gravity_;
	}
	object_rect_.top = player_rect_.top;
	collide_rect_.top = object_rect_.top + game_option_manager->GameCellSize;
	collide_rect_.bottom = object_rect_.bottom = player_rect_.bottom;
	set_object_pos();
}

void Player::LoadPlayerBitmap(UINT player_type)
{
	GameOptionManager& game_option_manager = *GameOptionManager::GetInstance();
	animation_idle_size = 1;
	animation_idle_change_frame = 100;
	animation_idle_bitmap_ids = new UCHAR[animation_idle_size];

	animation_walk_size = 3;
	animation_walk_bitmap_ids = new UCHAR[animation_walk_size];
	animation_walk_change_frame = game_option_manager.Frame / 10;

	animation_jump_size = 3;
	animation_jump_bitmap_ids = new UCHAR[animation_jump_size];
	animation_jump_change_frame = game_option_manager.Frame / 10;
	if (player_type == PLAYER_BLUE)
	{
		animation_idle_bitmap_ids[0] = BITMAP_CAT_BLUE_IDLE;

		animation_walk_bitmap_ids[0] = BITMAP_CAT_BLUE_WALK_1;
		animation_walk_bitmap_ids[1] = BITMAP_CAT_BLUE_WALK_2;
		animation_walk_bitmap_ids[2] = BITMAP_CAT_BLUE_WALK_3;

		animation_jump_bitmap_ids[0] = BITMAP_CAT_BLUE_JUMP_1;
		animation_jump_bitmap_ids[1] = BITMAP_CAT_BLUE_JUMP_2;
		animation_jump_bitmap_ids[2] = BITMAP_CAT_BLUE_JUMP_3;


		attackObject = new Object(NULL, BITMAP_CAT_BLUE_ATTACK, 0, 0, 3, 2);

	}
	else if (player_type == PLAYER_YELLOW)
	{
		animation_idle_bitmap_ids[0] = BITMAP_CAT_YELLOW_IDLE;

		animation_walk_bitmap_ids[0] = BITMAP_CAT_YELLOW_WALK_1;
		animation_walk_bitmap_ids[1] = BITMAP_CAT_YELLOW_WALK_2;
		animation_walk_bitmap_ids[2] = BITMAP_CAT_YELLOW_WALK_3;

		animation_jump_bitmap_ids[0] = BITMAP_CAT_YELLOW_JUMP_1;
		animation_jump_bitmap_ids[1] = BITMAP_CAT_YELLOW_JUMP_2;
		animation_jump_bitmap_ids[2] = BITMAP_CAT_YELLOW_JUMP_3;

		attackObject = new Object(NULL, BITMAP_CAT_YELLOW_ATTACK, 0, 0, 3, 2);
	}

	animation_attacked_size = 1;
	animation_attacked_bitmap_ids = animation_jump_bitmap_ids;
}

void Player::PlayerMove(CHAR dir)
{
	if (attack_key_pressed_ == TRUE || animation_state_ == player_attacked)
		return;
	if (dir != direction_)
		direction_ = direction(dir);
	if (dir == dir_left)
	{
		if (x_fos_ - x_spd_ > -max_x_fos_)
		{
			x_fos_ -= x_spd_;
		}
		else
			x_fos_ = -max_x_fos_;
	}
	else if (dir == dir_right)
	{
		if (x_fos_ + x_spd_ < max_x_fos_)
		{
			x_fos_ += x_spd_;
		}
		else
			x_fos_ = max_x_fos_;
	}

}

void Player::PlayerJumpKeyPressed()
{
	if (isOnLand() == TRUE && (animation_state_ == player_idle || animation_state_ == player_walk))
	{
		jump_key_pressed_ = TRUE;
		jump_key_count_ += 1;
	}
	else
	{
		jump_key_count_ = 0;
	}
}

void Player::PlayerJump(FLOAT jump_strength)
{
	y_fos_ = jump_power_ * jump_strength;
	set_animation(player_jump);
}

void Player::PlayerAttack()
{
	attack_key_pressed_ = TRUE;
	RECT attack_object_rect;
	if (direction_ == dir_left)
	{
		attack_object_rect.left = player_rect_.left - game_option_manager->GameCellSize * 2;
	}
	else
	{
		attack_object_rect.left = player_rect_.left + game_option_manager->GameCellSize * 5;
	}
	attack_object_rect.right = attack_object_rect.left + game_option_manager->GameCellSize * 4;
	attack_object_rect.top = player_rect_.top + game_option_manager->GameCellSize * 0.5;
	attack_object_rect.bottom = attack_object_rect.top + game_option_manager->GameCellSize * 3;

	RECT temp;
	if (attack_success_ == FALSE && IntersectRect(&temp, &attack_object_rect, &enemy_->collide_rect_))
	{
		enemy_->x_fos_ = max_x_fos_ * 3 * direction_;
		enemy_->set_animation(player_attacked);
		attack_success_ = TRUE;
	}

	attackObject->ObjectRect = attack_object_rect;
	if (player_object.size() == 0)
		player_object.push_back(attackObject);
}

void Player::PlayerAttacked()
{
}

void Player::update()
{

	if (attack_key_pressed_ == TRUE)
	{
		attack_key_pressed_ = FALSE;
	}
	else
	{
		if (player_object.empty() == FALSE)
		{
			player_object.pop_back();
			attack_success_ = FALSE;
		}
	}


	const static UINT max_jump_key_count = GameOptionManager::GetInstance()->Frame * 0.1;
	if ((jump_key_pressed_ == FALSE && jump_key_count_ > 0) || jump_key_count_ == max_jump_key_count)
	{
		DOUBLE jump_strength = (DOUBLE)jump_key_count_ / max_jump_key_count;
		if (jump_strength < 0.6)
			jump_strength = 0.55;
		else if (jump_strength < 1)
			jump_strength = 0.8;
		else
			jump_strength = 1;
		PlayerJump(jump_strength);
		jump_key_count_ = 0;
	}
	if (jump_key_pressed_ == TRUE)
		jump_key_pressed_ = FALSE;

	if (isOnLand() == TRUE && !(y_fos_ > 0))
	{
		y_fos_ = 0;
		if (INT(x_fos_) == 0)
		{
			set_animation(player_idle);
			x_fos_ = 0;
		}
		else
		{
			if(animation_state_ != player_walk && animation_state_ != player_attacked)
				set_animation(player_walk);
		}
	}
	else
	{
		y_fos_update();
	}

	if (x_fos_ != 0)
	{
		x_fos_update();
	}

	set_object_pos();
}

void Player::LoadMap(tile*** map)
{
	*map_ = *map;
}

void Player::LoadEnemy(Player* enemy)
{
	enemy_ = enemy;
}

BOOL Player::isOnLand()
{
	if (ObjectPos.y < 0)
		return FALSE;
	if ((*map_[ObjectPos.y + ObjectWidthHeight.y][ObjectPos.x + 3]).tile_state != TILE_NULL)
		return TRUE;
	RECT retRect;
	if(enemy_->collide_rect_.top == collide_rect_.bottom && enemy_->collide_rect_.right > collide_rect_.left && enemy_->collide_rect_.left < collide_rect_.right)
		return TRUE;
	return FALSE;
}

INT Player::getVerticalDistanceToLand()
{
	GameOptionManager* game_option_manager = GameOptionManager::GetInstance();
	LONG search_idx = 0;
	CHAR dir = 0;
	INT cell_rest_ = 0;
	//INT player_distance = 0;
	if (y_fos_ < 0)
	{
		cell_rest_ = game_option_manager->GameCellSize - ObjectRect.bottom % game_option_manager->GameCellSize;
		search_idx = ObjectPos.y + ObjectWidthHeight.y + 1;
		dir = 1;
	}
	else
	{
		cell_rest_ = ObjectRect.top % game_option_manager->GameCellSize;
		search_idx = ObjectPos.y;
		dir = -1;
	}
	LONG cnt = 0;


	while (search_idx + cnt * dir < game_option_manager->VerticalGridCount && search_idx + cnt * dir > -1
		&& (*map_[search_idx + cnt * dir][ObjectPos.x + 3]).tile_state == TILE_NULL)
	{
		cnt++;
	}

	INT dis = (cnt * game_option_manager->GameCellSize + cell_rest_);

	RECT tempRect = collide_rect_;
	tempRect.top = enemy_->collide_rect_.top;
	tempRect.bottom = enemy_->collide_rect_.bottom;
	RECT temp;
	if(IntersectRect(&temp, &tempRect, &enemy_->collide_rect_))
	{
		INT player_dis = enemy_->collide_rect_.top - collide_rect_.bottom;
		if (y_fos_ > 0)
			player_dis = collide_rect_.top - enemy_->collide_rect_.bottom;
		if (player_dis >= 0 && player_dis < dis)
			dis = player_dis;
	}
	
	return dis  * dir;
}

INT Player::getHorizontalDistanceToLand()
{
	GameOptionManager* game_option_manager = GameOptionManager::GetInstance();
	LONG search_idx = object_pos_.x + 3;
	CHAR dir = -1;
	INT cell_rest = object_rect_.left % game_option_manager->GameCellSize;
	LONG cnt = 0;


	if (x_fos_ > 0)
	{
		dir = 1;
		cell_rest = (game_option_manager->GameCellSize - cell_rest) % game_option_manager->GameCellSize;
	}

	INT distance = INT_MAX;
	if (object_pos().y < 0)
		return distance;

	if (isOnLand() == FALSE)
	{
		search_idx = object_pos_.x + 3 + dir;
		UINT tempDistance = cell_rest;
		while (search_idx < game_option_manager->HorizontalGridCount && search_idx > -1
			&& (*map_[object_pos_.y + object_width_height_.y][search_idx]).tile_state == TILE_NULL)
		{
			tempDistance += game_option_manager->GameCellSize;
			if (tempDistance > distance)
				break;
			search_idx += dir;
		}
		if (tempDistance < distance)
			distance = tempDistance;
	}

	for (size_t i = 0; i < object_width_height_.y; i++)
	{
		search_idx = object_pos_.x + 3 + dir;
		UINT tempDistance = cell_rest;
		while (search_idx < game_option_manager->HorizontalGridCount && search_idx > -1
			&& (*map_[object_pos_.y + i][search_idx]).tile_state == TILE_NULL)
		{
			tempDistance += game_option_manager->GameCellSize;
			if (tempDistance > distance)
				break;
			search_idx += dir;
		}
		if (tempDistance < distance)
			distance = tempDistance;
	}
	return distance * dir - dir;
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
		animation_change_frame_count_ = animation_idle_change_frame;
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
	case player_attacked:
	{
		animation_bitmap_ids_ = animation_attacked_bitmap_ids;
		animation_size_ = animation_idle_size;
		animation_change_frame_count_ = animation_idle_change_frame;
		break;
	}
	}
	object_bitmap_id_ = animation_bitmap_ids_[0];
	current_frame_ = 0;
}

void Player::set_collider_rect()
{
	collide_rect_.left = object_rect_.left + game_option_manager->GameCellSize * 1;
	collide_rect_.right = collide_rect_.left + game_option_manager->GameCellSize * 3;
	collide_rect_.top = object_rect().top + game_option_manager->GameCellSize;
	collide_rect_.bottom = object_rect().bottom;
}

Player::Player()
{
	map_ = new tile**;
}


Player::~Player()
{
}
