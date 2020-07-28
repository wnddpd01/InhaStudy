#include "stdafx.h"
#include "GameScene.h"

extern Singleton * singleton;
CObject * CGameScene::healthObjects[6];
std::list<CObject *> CGameScene::bulletObjects;
std::list<Enemy *> CGameScene::enemyObjects;
std::list<Enemy *> CGameScene::enemyEffectObjects;
std::stack<CObject *> CGameScene::deadBulletPool;
std::stack<Enemy *> CGameScene::deadEnemyPool;

POINT CGameScene::circleXY;
Cannon* CGameScene::cannon;
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
		healthBox->y = healthBox->spdY = 810;
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
	diffuculty = 1;
	leftHealth = 6;
	cannon->curAngle = 0;
	cannon->destAngle = 0;
	cannon->Rotate(0);

	unsigned short leftX;
	unsigned short rightX;
	unsigned short upY;
	unsigned short downY;

	for (size_t i = 0; i < 6; i++)
	{
		healthObjects[i]->isDead = false;
		healthObjects[i]->y = healthObjects[i]->spdY = 810;

		upY = healthObjects[i]->y - 50;
		downY = healthObjects[i]->y + 50;

		healthObjects[i]->points[0].y = healthObjects[i]->points[1].y = upY;
		healthObjects[i]->points[2].y = healthObjects[i]->points[3].y = downY;
	}
}

