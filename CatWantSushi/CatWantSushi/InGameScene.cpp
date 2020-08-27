#include "InGameScene.h"
#include <Windows.h>
#include "resource.h"
#include "GameOptionManager.h"
#include "GlobalValue.h"
#include "UI.h"

extern SceneRenderer scene_renderer_;

void InGameScene::CreateUI()
{
}

InGameScene::InGameScene()
{
}


InGameScene::~InGameScene()
{ 
}

void InGameScene::init()
{
}

void InGameScene::update()
{
}

void InGameScene::render(HDC hdc, const LPRECT paint_rect)
{
	scene_renderer_.DrawSceneBackground(hdc, paint_rect, BITMAP_INGAME_SCENE_BACKGROUND);
}

void InGameScene::free()
{
}
