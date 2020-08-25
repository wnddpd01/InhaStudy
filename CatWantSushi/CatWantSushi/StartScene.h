#pragma once
#include "Scene.h"

class UI;

class StartScene :
	public Scene
{
	void DrawBackGround(HDC hdc, const LPRECT paint_rect);
	void DrawUI(HDC hdc, const LPRECT paint_rect);
public:
	StartScene();
	~StartScene() override;
	
	void init() override;
	void update() override;
	void render(HDC hdc, const LPRECT paint_rect) override;
	void free() override;

	void StartBtnClick();
	void EndBtnClick();
};

