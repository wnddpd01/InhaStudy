#include "stdafx.h"
#include "GameManager.h"

extern HINSTANCE hInst;

GameManager::GameManager()
{
	inGameScene = new InGameScene;
	bitMapGangjunCharacter = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_));
}

void GameManager::RegisterInGameSceneWindow()
{
	wcex->lpfnWndProc = inGameScene->SceneWndProc;
	wcex->lpszMenuName = NULL;
	wcex->lpszClassName = L"InGameSceneClass";
	RegisterClassExW(wcex);
}

void GameManager::CreateIngameSceneWindow()
{
	inGameScene->SceneHWnd = CreateWindow(L"InGameSceneClass", L"InGameSceneWindow", NULL, 0, 0, rectViewMain.right, rectViewMain.bottom, *hWndMain, NULL, hInst, NULL);
}



void GameManager::RegisterSceneWindow(WNDCLASSEXW * wcex)
{
	this->wcex = wcex;
	RegisterInGameSceneWindow();
}

void GameManager::CreateSceneWindow(HWND & hWndMain)
{
	this->hWndMain = &hWndMain;
	GetClientRect(hWndMain, &(this->rectViewMain));
	CreateIngameSceneWindow();
}


GameManager::~GameManager()
{
}