void CGameScene::Update(UINT message, WPARAM wParam, LPARAM lParam)
{
	static bool spaceUp = true;
	switch (message)
	{
	case WM_KEYDOWN :
	{
		if (wParam == VK_LEFT)
		{
			if (cannon->destAngle == cannon->curAngle || cannon->moveAngle == 2)
			{
				if(cannon->destAngle > -70)
					cannon->destAngle -= 20;
				cannon->moveAngle = -2;
			}
		}
		else if (wParam == VK_RIGHT)
		{
			if (cannon->destAngle == cannon->curAngle || cannon->moveAngle == -2)
			{
				if(cannon->destAngle < 70)
					cannon->destAngle += 20;
				cannon->moveAngle = 2;
			}
		}
		else if (wParam == VK_SPACE)
		{
			if (spaceUp == true)
			{
				spaceUp = false;
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
	}
	break;
	case WM_KEYUP : 
	{
		if (wParam == VK_SPACE)
			spaceUp = true;
	}
	break;
	}
}

void CGameScene::Render(HDC hdc)
{
	HBRUSH oldBrush = (HBRUSH)SelectObject(hdc, singleton->darkGreenBrush);
	Ellipse(hdc, circleXY.x - 60, circleXY.y - 60, circleXY.x + 60, circleXY.y + 60);

	Polygon(hdc, cannon->points, 4);

	SelectObject(hdc, singleton->BlueBrush);
	for (size_t i = 0; i < 6; i++)
	{
		if(healthObjects[i]->isDead == false)
			Polygon(hdc, healthObjects[i]->points, 4);
	}
	SelectObject(hdc, singleton->darkGreyBrush);
	for (CObject* object : bulletObjects)
	{
		Ellipse(hdc, object->x - 10, object->y - 10, object->x + 10, object->y + 10);
	}
	SelectObject(hdc, singleton->darkRedBrush);
	for (Enemy * object : enemyEffectObjects)
	{
		Polygon(hdc, object->drawPoint, 4);
	}
	for (CObject * object : enemyObjects)
	{
		Polygon(hdc, object->points, 4);
	}
	SelectObject(hdc, oldBrush);

	HFONT oldFont = (HFONT)SelectObject(hdc, singleton->idSmallFont);
	Rectangle(hdc, nameRect.left, nameRect.top, nameRect.right, nameRect.bottom);
	DrawText(hdc, (L"ID : " + singleton->name).c_str(), singleton->name.size() + 5, &nameRect, DT_SINGLELINE | DT_CENTER | DT_VCENTER);

	Rectangle(hdc, timeRect.left, timeRect.top, timeRect.right, timeRect.bottom);
	size_t min = time / (60 * 1000);
	size_t sec = time % (60 * 1000) * 0.001;
	std::wstring wsTime = (min > 9 ? std::to_wstring(min) : L"0" + std::to_wstring(min)) + L":" + (sec > 9 ? std::to_wstring(sec) : + L"0" + std::to_wstring(sec));
	DrawText(hdc, wsTime.c_str(), wsTime.size(), &timeRect, DT_SINGLELINE | DT_CENTER | DT_VCENTER);

	Rectangle(hdc, scoreRect.left, scoreRect.top, scoreRect.right, scoreRect.bottom);
	wchar_t score[10];
	wsprintf(score, L"%d", singleton->score);
	DrawText(hdc, _itow(singleton->score, score, 10), wcslen(score), &scoreRect, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
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
	for (size_t i = 0; i < 6; i++)
	{
		if (healthObjects[i]->isDead)
			continue;
		if (healthObjects[i]->y > 910)
		{
			healthObjects[i]->isDead = true;
			leftHealth--;
			if (leftHealth == 0)
			{
				singleton->sceneManager->SceneChange(SceneState::end);
				return;
			}
		}
		else if (healthObjects[i]->y != healthObjects[i]->spdY)
		{
			healthObjects[i]->y += 1;
			for (size_t j = 0; j < 4; j++)
			{
				healthObjects[i]->points[j].y += 1;
			}
		}
	}
	if (cannon->curAngle != cannon->destAngle)
		cannon->Rotate(cannon->moveAngle);

	for (auto it = enemyEffectObjects.begin(); it != enemyEffectObjects.end();)
	{
		double _sin = singleton->sinArray[int((*it)->spdY)];
		double _cos = singleton->cosArray[int((*it)->spdY)];
		(*it)->spdY += (*it)->spdX;
		if ((*it)->spdY < 0)
			(*it)->spdY += 360;
		else if ((*it)->spdY > 359)
			(*it)->spdY -= 360;
		(*it)->spdX *= 1.2;
		if ((*it)->points[0].x < (*it)->points[1].x)
		{
			double xTmp = 0, yTmp = 0; 
			for (size_t i = 0; i < 4; i++)
			{
				xTmp = _cos * double((*it)->points[i].x - (*it)->x) - _sin * double((*it)->points[i].y - (*it)->y);
				yTmp = _sin * double((*it)->points[i].x - (*it)->x) + _cos * double((*it)->points[i].y - (*it)->y); 

				((*it)->drawPoint)[i].x = xTmp + (*it)->x;
				((*it)->drawPoint)[i].y = yTmp + (*it)->y;
			}
			(*it)->points[3].x += 3;
			(*it)->points[3].y -= 3;
			(*it)->points[0].x += 3;
			(*it)->points[2].y -= 3;
			it++;
		}
		else
		{
			deadEnemyPool.push(*it);
			it = enemyEffectObjects.erase(it);
		}
	}

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

					Enemy * effect;
					Enemy * effect2;
					if (deadEnemyPool.size() > 0)
					{
						effect = deadEnemyPool.top(); deadEnemyPool.pop();
					}
					else
					{
						effect = new Enemy;
						effect->isDead = true;
						effect->objType = ObjectType::Enemy;
						effect->points = new POINT[4];
					}
					if (deadEnemyPool.size() > 0)
					{
						effect2 = deadEnemyPool.top(); deadEnemyPool.pop();
					}
					else
					{
						effect2 = new Enemy;
						effect2->isDead = true;
						effect2->objType = ObjectType::Enemy;
						effect2->points = new POINT[4];
					}
					effect2->x = effect->x = (*enemyIt)->x;
					effect2->y = effect->y = (*enemyIt)->y;
					size_t effectRand = rand() % 2;
					if (effectRand > 0)
					{
						(*enemyIt)->spdX = 10;
					}
					else
					{
						(*enemyIt)->spdX = -10;
					}
					effect->spdY = (*enemyIt)->spdY + 120;
					effect2->spdY = effect->spdY + 120;
					effect2->spdX = effect->spdX = (*enemyIt)->spdX;
					double _sin = singleton->sinArray[120];
					double _cos = singleton->cosArray[120];
					double xTmp;
					double yTmp;
					(*enemyIt)->points[0].x = (*enemyIt)->points[3].x = (*enemyIt)->x;
					(*enemyIt)->points[2].y = (*enemyIt)->points[3].y = (*enemyIt)->y;
					for (size_t i = 0; i < 4; i++)
					{
						effect2->points[i].x = effect->points[i].x = (*enemyIt)->drawPoint[i].x = (*enemyIt)->points[i].x;
						effect2->points[i].y = effect->points[i].y = (*enemyIt)->drawPoint[i].y = (*enemyIt)->points[i].y;

						xTmp = _cos * double((*enemyIt)->points[i].x - (*enemyIt)->x) - _sin * double((*enemyIt)->points[i].y - (*enemyIt)->y);
						yTmp = _sin * double((*enemyIt)->points[i].x - (*enemyIt)->x) + _cos * double((*enemyIt)->points[i].y - (*enemyIt)->y);
						effect->drawPoint[i].x = xTmp + (*enemyIt)->x;
						effect->drawPoint[i].y = yTmp + (*enemyIt)->y;

						xTmp = _cos * double(effect->points[i].x - (*enemyIt)->x) - _sin * double(effect->points[i].y - (*enemyIt)->y);
						yTmp = _sin * double(effect->points[i].x - (*enemyIt)->x) + _cos * double(effect->points[i].y - (*enemyIt)->y);
						effect2->drawPoint[i].x = xTmp + (*enemyIt)->x;
						effect2->drawPoint[i].y = yTmp + (*enemyIt)->y;
					}
					enemyEffectObjects.push_back(*enemyIt);
					enemyEffectObjects.push_back(effect);
					enemyEffectObjects.push_back(effect2);
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
		if ((*it)->points[2].y > 960)
		{
			(*it)->isDead = true;
			deadEnemyPool.push(*it);
			it = enemyObjects.erase(it);
		}
		
		else if ((*it)->points[2].y > healthObjects[(*it)->num]->points[0].y)
		{
			if (healthObjects[(*it)->num]->isDead == false)
			{
				healthObjects[(*it)->num]->spdY += 21;
				healthObjects[(*it)->num]->y += 5;
				for (size_t i = 0; i < 4; i++)
				{
					healthObjects[(*it)->num]->points[i].y += 5;
				}
				(*it)->isDead = true;
				deadEnemyPool.push(*it);
				it = enemyObjects.erase(it);
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

void Cannon::Rotate(SHORT angle)
{
	this->curAngle += angle;
	SHORT _angle = this->curAngle;
	if (_angle < 0)
		_angle += 360;
	else if (_angle > 359)
		_angle %= 360;
	double _sin = singleton->sinArray[_angle];
	double _cos = singleton->cosArray[_angle];
	double xTmp = 0, yTmp = 0;
	for (size_t i = 0; i < 4; i++)
	{
		xTmp = _cos * double(this->cPoints[i].x - CGameScene::circleXY.x) - _sin * double(this->cPoints[i].y - CGameScene::circleXY.y);
		yTmp = _sin * double(this->cPoints[i].x - CGameScene::circleXY.x) + _cos * double(this->cPoints[i].y - CGameScene::circleXY.y);
		this->points[i].x = xTmp + CGameScene::circleXY.x;
		this->points[i].y = yTmp + CGameScene::circleXY.y;
	}
}
