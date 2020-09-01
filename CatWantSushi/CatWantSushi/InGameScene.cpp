#include "InGameScene.h"
#include <Windows.h>
#include "resource.h"
#include "GameOptionManager.h"
#include "GlobalValue.h"
#include "UI.h"
#include "AnimateObject.h"
#include "Player.h"

extern SceneRenderer scene_renderer_;

void InGameScene::CreateUI()
{

}

void InGameScene::CreateObject()
{
	GameOptionManager *game_option_manager = GameOptionManager::GetInstance();
	//size_t cellSize = game_option_manager->GameHeight / game_option_manager->VerticalGridCount;
	LONG yellowCatLeft = 0;
	LONG yellowCatTop = 0;
	//RECT startBtnRect = { startBtnLeft, startBtnTop, startBtnLeft + cellSize * 4, startBtnTop + cellSize * 2 };
	Object * yellowCat = new Object(IMAGE_YELLOW_CAT, BITMAP_INGAME_SCENE_UI_CAT_YELLOW, yellowCatLeft, yellowCatTop, 5, 6);
	scene_objects_.push_back(yellowCat);

	LONG blueCatLeft = game_option_manager->HorizontalGridCount - 5;
	LONG blueCatTop = 0;
	Object * blueCat = new Object(IMAGE_BLUE_CAT, BITMAP_INGAME_SCENE_UI_CAT_BLUE, blueCatLeft, blueCatTop, 5, 6, dir_left);
	scene_objects_.push_back(blueCat);

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
	
	LONG playerLeft = 0;
	LONG playerTop = basePlatFormTop - 7;
	player1 = new Player(PLAYER_BLUE, BITMAP_CAT_BLUE_IDLE, playerLeft, playerTop, 7, 7, FALSE, 1, 0, 0, nullptr);
	scene_objects_.push_back(player1);
}

InGameScene::InGameScene()
{
	CreateUI();
	CreateObject();
}


InGameScene::~InGameScene()
{ 
}

void InGameScene::init()
{
}

void InGameScene::update()
{
	for(Object * object : scene_objects_)
	{
		object->update();
	}
}

void InGameScene::render(HDC hdc, const LPRECT paint_rect)
{
	scene_renderer_.DrawSceneBackground(hdc, paint_rect, BITMAP_INGAME_SCENE_BACKGROUND);
	scene_renderer_.DrawSceneUI(hdc, paint_rect, scene_uis_);
	scene_renderer_.DrawSceneObeject(hdc, paint_rect, scene_objects_);
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
		player1->PlayerMove(dir_left);
	}
	else if(key == game_option_manager->shortCutKeyList[player1_move_right])
	{
		player1->PlayerMove(dir_right);
	}
	else if(key == game_option_manager->shortCutKeyList[player1_jump])
	{
		player1->PlayerJump();
	}
}
