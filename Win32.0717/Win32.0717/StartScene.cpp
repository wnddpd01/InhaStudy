#include "stdafx.h"
#include "StartScene.h"

extern Singleton * singleton;

CStartScene::CStartScene()
{
	titleRect = { 100, 100 , 500, 200 };
	idMenuRect = { 50, 500, 150, 580 };
	idRect = { idMenuRect.right, idMenuRect.top, idMenuRect.right + 300, idMenuRect.bottom };
	enterRect = { idMenuRect.right + 300, idRect.top, idRect.right + 100, idRect.bottom };
}


CStartScene::~CStartScene()
{
}


void CStartScene::Init()
{
}

void CStartScene::Update(UINT message, WPARAM wParam, LPARAM lParam)
{
	if (message == WM_CHAR)
	{
		if (wParam == VK_BACK)
		{
			if (singleton->name.size() > 0)
				singleton->name.pop_back();
		}
		else if (wParam == VK_RETURN)
		{
			if (singleton->name.size() > 0)
				singleton->sceneManager.SceneChange(SceneState::ingame);
		}
		else if (wParam == VK_SPACE)
		{

		}
		else if (iswalpha(wParam) || iswalnum(wParam))
		{
			if (singleton->name.size() < 8)
				singleton->name += wParam;
		}
	}
}

void CStartScene::Render(HDC hdc)
{
	HFONT oldFont = (HFONT)SelectObject(hdc, singleton->titleFont);
	DrawText(hdc, singleton->title, _tcslen(singleton->title), &titleRect, DT_TOP | DT_CENTER | DT_SINGLELINE);

	Rectangle(hdc, idMenuRect.left, idMenuRect.top, idMenuRect.right, idMenuRect.bottom);
	(HFONT)SelectObject(hdc, singleton->idMenuFont);
	DrawText(hdc, L"ID", 2, &idMenuRect, DT_SINGLELINE | DT_CENTER | DT_VCENTER);

	Rectangle(hdc, idRect.left, idRect.top, idRect.right, idRect.bottom);
	(HFONT)SelectObject(hdc, singleton->idFont);
	DrawText(hdc, singleton->name.c_str(), singleton->name.size(), &idRect, DT_SINGLELINE | DT_CENTER |DT_VCENTER);

	if (singleton->name.size() == 0)
		SetTextColor(hdc, RGB(212, 212, 212));
	Rectangle(hdc, enterRect.left, enterRect.top, enterRect.right, enterRect.bottom);
	(HFONT)SelectObject(hdc, singleton->idMenuFont);
	DrawText(hdc, L"Ȯ��", 2, &enterRect, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
	SelectObject(hdc, oldFont);
}

void CStartScene::Free(void)
{
}
