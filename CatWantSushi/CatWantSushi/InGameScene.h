#pragma once
#include "Scene.h"

class Player;

enum ingame_scene_ui_id
{
	IMAGE_YELLOW_CAT = 0, IMAGE_BLUE_CAT
};

enum ingame_object_id
{

	OBJECT_PLATFORM = 0, ANIMATED_OBJECT_RAIL, PLAYER_BLUE
};

enum tile_state : UCHAR
{
	TILE_NULL = 0, TILE_PLATFORM
};

class InGameScene :
	public Scene
{
	Player* player1;
protected:
	void CreateUI() override;
	void CreateObject() override;
public:
	InGameScene();
	~InGameScene();
	void init() override;
	void update() override;
	void render(HDC hdc, const LPRECT paint_rect) override;
	void free() override;

	void keyInputHandle(UCHAR key) override;
};

