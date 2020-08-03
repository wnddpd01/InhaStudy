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
	bool isInFootPrint(POINT& p);
	RECT getPlayerRect();
	void setPlayerPos(POINT pos);
};

inline bool isEqualPoint(const POINT& p1, const POINT& p2)
{
	if (p1.x == p2.x && p1.y == p2.y)
		return true;
	return false;
}