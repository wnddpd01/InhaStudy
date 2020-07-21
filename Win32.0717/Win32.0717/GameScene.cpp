#include "stdafx.h"
#include "GameScene.h"

extern Singleton * singleton;
CObject * CGameScene::healthObjects[6];
std::list<CObject *> CGameScene::bulletObjects;
std::list<Enemy *> CGameScene::enemyObjects;
std::stack<CObject *> CGameScene::deadBulletPool;
std::stack<Enemy *> CGameScene::deadEnemyPool;
size_t CGameScene::leftHealth;
ULONG CGameScene::time;
ULONG CGameScene::lastEnemyCreateTime;
size_t CGameScene::diffuculty;

USHORT GetRectZone(int circleX, int circleY, int rectX, int rectY)
{
	int xZone = (circleX < rectX - 50) ? 0 :
		(circleX > rectX + 50) ? 2 : 1;
	int yZone = (circleY < rectY - 50) ? 0 :
		(circleY > rectY + 50) ? 2 : 1;
	return xZone + 3 * yZone;
}

bool CollisionCircleRect(CObject * bullet, Enemy * enemy)
{
	bool collisionDectected = false;
	USHORT nZone = GetRectZone(bullet->x, bullet->y, enemy->x, enemy->y);
	float halfWidth = (enemy->points[1].x - enemy->points[0].x) * 0.5;
	float halfHeight = (enemy->points[2].y - enemy->points[1].y) * 0.5;

	switch(nZone)
	{
	case 1 :
	case 7 :
	{
		float distY = fabs(bullet->y - enemy->y);
		if (distY <= (10 + halfHeight))
			collisionDectected = true;
	}
	break;
	case 3 :
	case 5 :
	{
		float distX = fabs(bullet->x - enemy->x);
		if (distX <= (10 + halfWidth))
			collisionDectected = true;
	}
	break;
	case 4 :
		collisionDectected = true;
		break;
	default :
	{
		float connerX = (nZone == 0 || nZone == 6) ? enemy->x - halfWidth : enemy->x + halfWidth;
		float connerY = (nZone == 0 || nZone == 2) ? enemy->y - halfHeight : enemy->y + halfHeight;
		if (pow(connerX - bullet->x, 2) + pow(connerY - bullet->y, 2) <= 100)
			collisionDectected = true;
	}
	break;
	}
	return collisionDectected;
}

CGameScene::CGameScene()
{
	singleton = Singleton::GetInstance();
	nameRect = { 0, 0, 225, 50 };
	timeRect = { nameRect.right, 0, nameRect.right + 150, 50 };
	scoreRect = { timeRect.right, 0, timeRect.right + 225, 50 };

	circleXY.x = 300;
	circleXY.y = 760;
	cannon = new Cannon;
	cannon->objType = ObjectType::Cannon;
	cannon->spdX = cannon->spdY = 0;
	cannon->x = 300;
	cannon->y = 660;
	cannon->points = new POINT[4];

	unsigned short leftX;
	unsigned short rightX;
	unsigned short upY;
	unsigned short downY;

	leftX = 300 - 15;
	rightX = 300 + 15;
	upY = 690 - 30;
	downY = 690 + 30;

	cannon->cPoints[0].x = cannon->cPoints[3].x = cannon->points[0].x = cannon->points[3].x = leftX;
	cannon->cPoints[1].x = cannon->cPoints[2].x = cannon->points[1].x = cannon->points[2].x = rightX;
	cannon->cPoints[0].y = cannon->cPoints[1].y = cannon->points[0].y = cannon->points[1].y = upY;
	cannon->cPoints[2].y = cannon->cPoints[3].y = cannon->points[2].y = cannon->points[3].y = downY;
	cannon->curAngle = 0;
	for (size_t i = 0; i < 6; i++)
	{
		CObject* healthBox = new CObject;
		healthBox->objType = ObjectType::Health;
		healthBox->spdX = healthBox->spdY = 0;
		healthBox->x = 50 + i * 100;
		healthBox->y = 810;
		healthBox->points = new POINT[4];

		leftX = healthBox->x - 50;
		rightX = healthBox->x + 50;
		upY = healthBox->y - 50;
		downY = healthBox->y + 50;

		healthBox->points[0].x = healthBox->points[3].x = leftX;
		healthBox->points[1].x = healthBox->points[2].x = rightX;
		healthBox->points[0].y = healthBox->points[1].y = upY;
		healthBox->points[2].y = healthBox->points[3].y = downY;
		healthObjects[i] = healthBox;
	}

	for (size_t i = 0; i < 100; i++)
	{
		CObject* bullet = new CObject;
		bullet->objType = ObjectType::Bullet;
		bullet->isDead = true;
		deadBulletPool.push(bullet);
		Enemy * enemy = new Enemy;
		enemy->objType = ObjectType::Enemy;
		enemy->isDead = true;
		enemy->points = new POINT[4];
		deadEnemyPool.push(enemy);
	}
}


