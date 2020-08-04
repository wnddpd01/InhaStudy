#pragma once
#include <vector>
class CPolygon;

class Player
{
public:
	POINT playerPos;
	CPolygon playerFootprint;
	Player();
	~Player();

	void PlayerMove(WPARAM moveDir, CPolygon ** transparentPoly);
	void footprintOrderSort(ULONG *startLineNum, ULONG *endLineNum);
	bool isInFootPrint(POINT& p);
	RECT getPlayerRect();
	void setPlayerPos(POINT pos);
};
