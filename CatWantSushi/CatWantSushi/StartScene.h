#pragma once
#include "Scene.h"

class UI;

enum start_scene_ui_id
{
	BTN_START = 0, BTN_QUIT
};

class StartScene :
	public Scene
{
	void DrawBackGround(HDC hdc, const LPRECT paint_rect);
	void DrawUI(HDC hdc, const LPRECT paint_rect);
protected:
	void CreateUI() override;
public:
	StartScene();
	~StartScene() override;
	
	void init() override;
	void update() override;
	void render(HDC hdc, const LPRECT paint_rect) override;
	void free() override;

	void mouse_left_up(POINT & pt) override;
	void mouse_move(POINT & pt) override;
	
	static void StartBtnClick();
	static void EndBtnClick();
};

