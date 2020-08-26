#pragma once
#include <vector>
#include <Windows.h>

#pragma comment (lib, "Msimg32.lib")
class UI;

class Scene
{
protected:
	std::vector<UI *> scene_UI_;
public:
	Scene() = default;

	virtual void init() = 0;
	virtual void update() = 0;
	virtual void render(HDC hdc, const LPRECT paint_rect) = 0;
	virtual void free() = 0;
	
	virtual ~Scene() = default;
};

