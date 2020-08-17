#pragma once
#include <vector>
class CPolygon;

class Player
{
public:
	POINT playerPos;
	POINT playerDestPos;
	WPARAM prevMoveDir;
	WPARAM lastMoveDir;
	bool drawMode;
	CPolygon playerFootprint;
	Player();
	virtual ~Player();

	WPARAM PlayerMove(WPARAM dir, CPolygon ** transparentPoly);
	void MoveToLine(POINT &tempPoint);
	void MoveOutOfPoly(POINT & tempPoint);
	bool NeedDrawNewPoly();
	void footprintOrderSort();
	bool isInFootPrint(POINT& p);
	RECT getPlayerRect();
	void setPlayerPos(POINT pos);
	void setPlayerDestPos(WPARAM dir);
	double getDistanceFromFootprint(POINT & p);
};
