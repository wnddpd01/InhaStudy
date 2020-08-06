#pragma once
#include <vector>
class CPolygon;

class Player
{
public:
	POINT playerPos;
	WPARAM prevMoveDir;
	WPARAM lastMoveDir;
	ULONG startLineNum;
	ULONG endLineNum;
	bool drawMode;
	CPolygon playerFootprint;
	Player();
	~Player();

	WPARAM PlayerMove(WPARAM moveDir, CPolygon ** transparentPoly);
	void MoveToLine(int &lineNum,POINT &tempPoint);
	bool NeedDrawNewPoly();
	void MoveOutOfPoly(POINT & tempPoint);
	void footprintOrderSort();
	bool isInFootPrint(POINT& p);
	RECT getPlayerRect();
	void setPlayerPos(POINT pos);
};
