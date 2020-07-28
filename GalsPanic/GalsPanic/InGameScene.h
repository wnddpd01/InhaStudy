#pragma once
#include "Scene.h"
class InGameScene :
	public Scene
{
public:
	HBITMAP * InGameBitmap;

	InGameScene();
	~InGameScene();

	void SceneInit();
	void SceneUpdate();
	void SceneRender();
	void SceneFree();
	static LRESULT CALLBACK SceneWndProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam);
};

