#pragma once
#include <list>
#include "Scene.h"

class AnimateObject;
class SushiGenerator;
class Player;

enum ingame_scene_ui_id
{
	IMAGE_YELLOW_CAT = 0, IMAGE_BLUE_CAT, TIMER_UI, COUNT_UI, PLAYER1_SCORE_UI, PLAYER2_SCORE_UI
};

enum ingame_object_id
{
	OBJECT_PLATFORM = 0, ANIMATED_OBJECT_RAIL, PLAYER_BLUE, PLAYER_YELLOW, OBJECT_SUSHI_EGG, OBJECT_SUSHI_SHRIMP, OBJECT_SUSHI_TUNA
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
	Player* player_blue_;
	Player* player_yellow_;
	UINT game_time_;
	std::list<Object*> sushi_list_;
	SushiGenerator* sushi_generator_;
	AnimateObject* rail;

	void sushi_collide_process();
	void player_collide_process();
	void (InGameScene::* inGameScene_update)();
	void update_game_ready();
	void update_game_start();
protected:
	void CreateUI() override;
	void CreateObject() override;
	void SetMap();
public:
	tile** map_;
	InGameScene();
	~InGameScene();
	void init() override;
	void update() override;
	void render(HDC hdc, const LPRECT paint_rect) override;
	void free() override;
	
	void keyInputHandle(UCHAR key) override;

};

inline UINT getRectArea(RECT & rect)
{
	return (rect.right - rect.left) * (rect.bottom - rect.top);
}