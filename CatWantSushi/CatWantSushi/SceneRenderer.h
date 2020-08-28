#pragma once
#include <Windows.h>
#include <vector>
class UI;
class Object;
class SceneRenderer
{
public:
	SceneRenderer();
	~SceneRenderer();

	void DrawSceneBackground(HDC hdc, const LPRECT paint_rect, UINT bitmap_id);
	void DrawSceneUI(HDC hdc, const LPRECT paint_rect, std::vector<UI*> scene_uis);
	void DrawSceneObeject(HDC hdc, const LPRECT paint_rect, std::vector<Object *> scene_objects);
	void DrawGrid(HDC hdc, const LPRECT paint_rect);
};

