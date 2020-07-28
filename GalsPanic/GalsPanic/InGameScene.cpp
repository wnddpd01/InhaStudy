#include "stdafx.h"
#include "InGameScene.h"


InGameScene::InGameScene()
{
}


InGameScene::~InGameScene()
{
}

void InGameScene::SceneInit()
{
}

void InGameScene::SceneUpdate()
{
}

void InGameScene::SceneRender()
{
}

void InGameScene::SceneFree()
{
}

LRESULT InGameScene::SceneWndProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	switch (iMsg)
	{
	case WM_PAINT :
		{

		}
		break;
	default:
		break;
	}
	return LRESULT();
}
