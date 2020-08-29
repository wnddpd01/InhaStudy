#pragma once
#include "Scene.h"

enum ingame_scene_ui_id
{
};

enum ingame_object_id
{

	IMAGE_YELLOW_CAT = 0, IMAGE_BLUE_CAT, OBJECT_PLATFORM, ANIMATED_OBJECT_RAIL
};

enum tile_state : UCHAR
{
	TILE_NULL = 0, TILE_PLATFORM
};

class InGameScene :
	public Scene
{
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
};

