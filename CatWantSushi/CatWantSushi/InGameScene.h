#pragma once
#include "Scene.h"

enum ingame_scene_ui_id
{
	
};

class InGameScene :
	public Scene
{
protected:
	void CreateUI() override;
public:
	InGameScene();
	~InGameScene();
	void init() override;
	void update() override;
	void render(HDC hdc, const LPRECT paint_rect) override;
	void free() override;
};

