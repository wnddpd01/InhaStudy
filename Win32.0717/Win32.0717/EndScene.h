#pragma once
#include "Scene.h"
#include <vector>

typedef struct PlayerRank
{
	USHORT rank;
	wchar_t playerName[20];
	ULONG score;
	std::wstring toString()
	{
		wchar_t wchar[32];
		wsprintf(wchar, L"%-3d. %-10s %4d", rank, playerName, score);
		std::wstring ret(wchar);
		return ret;
	}
};

class CEndScene :
	public CScene
{
public:
	RECT gameOverRect;
	RECT rankRect[6];
	RECT gameReplayRect;
	RECT gameQuitRect;
	FILE * rankFile;
	PlayerRank * lastPlayerRank;
	std::vector<PlayerRank *> ranks;
	CEndScene();
	~CEndScene();

	void Init(void);
	void Update(UINT message, WPARAM wParam, LPARAM lParam);
	void Render(HDC hdc);
	void Free(void);

	void PlayerRankDraw(HDC& hdc, RECT& rect);
};

