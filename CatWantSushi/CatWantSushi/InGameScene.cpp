#include "InGameScene.h"
#include <Windows.h>
#include "resource.h"
#include "GameOptionManager.h"
#include "GlobalValue.h"
#include "UI.h"
#include "AnimateObject.h"
#include "Player.h"
#include "Sushi.h"
#include "SushiGenerator.h"

extern SceneRenderer scene_renderer_;
extern GameOptionManager* game_option_manager;

void InGameScene::sushi_collide_process()
{
	RECT player_yellow_collide_rect;
	RECT player_blue_collide_rect;
	UINT player_yellow_collide_rect_area;
	UINT player_blue_collide_rect_area;
	for(auto it = sushi_list_.begin(); it != sushi_list_.end();)
	{
		IntersectRect(&player_yellow_collide_rect, &player_yellow_->collide_rect_, &(*it)->object_rect());
		IntersectRect(&player_blue_collide_rect, &player_blue_->collide_rect_, &(*it)->object_rect());
		player_yellow_collide_rect_area = getRectArea(player_yellow_collide_rect);
		player_blue_collide_rect_area = getRectArea(player_blue_collide_rect);
		if (player_blue_collide_rect_area == player_yellow_collide_rect_area)
		{
			++it;
			continue;
		}
		else if(player_blue_collide_rect_area > player_yellow_collide_rect_area)
		{
			player_blue_->score += ((Sushi *)(*it))->sushi_score;
		}
		else
		{
			player_yellow_->score += ((Sushi*)(*it))->sushi_score;
		}
		delete (*it);
		it = sushi_list_.erase(it);
	}
}

void InGameScene::player_collide_process()
{
	RECT playerCollideRect;
	if(IntersectRect(&playerCollideRect, &player_blue_->collide_rect_, &player_yellow_->collide_rect_))
	{
		//FLOAT player_blue_xfos = player_blue_->x_fos_;
		LONG collide_width = playerCollideRect.right - playerCollideRect.left;
		player_blue_->x_fos_ = collide_width;
		player_yellow_->x_fos_ = collide_width;
		if(player_yellow_->object_rect().left < player_blue_->object_rect().left)
		{
			player_yellow_->x_fos_ *= -1;
		}
		else
		{
			player_blue_->x_fos_ *= -1;
		}
		player_blue_->x_fos_update();
		player_yellow_->x_fos_update();
	}
}

void InGameScene::update_game_ready()
{
	static UINT lastTime;
	if (lastTime == 0)
	{
		LONG cntLeft = game_option_manager->HorizontalGridCount / 2 - 3;
		LONG cntTop = game_option_manager->VerticalGridCount / 2 - 5;
		UCHAR* cntAnimationBitmapIds = new UCHAR[3];
		cntAnimationBitmapIds[0] = BITMAP_TEXT_3;
		cntAnimationBitmapIds[1] = BITMAP_TEXT_2;
		cntAnimationBitmapIds[2] = BITMAP_TEXT_1;
		AnimateObject* cnt = new AnimateObject(NULL, BITMAP_TEXT_3, cntLeft, cntTop, 6, 10, FALSE, 3, 0, game_option_manager->Frame, cntAnimationBitmapIds);
		scene_objects_.push_back(cnt);
		lastTime = GetTickCount();
	}
	if(GetTickCount() - lastTime > 2999)
	{
		inGameScene_update = &InGameScene::update_game_start;
		delete scene_objects_.back();
		scene_objects_.pop_back();
		lastTime = 0;
	}
}

void InGameScene::update_game_start()
{
	static UINT frame_count = 0;
	static UINT sushi_generation_time = 6;
	static UINT sushi_generation_time_change_time = 60;
	frame_count++;
	if(frame_count == game_option_manager->Frame)
	{
		if (game_time_ % sushi_generation_time == 0)
		{
			sushi_list_.push_back(sushi_generator_->sushi_generation());
		}
		game_time_--;
		frame_count = 0;
		if(game_time_ == sushi_generation_time_change_time)
		{
			rail->AnimationChangeFrameCount /= 2;
			sushi_generation_time /= 2;
			sushi_generation_time_change_time = 10;
		}
	}
	for (Object* object : scene_objects_)
	{
		object->update();
	}
	sushi_collide_process();
	player_collide_process();
}

