#pragma once
#include "Scene.h"
#include <vector>

struct CPolygon
{
	size_t vertexCount;
	POINT *points;
};

class InGameScene :
	public Scene
{
public:
	HDC InGameSceneHDC;
	PAINTSTRUCT InGameScenePaintStruct;
	HBRUSH shadeScreenColorBrush;
	HBRUSH shadeScreenTransparentBrush;
	HBITMAP * InGameSceneBackHBit;
	BITMAP InGameSceneBackBit;
	COLORREF TransparentRGB;
	std::vector<CPolygon *> polygonVector;

	InGameScene();
	~InGameScene();

	void CreateSceneWindow(HWND hWndMain, RECT &rectViewMain);
	void RegisterSceneClass(WNDCLASSEX* wcex);
	void DrawBackGroundBit();
	void DrawShadeScreen();
	void DrawTransparentPoly(const HDC hMemShadeDC);
	void ClearPolygonVector();

	LRESULT CALLBACK InGameSceneWndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam);
	static LRESULT CALLBACK staticInGameSceneWndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam);
};

