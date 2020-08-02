#pragma once
#include "Scene.h"
#include <vector>
class Player;

enum class PointInPolygon
{
	inLine = 0, inPoly, outOfPoly
};

class InGameScene :
	public Scene
{
public:
	HDC InGameSceneHDC;
	HDC InGameShadeHDC;
	PAINTSTRUCT InGameScenePaintStruct;
	HBRUSH shadeScreenColorBrush;
	HBRUSH shadeScreenTransparentBrush;
	HBITMAP * InGameSceneBackHBit;
	BITMAP InGameSceneBackBit;
	COLORREF TransparentRGB;
	Player * player;
	std::vector<CPolygon *> polygonVector;
	CPolygon * transperentCPoly;
	//std::vector<HRGN *> hRgnVector;
	//static HRGN transparentRgn;

	InGameScene();
	~InGameScene();

	void CreateSceneWindow(HWND hWndMain, RECT &rectViewMain);
	void RegisterSceneClass(WNDCLASSEX* wcex);
	void SceneInit();

	void DrawBackGroundBit();
	void DrawShadeScreen();
	void DrawTransparentPoly(const HDC hMemShadeDC);
	void ClearPolygonVector();

	LRESULT CALLBACK InGameSceneWndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam);
	static LRESULT CALLBACK staticInGameSceneWndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam);
};

