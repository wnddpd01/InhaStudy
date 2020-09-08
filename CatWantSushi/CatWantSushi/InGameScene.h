#pragma once
#include "Scene.h"

class Player;

enum ingame_scene_ui_id
{
	IMAGE_YELLOW_CAT = 0, IMAGE_BLUE_CAT, TIMER_UI, COUNT_UI
};

enum ingame_object_id
{
	OBJECT_PLATFORM = 0, ANIMATED_OBJECT_RAIL, PLAYER_BLUE, PLAYER_YELLOW
};

enum tile_state : UCHAR
{
	TILE_NULL = 0, TILE_PLATFORM
};

class InGameScene :
	public Scene
{
	Player* player_blue_;
	Player* player_yellow_;
	tile_state **map_;
	UINT game_time_;
	void (InGameScene::* inGameScene_update)();
	void update_game_ready();
	void update_game_start();
protected:
	void CreateUI() override;
	void CreateObject() override;
	void SetMap();
public:
	InGameScene();
	~InGameScene();
	void init() override;
	void update() override;
	void render(HDC hdc, const LPRECT paint_rect) override;
	void free() override;

	void keyInputHandle(UCHAR key) override;

};