void InGameScene::CreateUI()
{
	LONG yellowCatLeft = 0;
	LONG yellowCatTop = 0;
	UI* yellowCat = new UI(IMAGE_YELLOW_CAT, BITMAP_INGAME_SCENE_UI_CAT_YELLOW, yellowCatLeft, yellowCatTop, 4, 4, dir_right);
	scene_objects_.push_back(yellowCat);

	LONG blueCatLeft = game_option_manager->HorizontalGridCount - 5;
	LONG blueCatTop = 0;
	UI* blueCat = new UI(IMAGE_BLUE_CAT, BITMAP_INGAME_SCENE_UI_CAT_BLUE, blueCatLeft, blueCatTop, 4, 4, dir_left);
	scene_objects_.push_back(blueCat);

	LONG timerLeft = game_option_manager->HorizontalGridCount / 2 - 3;
	UI* timerUI = new UI(TIMER_UI, BITMAP_UI_TIMER, timerLeft, 0, 6, 3);
	timerUI->render = static_cast<void(Object::*)(HDC hdc, HDC backHDC)>(&UI::render_dataUI);
	timerUI->ui_data = &game_time_;
	scene_objects_.push_back(timerUI);

	LONG player2ScoreUILeft = yellowCat->ObjectPos.x + yellowCat->ObjectWidthHeight.x + 1;
	UI * player2ScoreUI = new UI(PLAYER2_SCORE_UI, BITMAP_UI_SCORE_PLAYER2,player2ScoreUILeft, 0, 12, 3);
	player2ScoreUI->render = static_cast<void(Object::*)(HDC hdc, HDC backHDC)>(&UI::render_dataUI);
	player2ScoreUI->data_draw_rect.left += 2 * game_option_manager->GameCellSize;
	player2ScoreUI->data_draw_rect.right = player2ScoreUI->data_draw_rect.left + 1 * game_option_manager->GameCellSize;
	player2ScoreUI->ui_data = &player_yellow_->score;
	scene_objects_.push_back(player2ScoreUI);

	LONG player1ScoreUILeft = blueCatLeft - 12 - 1;
	UI* player1ScoreUI = new UI(PLAYER1_SCORE_UI, BITMAP_UI_SCORE_PLAYER1, player1ScoreUILeft, 0, 12, 3);
	player1ScoreUI->render = static_cast<void(Object::*)(HDC hdc, HDC backHDC)>(&UI::render_dataUI);
	player1ScoreUI->data_draw_rect.right -= 2 * game_option_manager->GameCellSize;
	player1ScoreUI->data_draw_rect.left = player1ScoreUI->data_draw_rect.right - 1 * game_option_manager->GameCellSize;
	player1ScoreUI->ui_data = &player_blue_->score;
	scene_objects_.push_back(player1ScoreUI);
}

void InGameScene::CreateObject()
{
	LONG railLeft = 0;
	LONG railTop = game_option_manager->VerticalGridCount / 2 + 2;
	UCHAR * railAnimationBitmapIds = new UCHAR[2];
	railAnimationBitmapIds[0] = BITMAP_OBJECT_RAIL_1;
	railAnimationBitmapIds[1] = BITMAP_OBJECT_RAIL_2;
	rail = new AnimateObject(ANIMATED_OBJECT_RAIL, BITMAP_OBJECT_RAIL_1, railLeft, railTop, game_option_manager->HorizontalGridCount, 7, TRUE ,2 , 0, game_option_manager->Frame / 2, railAnimationBitmapIds);
	scene_objects_.push_back(rail);

	LONG basePlatFormLeft = 0;
	LONG basePlatFormTop = game_option_manager->VerticalGridCount - 3;
	Object * basePlatform = new Object(OBJECT_PLATFORM, BITMAP_OBJECT_PLATFORM, basePlatFormLeft, basePlatFormTop, game_option_manager->HorizontalGridCount, 3);
	scene_objects_.push_back(basePlatform);

	LONG leftPlatFormLeft = 7;
	LONG leftPlatFormTop = basePlatFormTop - 9 - 1;
	Object * leftPlatform = new Object(OBJECT_PLATFORM, BITMAP_OBJECT_PLATFORM, leftPlatFormLeft, leftPlatFormTop, 12, 1);
	scene_objects_.push_back(leftPlatform);

	LONG rightPlatFormleft = game_option_manager->HorizontalGridCount - 7 - 12;
	LONG rightPlatFormTop = leftPlatFormTop;
	Object * rightPlatform = new Object(OBJECT_PLATFORM, BITMAP_OBJECT_PLATFORM, rightPlatFormleft, rightPlatFormTop, 12, 1);
	scene_objects_.push_back(rightPlatform);
	
	LONG topPlatFormleft = game_option_manager->HorizontalGridCount / 2 - 6;
	LONG topPlatFormTop = leftPlatFormTop - 9 - 1;
	Object * topPlatform = new Object(OBJECT_PLATFORM, BITMAP_OBJECT_PLATFORM, topPlatFormleft, topPlatFormTop, 12, 1);
	scene_objects_.push_back(topPlatform);
	
	LONG playerLeft = game_option_manager->HorizontalGridCount - 7;
	LONG playerTop = basePlatFormTop - 7;
	player_blue_ = new Player(PLAYER_BLUE, playerLeft, playerTop);
	scene_objects_.push_back(player_blue_);

	playerLeft = 0;
	player_yellow_ = new Player(PLAYER_YELLOW, playerLeft, playerTop);
	scene_objects_.push_back(player_yellow_);
}

