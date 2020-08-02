#include "stdafx.h"
#include "GameManager.h"
#include "InGameScene.h"
#include "resource.h"

extern HINSTANCE hInst;

GameManager::GameManager()
{
	inGameScene = new InGameScene;
	curScene = inGameScene;
}

void GameManager::RegisterSceneWindow(WNDCLASSEXW * wcex)
{
	inGameScene->RegisterSceneClass(wcex);
}

void GameManager::CreateSceneWindow(HWND & hWndMain)
{
	this->hWndMain = &hWndMain;
	GetClientRect(hWndMain, &(this->rectViewMain)); 
	inGameScene->CreateSceneWindow(hWndMain, rectViewMain);
}



GameManager::~GameManager()
{
}