CGameScene::~CGameScene()
{
}

void CGameScene::Init(void)
{
	SetTimer(singleton->hWnd, TimerID::updateGameTimer, 1000 / 30, this->BulletUpdate);
	SetTimer(singleton->hWnd, TimerID::timeTimer, 100, this->TimeUpdate);
	SetTimer(singleton->hWnd, TimerID::enemyCreateTimer, 1000 / 30, this->EnemyCreate);
	lastEnemyCreateTime = 0;
	time = 0;
	diffuculty = 6;
	leftHealth = 6;
	for (size_t i = 0; i < 6; i++)
	{
		healthObjects[i]->isDead = false;
	}
}

void CGameScene::Update(UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_CHAR :
	{
		if (wParam == 97 || wParam == VK_LEFT)
		{
			CannonRotate(-5);
		}
		else if (wParam == 100 || wParam == VK_RIGHT)
		{
			CannonRotate(5);
		}
		else if (wParam == VK_SPACE)
		{
			CObject* bullet;
			if (deadBulletPool.size() == 0)
			{
				bullet = new CObject;
				bullet->objType = ObjectType::Bullet;
				bullet->points = nullptr;
			}
			else
			{
				bullet = deadBulletPool.top();
				deadBulletPool.pop();
				bullet->isDead = false;
			}
			bullet->x = (cannon->points[1].x - cannon->points[0].x) * 0.5 + cannon->points[0].x;
			bullet->y = (cannon->points[1].y - cannon->points[0].y) * 0.5 + cannon->points[0].y;
			bullet->spdX = (cannon->points[0].x - cannon->points[3].x) * 0.15;
			bullet->spdY = (cannon->points[0].y - cannon->points[3].y) * 0.15;
			bulletObjects.push_back(bullet);
		}
	}
	break;
	}
}

