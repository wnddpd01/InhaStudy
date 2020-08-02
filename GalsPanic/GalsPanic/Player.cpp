#include "stdafx.h"
#include "Player.h"
#include "InGameScene.h"

Player::Player()
{
	playerBrush = CreateSolidBrush(RGB(255, 0, 0));
}


Player::~Player()
{
}

void Player::PlayerDraw(const HDC hdc)
{
	HBRUSH oldBrush = (HBRUSH)SelectObject(hdc, playerBrush);
	Ellipse(hdc, playerPos.x - 10, playerPos.y - 10, playerPos.x + 10, playerPos.y + 10);
	SelectObject(hdc, oldBrush);
}

void Player::PlayerMove(WPARAM moveDir, CPolygon ** transparentPoly, const HDC inGameShadeHDC)
{
	static UCHAR speed = 10;
	static bool drawMode = false;
	static WPARAM prevMoveDir = NULL;
	POINT tempPoint = { playerPos.x , playerPos.y };
	switch (moveDir)
	{
	case VK_RIGHT :
		tempPoint.x += speed;
		break;
	case VK_LEFT :
		tempPoint.x -= speed;
		break;
	case VK_DOWN :
		tempPoint.y += speed;
		break;
	case VK_UP :
		tempPoint.y -= speed;
		break;
	default:
		break;
	}
	int lineNum = 0;
	static int startLineNum;
	static int endLineNum;
	if ((lineNum = (*transparentPoly)->isInLine(tempPoint)) != -1)
	{
		if (drawMode == true)
		{
			if (prevMoveDir != moveDir)
			{
				playerFootprint.points.push_back(playerPos);
				prevMoveDir = moveDir;
			}
			playerFootprint.points.push_back(tempPoint);
			drawMode = false;
			endLineNum = lineNum;

			if (playerFootprint.getArea() < 0)
			{
				std::reverse(playerFootprint.points.begin(), playerFootprint.points.end());
				int temp = startLineNum;
				startLineNum = endLineNum;
				endLineNum = temp;
			}

			CPolygon * temp1 = new CPolygon(&playerFootprint.points[0], playerFootprint.points.size());
			CPolygon * temp2 = new CPolygon(&playerFootprint.points[0], playerFootprint.points.size());

			int lineSearch = endLineNum;
			while (lineSearch != startLineNum)
			{
				temp1->points.push_back(((*transparentPoly)->points[lineSearch]));
				lineSearch--;
				if (lineSearch < 0)
					lineSearch = (*transparentPoly)->points.size() - 1;
			}

			lineSearch = ++endLineNum % (*transparentPoly)->points.size();

			do 
			{
				temp2->points.push_back(((*transparentPoly)->points[lineSearch]));
				lineSearch++;
				if (lineSearch == (*transparentPoly)->points.size())
					lineSearch = 0;
			} while ((lineSearch != (startLineNum + 1) % (*transparentPoly)->points.size()));

			playerFootprint.points.clear();

			double ret2 = temp2->getArea();
			double ret  = temp1->getArea();
			if (fabs(ret2) > fabs(ret))
			{
				if (ret2 < 0)
					std::reverse(temp2->points.begin(), temp2->points.end());
				*transparentPoly = temp2;
				delete temp1;
			}
			else
			{
				if (ret < 0)
					std::reverse(temp1->points.begin(), temp1->points.end());
				*transparentPoly = temp1;
				delete temp2;
			}
		}
		else
		{
			startLineNum = lineNum;
		}
		setPlayerPos(tempPoint);
		return;
	}
	if ((*transparentPoly)->isInPoly(tempPoint) == false)
	{
		if (drawMode == false)
		{
			drawMode = true;
		}
		if (prevMoveDir != moveDir)
		{
			playerFootprint.points.push_back(playerPos);
			prevMoveDir = moveDir;
		}
		setPlayerPos(tempPoint);
	}
}

RECT Player::getPlayerRect()
{
	RECT playerRect = { playerPos.x - 30, playerPos.y - 30, playerPos.x + 30, playerPos.y + 30 };
	return playerRect;
}

void Player::setPlayerPos(POINT pos)
{
	this->playerPos.x = pos.x;
	this->playerPos.y = pos.y;
}
