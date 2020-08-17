#include "stdafx.h"
#include "Monster.h"


Monster::Monster()
{
	pos.x = 20;
	pos.y = 20;
	xSpd = 3;
	ySpd = 4;
	size = 10;
}


void Monster::Move(CPolygon & transparentPoly)
{
	POINT tempPos = { pos.x + xSpd, pos.y + ySpd };
	
	if (BoundaryCheck(tempPos) == false)
	{
		if (tempPos.x < 8 || tempPos.x > WINDOW_WIDTH - 8)
			xSpd *= -1;
		else
			ySpd *= -1;
		return;
	}
	else if (transparentPoly.getDistanceWithPolygon(tempPos) < size)
	{
		if (xSpd < 0)
			tempPos.x -= size;
		else
			tempPos.x += size;
		if (transparentPoly.isInPoly(tempPos) == true)
			xSpd *= -1;
		else
			ySpd *= -1;
		return;
	}
	pos = tempPos;
}

Monster::~Monster()
{
}
