#include "stdafx.h"
#include "InGameScene.h"
#include "Player.h"
#include <string>
extern HINSTANCE hInst;
extern RECT rectViewMain;

InGameScene::InGameScene()
{
	InGameSceneBackHBit = NULL;
	TransparentRGB = RGB(70, 110, 10);
	shadeScreenColorBrush = CreateSolidBrush(RGB(180, 140, 240));
	shadeScreenTransparentBrush = CreateSolidBrush(TransparentRGB);
	playerBrush = CreateSolidBrush(RGB(255, 0, 0));
	POINT initPolygonPoints[4] = { {100, 100}, {200, 100}, {200, 200}, {100, 200} };
	//polygonVector.push_back(new CPolygon(initPolygonPoints, 4));
	InGameSceneBackHDC = NULL;
	transperentCPoly = new CPolygon(initPolygonPoints, 4);

	player = new Player;
	player->setPlayerPos({ 100, 100 });
}



InGameScene::~InGameScene()
{
}

void InGameScene::CreateSceneWindow(HWND hWndMain, RECT& rectViewMain)
{
	InGameSceneBackHBit = &BitMapManager::getInstance()->bitMapMountain;
	GetObject(*InGameSceneBackHBit, sizeof(BITMAP), &InGameSceneBackBit);
	this->SceneHWnd = CreateWindowW(L"InGameSceneClass", L"InGameSceneWindow", WS_CHILD | WS_VISIBLE | WS_OVERLAPPED, rectViewMain.right * 0.5 - InGameSceneBackBit.bmWidth * 0.5, 0, InGameSceneBackBit.bmWidth, InGameSceneBackBit.bmHeight, hWndMain, NULL, hInst, this);
	SetFocus(this->SceneHWnd);
}

void InGameScene::RegisterSceneClass(WNDCLASSEX* wcex)
{
	wcex->lpfnWndProc = InGameScene::staticInGameSceneWndProc;
	wcex->lpszMenuName = NULL;
	wcex->lpszClassName = L"InGameSceneClass";
	RegisterClassExW(wcex);
}

void InGameScene::SceneInit()
{
	ClearPolygonVector();
}

void InGameScene::DrawBackGroundBit()
{
	HDC backgroundHDC = CreateCompatibleDC(InGameSceneBackHDC);
	HBITMAP oldBitmap = (HBITMAP)SelectObject(backgroundHDC, *InGameSceneBackHBit);
	BitBlt(InGameSceneBackHDC, 0, 0, InGameSceneBackBit.bmWidth, InGameSceneBackBit.bmHeight, backgroundHDC, 0, 0, SRCCOPY);
	SelectObject(backgroundHDC, oldBitmap);
	DeleteDC(backgroundHDC);
}

void InGameScene::DrawShadeScreen()
{
	HDC InGameSceneShadeHDC = CreateCompatibleDC(InGameSceneBackHDC);
	HBITMAP ShadeScreen = CreateCompatibleBitmap(InGameSceneBackHDC, InGameSceneBackBit.bmWidth, InGameSceneBackBit.bmHeight);
	HBITMAP oldBitmap = (HBITMAP)SelectObject(InGameSceneShadeHDC, ShadeScreen);
	HBRUSH oldBrush = (HBRUSH)SelectObject(InGameSceneShadeHDC, shadeScreenColorBrush);

	Rectangle(InGameSceneShadeHDC, 0, 0, InGameSceneBackBit.bmWidth, InGameSceneBackBit.bmHeight);
	if (player->playerFootprint.points.size() > 0)
	{
		size_t footPrintSize = player->playerFootprint.points.size();
		MoveToEx(InGameSceneShadeHDC, player->playerFootprint.points.front().x, player->playerFootprint.points.front().y, NULL);
		for (size_t i = 1; i < footPrintSize; i++)
		{
			LineTo(InGameSceneShadeHDC, player->playerFootprint.points[i].x, player->playerFootprint.points[i].y);
		}
		MoveToEx(InGameSceneShadeHDC, player->playerFootprint.points.back().x, player->playerFootprint.points.back().y, NULL);
		LineTo(InGameSceneShadeHDC, player->playerPos.x, player->playerPos.y);
	}
	DrawTransparentPoly(InGameSceneShadeHDC);
	//BitBlt(InGameSceneHDC, 0, 0, InGameSceneBackBit.bmWidth, InGameSceneBackBit.bmHeight, hMemShadeDC, 0, 0, SRCCOPY);
	TransparentBlt(InGameSceneBackHDC, 0, 0, InGameSceneBackBit.bmWidth, InGameSceneBackBit.bmHeight, InGameSceneShadeHDC, 0, 0, InGameSceneBackBit.bmWidth, InGameSceneBackBit.bmHeight, TransparentRGB);

	DeleteObject(ShadeScreen);
	DeleteDC(InGameSceneShadeHDC);
	//SelectObject(InGameShadeHDC, oldBitmap);
	//SelectObject(InGameShadeHDC, oldBrush);
}

