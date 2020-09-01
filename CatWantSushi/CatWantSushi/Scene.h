#pragma once
#include <vector>
#include <Windows.h>
#include "SceneRenderer.h"
#pragma comment (lib, "Msimg32.lib")

class Object;
class UI;
class Scene
{
protected:
	std::vector<Object *> scene_objects_;
	std::vector<UI *> scene_uis_;
	static SceneRenderer scene_renderer_;
	virtual void CreateUI(){}
	virtual void CreateObject(){}
public:
	Scene() = default;

	virtual void init() = 0;
	virtual void update() = 0;
	virtual void render(HDC hdc, const LPRECT paint_rect) = 0;
	virtual void free() = 0;

	virtual void keyInputHandle(UCHAR key) {}
	virtual void mouse_left_up(POINT & pt){}
	virtual void mouse_move(POINT & pt){}
	
	virtual ~Scene() = default;
};

