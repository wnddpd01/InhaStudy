#pragma once
#include <list>
#include <stack>
#include "Scene.h"

enum class ObjectType
{
	Health, Enemy, Bullet, Cannon
};

struct MyPoint
{
	float x;
	float y;
};

class CObject
{
public :
	float x, y;
	char spdX, spdY;
	bool isDead;
	size_t health;
	ObjectType objType;
	POINT *points;
	CObject()
	{
		isDead = false;
	}
};

class Cannon : public CObject
{
public :
	POINT cPoints[4];
	SHORT curAngle;
};

class CGameScene :
	public CScene
{
public:
	RECT nameRect;
	RECT timeRect;
	RECT scoreRect;
	POINT circleXY;
	Cannon* cannon;
	std::list<CObject *> objects;
	std::stack<CObject *> deadBulletPool;
	CGameScene();
	~CGameScene();

	void Init(void);
	void Update(UINT message, WPARAM wParam, LPARAM lParam);
	void Render(HDC hdc);
	void Free(void);

	void CannonRotate(SHORT angle);
};

