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
	float spdX, spdY;
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
	static ULONG time;
	static ULONG lastEnemyCreateTime;
	static size_t diffuculty;
	RECT nameRect;
	RECT timeRect;
	RECT scoreRect;
	POINT circleXY;
	Cannon* cannon;
	CObject * healthObjects[6];
	static std::list<CObject *> bulletObjects;
	static std::list<CObject *> enemyObjects;
	static std::stack<CObject *> deadBulletPool;
	static std::stack<CObject *> deadEnemyPool;
	CGameScene();
	~CGameScene();

	void Init(void);
	void Update(UINT message, WPARAM wParam, LPARAM lParam);
	void Render(HDC hdc);
	void Free(void);
	static void CALLBACK BulletUpdate(HWND, UINT, UINT_PTR, DWORD);
	static void CALLBACK TimeUpdate(HWND, UINT, UINT_PTR, DWORD);
	static void CALLBACK EnemyCreate(HWND, UINT, UINT_PTR, DWORD);
	void CannonRotate(SHORT angle);
};

