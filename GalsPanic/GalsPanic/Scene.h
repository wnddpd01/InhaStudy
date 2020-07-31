#pragma once
#pragma comment (lib, "Msimg32.lib")
class Scene
{
public:
	HWND SceneHWnd;
	RECT SceneWindowRect;
	Scene()
	{
	}

	virtual ~Scene()
	{
	}

	virtual void RegisterSceneClass(WNDCLASSEX* wcex) = 0;
	virtual void CreateSceneWindow(HWND hWndMain, RECT& rectViewMain) = 0;
	virtual void SceneInit() = 0;
	//virtual LRESULT CALLBACK SceneWndProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam) = 0;
};

