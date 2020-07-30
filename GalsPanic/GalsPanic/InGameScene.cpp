#include "stdafx.h"
#include "InGameScene.h"

extern HINSTANCE hInst;
extern RECT rectViewMain;

InGameScene::InGameScene()
{
	InGameSceneBackHBit = NULL;
	TransparentRGB = RGB(70, 110, 10);
	shadeScreenColorBrush = CreateSolidBrush(RGB(180, 140, 240));
	shadeScreenTransparentBrush = CreateSolidBrush(TransparentRGB);
}


InGameScene::~InGameScene()
{
}

void InGameScene::CreateSceneWindow(HWND hWndMain, RECT& rectViewMain)
{
	InGameSceneBackHBit = &BitMapManager::getInstance()->bitMapMountain;
	GetObject(*InGameSceneBackHBit, sizeof(BITMAP), &InGameSceneBackBit);
	this->SceneHWnd = CreateWindowW(L"InGameSceneClass", L"InGameSceneWindow", WS_CHILD | WS_VISIBLE | WS_OVERLAPPED, rectViewMain.right * 0.5 - InGameSceneBackBit.bmWidth * 0.5, 0, InGameSceneBackBit.bmWidth, InGameSceneBackBit.bmHeight, hWndMain, NULL, hInst, this);
}

void InGameScene::RegisterSceneClass(WNDCLASSEX* wcex)
{
	wcex->lpfnWndProc = InGameScene::staticInGameSceneWndProc;
	wcex->lpszMenuName = NULL;
	wcex->lpszClassName = L"InGameSceneClass";
	RegisterClassExW(wcex);
}

void InGameScene::DrawBackGroundBit()
{
	HDC hMemBackDC = CreateCompatibleDC(InGameSceneHDC);
	HBITMAP oldBitmap = (HBITMAP)SelectObject(hMemBackDC, *InGameSceneBackHBit);
	BitBlt(InGameSceneHDC, 0, 0, InGameSceneBackBit.bmWidth, InGameSceneBackBit.bmHeight, hMemBackDC, 0, 0, SRCCOPY);
	SelectObject(hMemBackDC, oldBitmap);
	ReleaseDC(SceneHWnd,hMemBackDC);
}

void InGameScene::DrawShadeScreen()
{
	HDC hMemShadeDC = CreateCompatibleDC(InGameSceneHDC);
	static HBITMAP ShadeScreen = CreateCompatibleBitmap(InGameSceneHDC, InGameSceneBackBit.bmWidth, InGameSceneBackBit.bmHeight);

	HBITMAP oldBitmap = (HBITMAP)SelectObject(hMemShadeDC, ShadeScreen);
	HBRUSH oldBrush = (HBRUSH)SelectObject(hMemShadeDC, shadeScreenColorBrush);

	Rectangle(hMemShadeDC, 0, 0, InGameSceneBackBit.bmWidth, InGameSceneBackBit.bmHeight);

	DrawTransparentPoly(hMemShadeDC);

	//BitBlt(InGameSceneHDC, 0, 0, InGameSceneBackBit.bmWidth, InGameSceneBackBit.bmHeight, hMemShadeDC, 0, 0, SRCCOPY);
	TransparentBlt(InGameSceneHDC, 0, 0, InGameSceneBackBit.bmWidth, InGameSceneBackBit.bmHeight, hMemShadeDC, 0, 0, InGameSceneBackBit.bmWidth, InGameSceneBackBit.bmHeight, TransparentRGB);

	SelectObject(hMemShadeDC, oldBitmap);
	SelectObject(hMemShadeDC, oldBrush);
	ReleaseDC(SceneHWnd, hMemShadeDC);
}

void InGameScene::DrawTransparentPoly(const HDC hMemShadeDC)
{
	HBRUSH oldBrush = (HBRUSH)SelectObject(hMemShadeDC, shadeScreenTransparentBrush);
	SelectObject(hMemShadeDC, oldBrush);
}

void InGameScene::ClearPolygonVector()
{
	while(polygonVector.size() > 1)
	{ }
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
		DrawBackGroundBit();
		DrawShadeScreen();
		EndPaint(SceneHWnd, &InGameScenePaintStruct);
	}
	break;
	case WM_CHAR :
	{
	}
	break;
	case WM_LBUTTONDOWN :
	{
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



