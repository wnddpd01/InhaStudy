#pragma once
class Scene
{
public:
	HWND SceneHWnd;

	Scene()
	{
	}

	virtual ~Scene()
	{
	}

	virtual void RegisterSceneClass(WNDCLASSEX* wcex) = 0;
	virtual void CreateSceneWindow(HWND hWndMain, RECT& rectViewMain) = 0;
	//virtual LRESULT CALLBACK SceneWndProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam) = 0;
};

