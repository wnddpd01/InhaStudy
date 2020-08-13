#pragma once
class Monster
{
public:
	int xSpd;
	int ySpd;
	int size;
	POINT pos;
	Monster();

	void Move();
	~Monster();
};