void InGameScene::SetMap()
{
	map_ = new tile*[game_option_manager->VerticalGridCount];
	for (size_t i = 0; i < game_option_manager->VerticalGridCount; i++)
	{
		map_[i] = new tile[game_option_manager->HorizontalGridCount];
		for (size_t j = 0; j < game_option_manager->HorizontalGridCount; j++)
		{
			map_[i][j].tile_state = TILE_NULL;
			map_[i][j].tile_object = nullptr;
		}
	}
	for(Object * object : scene_objects_)
	{
		if(object->ObjectId == OBJECT_PLATFORM)
		{
			for (size_t i = 0; i < object->ObjectWidthHeight.y; i++)
			{
				for (size_t j = 0; j < object->ObjectWidthHeight.x; j++)
				{
					map_[object->ObjectPos.y + i][object->ObjectPos.x + j].tile_state = TILE_PLATFORM;
					map_[object->ObjectPos.y + i][object->ObjectPos.x + j].tile_object = object;
				}
			}
		}
	}
	
}

InGameScene::InGameScene()
{
	init();
}


InGameScene::~InGameScene()
{ 
}

void InGameScene::init()
{
	this->game_time_ = 120;
	CreateObject();
	CreateUI();
	SetMap();
	player_blue_->LoadMap(map_);
	player_yellow_->LoadMap(map_);
	player_blue_->LoadEnemy(player_yellow_);
	player_yellow_->LoadEnemy(player_blue_);
	inGameScene_update = &InGameScene::update_game_ready;
	sushi_generator_ = new SushiGenerator();
}

void InGameScene::update()
{
	(this->*this->inGameScene_update)();
}

void InGameScene::render(HDC hdc, const LPRECT paint_rect)
{
	scene_renderer_.DrawSceneBackground(hdc, paint_rect, BITMAP_INGAME_SCENE_BACKGROUND);
	scene_renderer_.DrawSceneUI(hdc, paint_rect, scene_uis_);
	scene_renderer_.DrawSceneObeject(hdc, paint_rect, scene_objects_);
	scene_renderer_.DrawSceneObeject(hdc, paint_rect, sushi_list_);
	scene_renderer_.DrawSceneObeject(hdc, paint_rect, player_yellow_->player_object);
	scene_renderer_.DrawSceneObeject(hdc, paint_rect, player_blue_->player_object);
	//scene_renderer_.DrawGrid(hdc, paint_rect);
}

void InGameScene::free()
{
}

void InGameScene::keyInputHandle(UCHAR key)
{
	static auto * game_option_manager = GameOptionManager::GetInstance();
	if(key == game_option_manager->shortCutKeyList[player1_move_left])
	{
		player_blue_->PlayerMove(dir_left);
	}
	else if(key == game_option_manager->shortCutKeyList[player1_move_right])
	{
		player_blue_->PlayerMove(dir_right);
	}
	else if(key == game_option_manager->shortCutKeyList[player1_jump])
	{
		player_blue_->PlayerJumpKeyPressed();
	}
	else if(key == game_option_manager->shortCutKeyList[player1_attack])
	{
		player_blue_->PlayerAttack();
	}
	else if(key == game_option_manager->shortCutKeyList[player2_move_left])
	{
		player_yellow_->PlayerMove(dir_left);
	}
	else if (key == game_option_manager->shortCutKeyList[player2_move_right])
	{
		player_yellow_->PlayerMove(dir_right);
	}
	else if (key == game_option_manager->shortCutKeyList[player2_jump])
	{
		player_yellow_->PlayerJumpKeyPressed();
	}
	else if(key == game_option_manager->shortCutKeyList[player2_attack])
	{
		player_yellow_->PlayerAttack();
	}
}

