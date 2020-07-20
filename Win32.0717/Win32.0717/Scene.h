#pragma once
class CScene
{
public:
	CScene()
	{
	}

	virtual ~CScene()
	{
	}

	virtual void Init(void) = 0;
	virtual void Update(UINT message, WPARAM wParam, LPARAM lParam) = 0;
	virtual void Render(HDC hdc) = 0;
	virtual void Free(void) = 0;
};

