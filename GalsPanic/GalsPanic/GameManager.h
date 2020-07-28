#pragma once
class Scene;
class InGameScene;

class GameManager
{
	WNDCLASSEXW *wcex;
	HWND *hWndMain;
	RECT rectViewMain;

	GameManager();

	void RegisterInGameSceneWindow();
	void CreateIngameSceneWindow();
public:
	static GameManager * getInstance()
	{
		static GameManager ins;
		return &ins;
	}

	Scene * curScene;
	InGameScene * inGameScene;
	HBITMAP bitMapGangjunCharacter;

	void RegisterSceneWindow(WNDCLASSEXW *wcex);
	void CreateSceneWindow(HWND &hWndMain);
	~GameManager();
};

