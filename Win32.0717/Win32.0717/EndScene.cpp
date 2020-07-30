#include "stdafx.h"
#include "EndScene.h"
#include <algorithm>
extern Singleton * singleton;

bool compare(PlayerRank * p1, PlayerRank * p2)
{
	return p1->score > p2->score;
}

CEndScene::CEndScene()
{
	gameOverRect = { 100, 100, 500, 200 };
	for (LONG i = 0; i < 6; i++)
	{
		rankRect[i] = { 150, 250 + i * 50, 450, 300 + i * 50 };
	}
	gameReplayRect = { 100, 650, 250, 750 };
	gameQuitRect = { 350, 650, 500, 750 };
}

CEndScene::~CEndScene()
{
}

void CEndScene::Init(void)
{
	if (!(rankFile = fopen("rankFile.txt", "r+")))
		rankFile = fopen("rankFile.txt", "w+");
	PlayerRank * playerRank = new PlayerRank;
	while (fread(playerRank, sizeof(PlayerRank), 1, rankFile))
	{
		ranks.push_back(playerRank);
		playerRank = new PlayerRank;
	}
	fseek(rankFile, 0, SEEK_SET);
	this->lastPlayerRank = playerRank;
	playerRank->rank = 0;
	lstrcpyW(playerRank->playerName, singleton->name.c_str());
	playerRank->score = singleton->score;
	ranks.push_back(playerRank);
	std::sort(ranks.begin(), ranks.end(), compare);
	USHORT rank = 1;
	for (PlayerRank * pr : ranks)
	{
		pr->rank = rank++;
		fwrite(pr, sizeof(PlayerRank), 1, rankFile);
	}
	selectedMenu = EndMenu::null;
	fclose(rankFile);
}

void CEndScene::Update(UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_LBUTTONDOWN:
	{
		int mouseX = LOWORD(lParam);
		int mouseY = HIWORD(lParam);
		if (mouseX > gameReplayRect.left && mouseX < gameReplayRect.right && mouseY > gameReplayRect.top && mouseY < gameReplayRect.bottom)
		{
			singleton->sceneManager->SceneChange(SceneState::start);
		}
		else if (mouseX > gameQuitRect.left && mouseX < gameQuitRect.right && mouseY > gameQuitRect.top && mouseY < gameQuitRect.bottom)
		{
			singleton->sceneManager->SceneChange(SceneState::quit);
		}
	}
	break;
	case WM_MOUSEMOVE :
	{
		int mouseX = LOWORD(lParam);
		int mouseY = HIWORD(lParam);
		if (mouseX > gameReplayRect.left && mouseX < gameReplayRect.right && mouseY > gameReplayRect.top && mouseY < gameReplayRect.bottom)
		{
			selectedMenu = EndMenu::replay;
		}
		else if (mouseX > gameQuitRect.left && mouseX < gameQuitRect.right && mouseY > gameQuitRect.top && mouseY < gameQuitRect.bottom)
		{
			selectedMenu = EndMenu::quit;
		}
		else
		{
			selectedMenu = EndMenu::null;
		}
	}
	break;
	case WM_KEYDOWN :
	{
		switch (wParam)
		{
		case VK_LEFT:
		case VK_RIGHT:
			if (selectedMenu == EndMenu::quit)
				selectedMenu = EndMenu::replay;
			else
				selectedMenu = EndMenu::quit;
			break;
		case VK_RETURN :
			if (selectedMenu == EndMenu::replay)
				singleton->sceneManager->SceneChange(SceneState::start);
			else if (selectedMenu == EndMenu::quit)
				singleton->sceneManager->SceneChange(SceneState::quit);
			break;
		default:
			break;
		}
	}
	break;
	}
}

void CEndScene::Render(HDC hdc)
{
	HFONT oldFont = (HFONT)SelectObject(hdc, singleton->titleFont);
	DrawText(hdc, L"강진 멸망", 5, &gameOverRect, DT_VCENTER | DT_CENTER | DT_SINGLELINE);

	SelectObject(hdc, singleton->idSmallFont);
	for (size_t i = 0; i < 5 && i < ranks.size(); i++)
	{
		if (ranks[i] == lastPlayerRank)
		{
			PlayerRankDraw(hdc, rankRect[i]);
			continue;
		}
		Rectangle(hdc, rankRect[i].left - 10, rankRect[i].top, rankRect[i].right + 10, rankRect[i].bottom);
		DrawText(hdc, ranks[i]->toString().c_str() , ranks[i]->toString().size(), &rankRect[i], DT_VCENTER | DT_LEFT | DT_SINGLELINE);
	}
	if (lastPlayerRank->rank > 5)
	{
		PlayerRankDraw(hdc, rankRect[5]);
	}

	Rectangle(hdc, gameReplayRect.left, gameReplayRect.top, gameReplayRect.right, gameReplayRect.bottom);
	DrawText(hdc, L"다시하기", 4, &gameReplayRect, DT_VCENTER | DT_CENTER | DT_SINGLELINE);
	Rectangle(hdc, gameQuitRect.left, gameQuitRect.top, gameQuitRect.right, gameQuitRect.bottom);
	DrawText(hdc, L"게임 끝내기", 6, &gameQuitRect, DT_VCENTER | DT_CENTER | DT_SINGLELINE);

	HBRUSH oldBrush = (HBRUSH)SelectObject(hdc, singleton->lightPinkBrush);
	if (selectedMenu == EndMenu::replay)
	{
		Rectangle(hdc, gameReplayRect.left, gameReplayRect.top, gameReplayRect.right, gameReplayRect.bottom);
		DrawText(hdc, L"다시하기", 4, &gameReplayRect, DT_VCENTER | DT_CENTER | DT_SINGLELINE);
	}
	else if (selectedMenu == EndMenu::quit)
	{
		Rectangle(hdc, gameQuitRect.left, gameQuitRect.top, gameQuitRect.right, gameQuitRect.bottom);
		DrawText(hdc, L"게임 끝내기", 6, &gameQuitRect, DT_VCENTER | DT_CENTER | DT_SINGLELINE);
	}
	SelectObject(hdc, oldBrush);
	SelectObject(hdc, oldFont);
}

void CEndScene::Free(void)
{
	for (PlayerRank * pr : ranks)
		delete pr;
	ranks.clear();
}

void CEndScene::PlayerRankDraw(HDC& hdc, RECT& rect)
{
	HBRUSH oldBrush = (HBRUSH)SelectObject(hdc, singleton->lightBlueBrush);
	Rectangle(hdc, rect.left - 10, rect.top, rect.right + 10, rect.bottom);
	DrawText(hdc, lastPlayerRank->toString().c_str(), 31, &rect, DT_VCENTER | DT_LEFT | DT_SINGLELINE);
	SelectObject(hdc, oldBrush);
}
