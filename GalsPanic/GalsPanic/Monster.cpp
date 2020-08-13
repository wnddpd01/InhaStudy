#include "stdafx.h"
#include "Monster.h"


Monster::Monster()
{
	pos.x = 0;
	pos.y = 0;
	xSpd = 6;
	ySpd = 8;
	size = 4;
}


void Monster::Move()
{
	pos.x += xSpd;
	pos.y += ySpd;
}

Monster::~Monster()
{
}
