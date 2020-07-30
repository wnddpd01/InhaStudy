#pragma once
#include "Scene.h"

class CStartScene :
	public CScene
{
public:
	bool enterSelected;
	RECT titleRect;
	RECT idMenuRect;
	RECT idRect;
	RECT enterRect;
	CStartScene();
	~CStartScene();

	void Init(void);
	void Update(UINT message, WPARAM wParam, LPARAM lParam);
	void Render(HDC hdc);
	void Free(void);
};

