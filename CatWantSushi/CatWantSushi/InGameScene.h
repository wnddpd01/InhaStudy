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

struct tile
{
	tile_state tile_state;
	Object* tile_object;
};

class InGameScene :
	public Scene
{
<<<<<<< HEAD
	Player* player_blue_;
	Player* player_yellow_;
	tile_state **map_;
	UINT game_time_;
	void (InGameScene::* inGameScene_update)();
	void update_game_ready();
	void update_game_start();
=======
	Player* player1;
	tile **map_;
>>>>>>> origin/master
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

