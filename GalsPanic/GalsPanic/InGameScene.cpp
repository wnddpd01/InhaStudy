#include "stdafx.h"
#include "InGameScene.h"
#include "Player.h"
#include "Monster.h"
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
	InGameSceneBackHDC = NULL;
	POINT initPolygonPoints[4] = { {96, 96 }, {192, 96}, {192, 192}, {96, 192} };
	transperentCPoly = new CPolygon(initPolygonPoints, 4);
	player = new Player;
	player->setPlayerPos({ 96, 96 });
	player->playerDestPos = player->playerPos;

	aliveMonster.push_back(new Monster);

}



InGameScene::~InGameScene()
{
}

void InGameScene::CreateSceneWindow(HWND hWndMain, RECT& rectViewMain)
{
	InGameSceneBackHBit = &BitMapManager::getInstance()->bitMapForMee;
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
	DrawFootprint(InGameSceneShadeHDC);
	DrawTransparentPoly(InGameSceneShadeHDC);
	//BitBlt(InGameSceneHDC, 0, 0, InGameSceneBackBit.bmWidth, InGameSceneBackBit.bmHeight, hMemShadeDC, 0, 0, SRCCOPY);
	TransparentBlt(InGameSceneBackHDC, 0, 0, InGameSceneBackBit.bmWidth, InGameSceneBackBit.bmHeight, InGameSceneShadeHDC, 0, 0, InGameSceneBackBit.bmWidth, InGameSceneBackBit.bmHeight, TransparentRGB);

	DeleteObject(ShadeScreen);
	DeleteDC(InGameSceneShadeHDC);
	//SelectObject(InGameShadeHDC, oldBitmap);
	//SelectObject(InGameShadeHDC, oldBrush);
}

void InGameScene::DrawFootprint(HDC InGameSceneShadeHDC)
{
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
	static char drawBigger = 1;
	static char drawCount = 0;
	static char addSize = 1;
	static char playerSize = 6;
	Rectangle(InGameSceneBackHDC, player->playerPos.x - (playerSize + (addSize * drawBigger)), player->playerPos.y - (playerSize + (addSize * drawBigger)), player->playerPos.x + (playerSize + (addSize * drawBigger)), player->playerPos.y + (playerSize + (addSize * drawBigger)));
	drawCount++;
	if (drawCount == 8)
	{
		drawCount = 0;
		drawBigger *= -1;
	}
	SelectObject(InGameSceneBackHDC, oldBrush);
}

void InGameScene::DrawEnemy()
{
	for (Monster * monster : aliveMonster)
	{
		Ellipse(InGameSceneBackHDC, monster->pos.x - monster->size, monster->pos.y - monster->size, monster->pos.x + monster->size, monster->pos.y + monster->size);
	}
}

void InGameScene::DrawUI()
{

}


LRESULT CALLBACK InGameScene::InGameSceneWndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	static WPARAM dir = NULL;
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
		SetTimer(hwnd, TimerID_SceneDraw, 1000 / 75, NULL);
		SetTimer(hwnd, TimerID_SceneUpdate, 1000 / 60, NULL);	
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
		DrawEnemy();
		DrawPlayer();
		DrawUI();
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
		switch (wParam)
		{
		case TimerID_SceneDraw :
			InvalidateRect(this->SceneHWnd, NULL, true);
			break;
		case TimerID_SceneUpdate:
			setDir(&dir);
			if ((player->PlayerMove(dir, &transperentCPoly)) == EVENT_DRAW_NEW_TR_POLYGON)
			{
				transperentCPoly->MergePolygon(player->playerFootprint.points);
				player->playerFootprint.points.clear();
			}
			dir = NULL;
			for (Monster * monster : aliveMonster)
			{
				monster->Move(*transperentCPoly);
				if (player->getDistanceFromFootprint(monster->pos) < monster->size)
				{
					delete transperentCPoly;
					POINT initPolygonPoints[4] = { {96, 96 }, {192, 96}, {192, 192}, {96, 192} };
					transperentCPoly = new CPolygon(initPolygonPoints, 4);


					delete player;
					player = new Player;
					player->setPlayerPos({ 96, 96 });
					player->playerDestPos = player->playerPos;

					delete aliveMonster.back();
					aliveMonster.pop_back();
					aliveMonster.push_back(new Monster);
				}
			}
			break;
		}
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
