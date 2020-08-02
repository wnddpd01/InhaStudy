#pragma once
#include <vector>
class CPolygon;

class Player
{
public:
	POINT playerPos;
	HBRUSH playerBrush;
	CPolygon playerFootprint;
	Player();
	~Player();

	void PlayerDraw(const HDC hdc);
	void PlayerMove(WPARAM moveDir, CPolygon ** transparentPoly, const HDC inGameShadeHDC);
	RECT getPlayerRect();
	void setPlayerPos(POINT pos);
};

