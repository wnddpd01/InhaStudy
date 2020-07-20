#include "stdafx.h"
#include "GameScene.h"

extern Singleton * singleton;


CGameScene::CGameScene()
{
	singleton = Singleton::GetInstance();
	nameRect = { 0, 0, 225, 50 };
	timeRect = { nameRect.right, 0, nameRect.right + 150, 50 };
	scoreRect = { timeRect.right, 0, timeRect.right + 225, 50 };

	unsigned short leftX;
	unsigned short rightX;
	unsigned short upY;
	unsigned short downY;
	circleXY.x = 300;
	circleXY.y = 760;
	cannon = new Cannon;
	cannon->objType = ObjectType::Cannon;
	cannon->spdX = cannon->spdY = 0;
	cannon->x = 300;
	cannon->y = 660;
	cannon->points = new POINT[4];

	leftX = 300 - 15;
	rightX = 300 + 15;
	upY = 690 - 30;
	downY = 690 + 30;

	cannon->cPoints[0].x = cannon->cPoints[3].x = cannon->points[0].x = cannon->points[3].x = leftX;
	cannon->cPoints[1].x = cannon->cPoints[2].x = cannon->points[1].x = cannon->points[2].x = rightX;
	cannon->cPoints[0].y = cannon->cPoints[1].y = cannon->points[0].y = cannon->points[1].y = upY;
	cannon->cPoints[2].y = cannon->cPoints[3].y = cannon->points[2].y = cannon->points[3].y = downY;
	cannon->curAngle = 0;
	objects.push_back(cannon);
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
		objects.push_back(healthBox);
	}

	for (size_t i = 0; i < 100; i++)
	{
		CObject* bullet = new CObject;
		bullet->objType = ObjectType::Bullet;
		bullet->isDead = true;
		deadBulletPool.push(bullet);
	}
}


CGameScene::~CGameScene()
{
}

void CGameScene::Init(void)
{
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
			CObject* bullet = deadBulletPool.top();
			if (bullet == NULL)
			{
				bullet = new CObject;
				bullet->objType = ObjectType::Bullet;
				bullet->points = nullptr;
			}
			else
			{
				deadBulletPool.pop();
				bullet->isDead = false;
			}
			bullet->x = (cannon->points[1].x - cannon->points[0].x) * 0.5 + cannon->points[0].x;
			bullet->y = (cannon->points[1].y - cannon->points[0].y) * 0.5 + cannon->points[0].y;
			bullet->spdX = cannon->points[0].x - cannon->points[3].x;
			bullet->spdY = cannon->points[0].y - cannon->points[3].y;
			objects.push_back(bullet);
		}
	}
	break;
	}
}

void CGameScene::Render(HDC hdc)
{
	HFONT oldFont = (HFONT)SelectObject(hdc, singleton->idSmallFont);
	Rectangle(hdc, nameRect.left, nameRect.top, nameRect.right, nameRect.bottom);
	DrawText(hdc, (L"ID : " + singleton->name).c_str(), singleton->name.size() + 5, &nameRect, DT_SINGLELINE | DT_CENTER | DT_VCENTER);

	Rectangle(hdc, timeRect.left, timeRect.top, timeRect.right, timeRect.bottom);

	Rectangle(hdc, scoreRect.left, scoreRect.top, scoreRect.right, scoreRect.bottom);
	SelectObject(hdc, oldFont);

	Ellipse(hdc, circleXY.x - 60, circleXY.y - 60, circleXY.x + 60, circleXY.y + 60);

	for (CObject* object : objects)
	{
		if (object->isDead == false)
		{
			if (object->objType == ObjectType::Bullet)
			{
				Ellipse(hdc, object->x - 10, object->y - 10, object->x + 10, object->y + 10);
			}
			else
				Polygon(hdc, object->points, 4);
		}
	}
}

void CGameScene::Free(void)
{
}

void CGameScene::CannonRotate(SHORT angle)
{
	Cannon * cannon = (Cannon *)objects.front();
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
