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

	virtual void SceneInit() = 0;
	virtual void SceneUpdate() = 0;
	virtual void SceneRender() = 0;
	virtual void SceneFree() = 0;
	//virtual LRESULT CALLBACK SceneWndProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam) = 0;
};

