#pragma once
#include <vector>
#include <Windows.h>
#include "SceneRenderer.h"
#pragma comment (lib, "Msimg32.lib")

class UI;

class Scene
{
protected:
	std::vector<UI *> scene_UI_;
	static SceneRenderer scene_renderer_;
	virtual void CreateUI() = 0;
public:
	Scene() = default;

	virtual void init() = 0;
	virtual void update() = 0;
	virtual void render(HDC hdc, const LPRECT paint_rect) = 0;
	virtual void free() = 0;

	virtual void mouse_left_up(POINT & pt){}
	virtual void mouse_move(POINT & pt){}
	
	virtual ~Scene() = default;
};

