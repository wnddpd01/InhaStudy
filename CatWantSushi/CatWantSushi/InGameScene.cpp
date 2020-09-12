#include "InGameScene.h"
#include <Windows.h>
#include "resource.h"
#include "GameOptionManager.h"
#include "GlobalValue.h"
#include "UI.h"
#include "AnimateObject.h"
#include "Player.h"

extern SceneRenderer scene_renderer_;
extern GameOptionManager* game_option_manager;

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
	if(GetTickCount() - lastTime > 3000)
	{
		inGameScene_update = &InGameScene::update_game_start;
		delete scene_objects_.back();
		scene_objects_.pop_back();
		lastTime = 0;
	}
}

void InGameScene::update_game_start()
{
	for (Object* object : scene_objects_)
	{
		object->update();
	}
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
	timerUI->render = static_cast<void(Object::*)(HDC hdc, HDC backHDC)>(&UI::render_timerUI);
	timerUI->ui_data = &game_time_;
	scene_objects_.push_back(timerUI);
}

void InGameScene::CreateObject()
{
	LONG railLeft = 0;
	LONG railTop = game_option_manager->VerticalGridCount / 2 + 2;
	UCHAR * railAnimationBitmapIds = new UCHAR[2];
	railAnimationBitmapIds[0] = BITMAP_OBJECT_RAIL_1;
	railAnimationBitmapIds[1] = BITMAP_OBJECT_RAIL_2;
	AnimateObject * rail = new AnimateObject(ANIMATED_OBJECT_RAIL, BITMAP_OBJECT_RAIL_1, railLeft, railTop, game_option_manager->HorizontalGridCount, 7, TRUE ,2 , 0, game_option_manager->Frame / 6, railAnimationBitmapIds);
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
	
	LONG playerLeft = game_option_manager->HorizontalGridCount - 1 - 7;
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
	CreateUI();
	CreateObject();
	SetMap();
	player_blue_->LoadMap(map_);
	player_yellow_->LoadMap(map_);
	inGameScene_update = &InGameScene::update_game_ready;
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
	scene_renderer_.DrawGrid(hdc, paint_rect);
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
}