void CGameScene::Render(HDC hdc)
{
	Ellipse(hdc, circleXY.x - 60, circleXY.y - 60, circleXY.x + 60, circleXY.y + 60);

	Polygon(hdc, cannon->points, 4);
	for (size_t i = 0; i < 6; i++)
	{
		if(healthObjects[i]->isDead == false)
			Polygon(hdc, healthObjects[i]->points, 4);
	}
	for (CObject* object : bulletObjects)
	{
		Ellipse(hdc, object->x - 10, object->y - 10, object->x + 10, object->y + 10);
	}
	for (CObject * object : enemyObjects)
	{
		Polygon(hdc, object->points, 4);
	}

	HFONT oldFont = (HFONT)SelectObject(hdc, singleton->idSmallFont);
	Rectangle(hdc, nameRect.left, nameRect.top, nameRect.right, nameRect.bottom);
	DrawText(hdc, (L"ID : " + singleton->name).c_str(), singleton->name.size() + 5, &nameRect, DT_SINGLELINE | DT_CENTER | DT_VCENTER);

	Rectangle(hdc, timeRect.left, timeRect.top, timeRect.right, timeRect.bottom);
	size_t min = time / (60 * 1000);
	size_t sec = time % (60 * 1000) * 0.001;
	std::wstring wsTime = (min > 9 ? std::to_wstring(min) : L"0" + std::to_wstring(min)) + L":" + (sec > 9 ? std::to_wstring(sec) : + L"0" + std::to_wstring(sec));
	DrawText(hdc, wsTime.c_str(), wsTime.size(), &timeRect, DT_SINGLELINE | DT_CENTER | DT_VCENTER);

	Rectangle(hdc, scoreRect.left, scoreRect.top, scoreRect.right, scoreRect.bottom);
	DrawText(hdc, std::to_wstring(singleton->score).c_str(), 8, &scoreRect, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
	SelectObject(hdc, oldFont);
}

void CGameScene::Free(void)
{
	while (!KillTimer(singleton->hWnd, TimerID::updateGameTimer));
	while (!KillTimer(singleton->hWnd, TimerID::enemyCreateTimer));
	while (!KillTimer(singleton->hWnd, TimerID::timeTimer));

	while (enemyObjects.size() > 0)
	{
		Enemy * enemy = enemyObjects.back();
		enemyObjects.pop_back();
		enemy->isDead = true;
		deadEnemyPool.push(enemy);
	}
	while (bulletObjects.size() > 0)
	{
		CObject * bullet = bulletObjects.back();
		bulletObjects.pop_back();
		bullet->isDead = true;
		deadBulletPool.push(bullet);
	}
}

void CGameScene::BulletUpdate(HWND hWnd, UINT uMsg, UINT_PTR timerID, DWORD dwTime)
{
	for (auto it = bulletObjects.begin(); it != bulletObjects.end();)
	{
		(*it)->x += (*it)->spdX;
		(*it)->y += (*it)->spdY;
		if ((*it)->y < 40 || (*it)->x < -10 || (*it)->x > 610)
		{
			(*it)->isDead = true;
			deadBulletPool.push(*it);
			it = bulletObjects.erase(it);
		}
		else
		{
			bool collision = false;
			for (auto enemyIt = enemyObjects.begin(); enemyIt != enemyObjects.end();)
			{
				if ((collision = CollisionCircleRect(*it, *enemyIt)))
				{
					(*it)->isDead = true;
					deadBulletPool.push(*it);
					it = bulletObjects.erase(it);
					deadEnemyPool.push(*enemyIt);
					enemyObjects.erase(enemyIt);
					singleton->score += 50;
					break;
				}
				else
					enemyIt++;
			}
			if(collision == false)
				it++;
		}
	}
	for (auto it = enemyObjects.begin(); it != enemyObjects.end() && leftHealth > 0; )
	{
		(*it)->y += (*it)->spdY;
		for (size_t i = 0; i < 4; i++)
		{
			(*it)->points[i].y += (*it)->spdY;
		}
		if ((*it)->points[2].y > 910)
		{
			(*it)->isDead = true;
			deadEnemyPool.push(*it);
			it = enemyObjects.erase(it);
		}
		else if ((*it)->points[2].y > 760)
		{
			if (healthObjects[(*it)->num]->isDead == false)
			{
				healthObjects[(*it)->num]->isDead = true;
				(*it)->isDead = true;
				deadEnemyPool.push(*it);
				it = enemyObjects.erase(it);
				leftHealth--;
				if (leftHealth == 0)
				{
					singleton->sceneManager->SceneChange(SceneState::end);
					return;
				}
			}
			else
			{
				it++;
			}
		}
		else
			it++;
	}
}

void CGameScene::TimeUpdate(HWND, UINT, UINT_PTR, DWORD)
{
	time+= 100;
	singleton->score++;
	if (time % 10000 == 0 && diffuculty < 6)
		diffuculty++;
}

void CGameScene::EnemyCreate(HWND, UINT, UINT_PTR, DWORD)
{
	if (time - lastEnemyCreateTime > 3000 / diffuculty)
	{
		lastEnemyCreateTime = time;
		bool check[6] = { false , false, false, false, false, false};
		for (size_t i = 0; i < diffuculty; i++)
		{
			size_t num = rand() % 6;
			while (check[num] == true)
				num = rand() % 6;
			Enemy * enemy;
			if (deadEnemyPool.size() == 0)
			{
				enemy = new Enemy;
				enemy->objType = ObjectType::Enemy;
				enemy->points = new POINT[4];
			}
			else
			{
				enemy = deadEnemyPool.top();
				deadEnemyPool.pop();
			}
			check[num] = true;
			enemy->num = num;
			enemy->x = 50 + num * 100;
			enemy->y = 0;
			enemy->points[0].x = enemy->points[3].x = enemy->x - 50;
			enemy->points[1].x = enemy->points[2].x = enemy->x + 50;
			enemy->points[0].y = enemy->points[1].y = enemy->y - 50;
			enemy->points[2].y = enemy->points[3].y = enemy->y + 50;
			enemy->spdY = 10;
			enemy->spdX = 0;
			enemy->isDead = false;
			enemyObjects.push_back(enemy);
		}
	}
}

void CGameScene::CannonRotate(SHORT angle)
{
	if (abs(cannon->curAngle + angle) > 78)
		return;
	cannon->curAngle += angle;
	SHORT _angle = cannon->curAngle;
	if (_angle < 0)
		_angle += 360;
	else if (_angle > 359)
		_angle %= 360;
	double _sin = singleton->sinArray[_angle];
	double _cos = singleton->cosArray[_angle];
	double xTmp = 0, yTmp = 0;
	for (size_t i = 0; i < 4; i++)
	{
		xTmp = _cos * double(cannon->cPoints[i].x - circleXY.x) - _sin * double(cannon->cPoints[i].y - circleXY.y);
		yTmp = _sin * double(cannon->cPoints[i].x - circleXY.x) + _cos * double(cannon->cPoints[i].y - circleXY.y);
		cannon->points[i].x = xTmp + circleXY.x;
		cannon->points[i].y = yTmp + circleXY.y;
	}
}
