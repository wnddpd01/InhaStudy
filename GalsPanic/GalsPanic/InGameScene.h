#pragma once
#include "Scene.h"

class InGameScene :
	public Scene
{
public:
	HDC InGameSceneHDC;
	PAINTSTRUCT InGameScenePaintStruct;
	HBITMAP * InGameSceneBackHBit;
	BITMAP InGameSceneBackBit;

	InGameScene();
	~InGameScene();

	void CreateSceneWindow(HWND hWndMain, RECT &rectViewMain);
	void RegisterSceneClass(WNDCLASSEX* wcex);

	LRESULT CALLBACK InGameSceneWndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam);
	static LRESULT CALLBACK staticInGameSceneWndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam);
};

