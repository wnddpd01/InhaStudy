#pragma once
#include "Scene.h"
#include <vector>

#define EVENT_NO_EVENT 0
#define EVENT_DRAW_NEW_TR_POLYGON 1
#define EVENT_PLAYER_CANT_MOVE 101

class Player;

enum class PointInPolygon
{
	inLine = 0, inPoly, outOfPoly
};

typedef enum TimerName
{
	
};

class InGameScene :
	public Scene
{
public:
	HDC InGameSceneFrontHDC;
	HDC InGameSceneBackHDC; 
	PAINTSTRUCT InGameScenePaintStruct;
	HBRUSH shadeScreenColorBrush;
	HBRUSH shadeScreenTransparentBrush;
	HBRUSH playerBrush;
	HBITMAP * InGameSceneBackHBit;
	BITMAP InGameSceneBackBit;
	COLORREF TransparentRGB;
	Player * player;
	CPolygon * transperentCPoly;
	bool needDrawBackGroud = true;
	//std::vector<HRGN *> hRgnVector;
	//static HRGN transparentRgn;

	InGameScene();
	virtual ~InGameScene();

	void CreateSceneWindow(HWND hWndMain, RECT &rectViewMain);
	void RegisterSceneClass(WNDCLASSEX* wcex);
	void SceneInit();

	void DrawBackGroundBit();
	void DrawShadeScreen();
	void DrawFootprint(HDC InGameSceneShadeHDC);
	void DrawTransparentPoly(const HDC hMemShadeDC);
	void DrawPlayer();
	void DrawUI();

	LRESULT CALLBACK InGameSceneWndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam);
	static LRESULT CALLBACK staticInGameSceneWndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam);
};

