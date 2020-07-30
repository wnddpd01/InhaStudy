#pragma once

class Scene;
class InGameScene;

class GameManager
{
	WNDCLASSEXW *wcex;
	HWND *hWndMain;

	GameManager();
public:
	static GameManager * getInstance()
	{
		static GameManager ins;
		return &ins;
	}

	Scene * curScene;
	InGameScene * inGameScene;
	RECT rectViewMain;

	void RegisterSceneWindow(WNDCLASSEXW *wcex);
	void CreateSceneWindow(HWND &hWndMain);
	~GameManager();
};

