#include "stdafx.h"
#include "InGameScene.h"
#include "Player.h"

extern HINSTANCE hInst;
extern RECT rectViewMain;


InGameScene::InGameScene()
{
	InGameSceneBackHBit = NULL;
	TransparentRGB = RGB(70, 110, 10);
	shadeScreenColorBrush = CreateSolidBrush(RGB(180, 140, 240));
	shadeScreenTransparentBrush = CreateSolidBrush(TransparentRGB);

	POINT initPolygonPoints[4] = { {100, 100}, {200, 100}, {200, 200}, {100, 200} };
	//polygonVector.push_back(new CPolygon(initPolygonPoints, 4));
	
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
	HDC hMemBackDC = CreateCompatibleDC(InGameSceneHDC);
	HBITMAP oldBitmap = (HBITMAP)SelectObject(hMemBackDC, *InGameSceneBackHBit);
	BitBlt(InGameSceneHDC, 0, 0, InGameSceneBackBit.bmWidth, InGameSceneBackBit.bmHeight, hMemBackDC, 0, 0, SRCCOPY);
	SelectObject(hMemBackDC, oldBitmap);
	ReleaseDC(SceneHWnd,hMemBackDC);
}

void InGameScene::DrawShadeScreen()
{
	static HDC InGameShadeHDC = CreateCompatibleDC(InGameSceneHDC);
	static HBITMAP ShadeScreen = CreateCompatibleBitmap(InGameSceneHDC, InGameSceneBackBit.bmWidth, InGameSceneBackBit.bmHeight);

	HBITMAP oldBitmap = (HBITMAP)SelectObject(InGameShadeHDC, ShadeScreen);
	HBRUSH oldBrush = (HBRUSH)SelectObject(InGameShadeHDC, shadeScreenColorBrush);

	Rectangle(InGameShadeHDC, 0, 0, InGameSceneBackBit.bmWidth, InGameSceneBackBit.bmHeight);

	DrawTransparentPoly(InGameShadeHDC);	
	//BitBlt(InGameSceneHDC, 0, 0, InGameSceneBackBit.bmWidth, InGameSceneBackBit.bmHeight, hMemShadeDC, 0, 0, SRCCOPY);
	TransparentBlt(InGameSceneHDC, 0, 0, InGameSceneBackBit.bmWidth, InGameSceneBackBit.bmHeight, InGameShadeHDC, 0, 0, InGameSceneBackBit.bmWidth, InGameSceneBackBit.bmHeight, TransparentRGB);


	if (player->playerFootprint.points.size() > 0)
	{
		size_t footPrintSize = player->playerFootprint.points.size();
		MoveToEx(InGameSceneHDC, player->playerFootprint.points.front().x, player->playerFootprint.points.front().y, NULL);
		for (size_t i = 1; i < footPrintSize; i++)
		{
			LineTo(InGameSceneHDC, player->playerFootprint.points[i].x, player->playerFootprint.points[i].y);
		}
		MoveToEx(InGameSceneHDC, player->playerFootprint.points.back().x, player->playerFootprint.points.back().y, NULL);
		LineTo(InGameSceneHDC, player->playerPos.x, player->playerPos.y);
	}

	player->PlayerDraw(InGameSceneHDC);
	//SelectObject(InGameShadeHDC, oldBitmap);
	//SelectObject(InGameShadeHDC, oldBrush);
}

void InGameScene::DrawTransparentPoly(const HDC hMemShadeDC)
{
	HBRUSH oldBrush = (HBRUSH)SelectObject(hMemShadeDC, shadeScreenTransparentBrush);

	/*for (CPolygon * cPoly : polygonVector)
	{
		Polygon(hMemShadeDC, cPoly->points, cPoly->vertexCount);
	}*/
	Polygon(hMemShadeDC, &(transperentCPoly->points[0]), transperentCPoly->points.size());
	//Polyline(hMemShadeDC, transperentCPoly->points, transperentCPoly->vertexCount);
	//SelectObject(hMemShadeDC, oldBrush);
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
		InGameSceneHDC = BeginPaint(SceneHWnd, &InGameScenePaintStruct);
		DrawBackGroundBit();
		DrawShadeScreen();
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
			player->PlayerMove(wParam, &transperentCPoly, InGameShadeHDC);
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
		InvalidateRect(this->SceneHWnd, NULL, false);
		SetFocus(this->SceneHWnd);
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

int CPolygon::isInLine(POINT & p)
{
	size_t vertexCount = this->points.size();
	for (int i = 0; i < vertexCount; i++) {
		int j = (i + 1) % vertexCount;
		
		LONG left, right;
		LONG up, down;

		if (points[i].x < points[j].x)
		{
			left = points[i].x;
			right = points[j].x;
		}
		else
		{
			left = points[j].x;
			right = points[i].x;
		}

		if (points[i].y < points[j].y)
		{
			up = points[i].y;
			down = points[j].y;
		}
		else
		{
			up = points[j].y;
			down = points[i].y;
		}

		if (p.x >= left && p.x <= right && p.y >= up && p.y <= down)
			return i;

	}
	return -1;
}

bool CPolygon::isInPoly(POINT & p)
{
	HRGN hrgn = CreatePolygonRgn(&(this->points[0]), points.size() , WINDING);
	return PtInRegion(hrgn,p.x,p.y);
}