void InGameScene::DrawTransparentPoly(const HDC hMemShadeDC)
{
	HBRUSH oldBrush = (HBRUSH)SelectObject(hMemShadeDC, shadeScreenTransparentBrush);
	Polygon(hMemShadeDC, &(transperentCPoly->points[0]), transperentCPoly->points.size());

	/*int i = 0;
	HPEN hpen = CreatePen(PS_SOLID, 0, RGB(0, 0, 0));
	for(POINT cp : transperentCPoly->points)
	{
		RECT rect = { cp.x - 5, cp.y - 5, cp.x + 5, cp.y + 5};
		DrawText(hMemShadeDC, std::to_wstring(i).c_str(), 1 ,&rect, DT_CENTER | DT_VCENTER);
		i++;
	}*/
	SelectObject(hMemShadeDC, oldBrush);
}

void InGameScene::DrawPlayer()
{
	HBRUSH oldBrush = (HBRUSH)SelectObject(InGameSceneBackHDC, playerBrush);
	Ellipse(InGameSceneBackHDC, player->playerPos.x - 10, player->playerPos.y - 10, player->playerPos.x + 10, player->playerPos.y + 10);
	SelectObject(InGameSceneBackHDC, oldBrush);
}

void InGameScene::ClearPolygonVector()
{
	while(polygonVector.size() > 1)
	{ 
	}
}


LRESULT CALLBACK InGameScene::InGameSceneWndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	switch (iMsg)
	{
	case WM_NCCREATE :
	{
		needDrawBackGroud = true;
		return TRUE;
	}
	break;
	case WM_CREATE :
	{
		SetTimer(hwnd, 2, 1000 / 30, NULL);
	}
	break;
	case WM_PAINT:
	{
		InGameSceneFrontHDC = BeginPaint(SceneHWnd, &InGameScenePaintStruct);
		if (InGameSceneBackHDC == NULL)
		{
			InGameSceneBackHDC = CreateCompatibleDC(InGameSceneFrontHDC);
			HBITMAP backHdcBit = CreateCompatibleBitmap(InGameSceneFrontHDC, InGameSceneBackBit.bmWidth, InGameSceneBackBit.bmHeight);
			SelectObject(InGameSceneBackHDC, backHdcBit);
		}
		DrawBackGroundBit();
		DrawShadeScreen();
		DrawPlayer();
		BitBlt(InGameSceneFrontHDC, 0, 0, InGameSceneBackBit.bmWidth, InGameSceneBackBit.bmHeight, InGameSceneBackHDC, 0, 0, SRCCOPY);
		EndPaint(SceneHWnd, &InGameScenePaintStruct);
	}
	break;
	case WM_KEYDOWN :
	{
		switch (wParam)
		{
		case VK_LEFT :
		case VK_RIGHT :
		case VK_UP :
		case VK_DOWN :
			double prevArea = transperentCPoly->getArea();
			player->PlayerMove(wParam, &transperentCPoly);
			if (prevArea != transperentCPoly->getArea())
				needDrawBackGroud = true;
			break;
 		}
	}
	break;
	case WM_LBUTTONDOWN :
	{
	}
	break;
	case WM_TIMER :
	{
		InvalidateRect(this->SceneHWnd, NULL, true);
	}
	break;
	case WM_SETFOCUS :
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
