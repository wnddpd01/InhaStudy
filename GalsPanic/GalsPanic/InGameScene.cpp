#include "stdafx.h"
#include "InGameScene.h"

extern HINSTANCE hInst;
extern RECT rectViewMain;
InGameScene::InGameScene()
{
	InGameSceneBackHBit = NULL;
}


InGameScene::~InGameScene()
{
}

void InGameScene::CreateSceneWindow(HWND hWndMain, RECT& rectViewMain)
{
	InGameSceneBackHBit = &BitMapManager::getInstance()->bitMapGangjinCharacter;
	GetObject(*InGameSceneBackHBit, sizeof(BITMAP), &InGameSceneBackBit);
	this->SceneHWnd = CreateWindow(L"InGameSceneClass", L"InGameSceneWindow", WS_CHILD | WS_VISIBLE, 0, 0, rectViewMain.right, rectViewMain.bottom, hWndMain, NULL, hInst, this);
}

void InGameScene::RegisterSceneClass(WNDCLASSEX* wcex)
{
	wcex->lpfnWndProc = InGameScene::staticInGameSceneWndProc;
	wcex->lpszMenuName = NULL;
	wcex->lpszClassName = L"InGameSceneClass";
	RegisterClassExW(wcex);
}


LRESULT CALLBACK InGameScene::InGameSceneWndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	switch (iMsg)
	{
	case WM_NCCREATE :
	{
		return TRUE;
	}
	break;
	case WM_PAINT:
	{
		InGameSceneHDC = BeginPaint(SceneHWnd, &InGameScenePaintStruct);
		HDC hMemDC = CreateCompatibleDC(InGameSceneHDC);
		HBITMAP oldBitmap = (HBITMAP)SelectObject(hMemDC, *InGameSceneBackHBit);
		StretchBlt(InGameSceneHDC, 0, 0, 1920, 1080, hMemDC, 0, 0, InGameSceneBackBit.bmWidth, InGameSceneBackBit.bmHeight, SRCCOPY);
		//BitBlt(InGameSceneHDC, 50, 50, InGameSceneBackBit.bmWidth, InGameSceneBackBit.bmHeight, hMemDC, 0, 0, SRCCOPY);
		EndPaint(SceneHWnd, &InGameScenePaintStruct);
	}
	break;
	}
	return 0;
}

LRESULT CALLBACK InGameScene::staticInGameSceneWndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	InGameScene* pThis;
	if (iMsg == WM_NCCREATE)
	{
		pThis = static_cast<InGameScene*>(reinterpret_cast<LPCREATESTRUCT>(lParam)->lpCreateParams);
		SetWindowLongPtr(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pThis));
	}
	else
	{
		pThis = reinterpret_cast<InGameScene*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
	}
	if (pThis)
	{
		return pThis->InGameSceneWndProc(hwnd, iMsg, wParam, lParam);
	}
	return DefWindowProc(hwnd, iMsg, wParam, lParam);
}



