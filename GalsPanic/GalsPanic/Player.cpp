#include "stdafx.h"
#include "Player.h"
#include "InGameScene.h"

Player::Player()
{
	playerPos = { 0, 0 };
}


Player::~Player()
{
}


void Player::PlayerMove(WPARAM moveDir, CPolygon ** transparentPoly)
{
	static UCHAR speed = 8;
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

	if (BoundaryCheck(tempPoint) == false)
		return;

	int lineNum =0;
	static ULONG startLineNum = 0;
	static ULONG endLineNum = 0;

	if (drawMode == true)
	{
		if (isInFootPrint(tempPoint))
		{
			if (isEqualPoint(tempPoint, playerFootprint.points.front()))
			{
				if (playerFootprint.points.size() == 1)
				{
					playerFootprint.points.pop_back();
					setPlayerPos(tempPoint);
					drawMode = false;
					return;
				}
			}
			else if (abs(getPointDirection(playerFootprint.points.back(), playerPos) - int(moveDir)) == 2)
			{
				if (isEqualPoint(tempPoint, playerFootprint.points.back()))
				{
					char direction = getPointDirection(playerPos, playerFootprint.points.back());
					if (moveDir % 2 == 1)
						prevMoveDir = VK_RIGHT + direction;
					else
						prevMoveDir = VK_UP + direction;
					playerFootprint.points.pop_back();
					if (playerFootprint.points.size() == 0)
					{
						drawMode = false;
						setPlayerPos(tempPoint);
						return;
					}
				}
			}
			else
			{
				return;
			}
		}
		else if (prevMoveDir != moveDir && abs(getPointDirection(playerFootprint.points.back(), tempPoint) - int(moveDir)) != 2)
		{
			{
				playerFootprint.points.push_back(playerPos);
				prevMoveDir = moveDir;
			}
		}
	}

	if ((lineNum = (*transparentPoly)->isInLine(tempPoint)) != -1)
	{
		if (drawMode == false)
		{
			POINT tempOnePixelPoint = { playerPos.x, playerPos.y};	
			switch (moveDir)
			{
			case VK_RIGHT:
				tempOnePixelPoint.x += 1;
				break;
			case VK_LEFT:
				tempOnePixelPoint.x -= 1;
				break;
			case VK_DOWN:
				tempOnePixelPoint.y += 1;
				break;
			case VK_UP:
				tempOnePixelPoint.y -= 1;
				break;
			default:
				break;
			}
			if ((*transparentPoly)->isInLine(tempOnePixelPoint) == -1 && (*transparentPoly)->isInPoly(tempOnePixelPoint) == false)
			{
				playerFootprint.points.push_back(playerPos);
				drawMode = true;
			}
			else
			{
				startLineNum = lineNum;
			}
		}
		if (drawMode == true)
		{
			drawMode = false;
			endLineNum = lineNum;
			playerFootprint.points.push_back(tempPoint);
			if (startLineNum == endLineNum)
			{
				if (playerFootprint.getArea() < 0 || getPointDistance((*transparentPoly)->points[startLineNum], playerFootprint.points.front())
					> getPointDistance((*transparentPoly)->points[startLineNum], playerFootprint.points.back()))
				{
					footprintOrderSort(&startLineNum, &endLineNum);
				}
			}

			CPolygon* selected;
			CPolygon* temp1 = new CPolygon(&playerFootprint.points[0], playerFootprint.points.size());
			CPolygon* temp2 = new CPolygon(&playerFootprint.points[0], playerFootprint.points.size());

			ULONG lineSearch = endLineNum;
			ULONG destLineNum = startLineNum;
			while (lineSearch != destLineNum)
			{
				temp1->points.push_back(((*transparentPoly)->points[lineSearch]));
				if (lineSearch == 0)
					lineSearch = (*transparentPoly)->points.size();
				lineSearch--;
			}

			lineSearch = (endLineNum + 1) % (*transparentPoly)->points.size();
			destLineNum = (startLineNum + 1) % (*transparentPoly)->points.size();

			if (endLineNum == startLineNum)
			{
				if(getPointDistance((*transparentPoly)->points[startLineNum], playerFootprint.points.front()) 
					< getPointDistance((*transparentPoly)->points[startLineNum], playerFootprint.points.back()))
				{
					temp2->points.push_back(((*transparentPoly)->points[lineSearch]));
					lineSearch = (lineSearch + 1) % (*transparentPoly)->points.size();
				}
			}
			while (lineSearch != destLineNum)
			{
				temp2->points.push_back(((*transparentPoly)->points[lineSearch]));
				lineSearch++;
				if (lineSearch == (*transparentPoly)->points.size())
					lineSearch = 0;
			} 

			double ret2 = temp2->getArea();
			double ret = temp1->getArea();
			if (fabs(ret2) > fabs(ret))
			{
				selected = temp2;
				if (ret2 < 0)
					std::reverse(selected->points.begin(), selected->points.end());
				delete temp1;
			}
			else
			{
				selected = temp1;
				if (ret < 0)
					std::reverse(selected->points.begin(), selected->points.end());
				delete temp2;
			}
			delete* transparentPoly;
			*transparentPoly = selected;
			playerFootprint.points.clear();
			startLineNum = (*transparentPoly)->isInLine(tempPoint);
		}
		setPlayerPos(tempPoint);
	}

	else if ((*transparentPoly)->isInPoly(tempPoint) == false)
	{
		if (drawMode == false)
		{
			drawMode = true;
			playerFootprint.points.push_back(playerPos);
			prevMoveDir = moveDir;
		}
		setPlayerPos(tempPoint);
	}
}

void Player::footprintOrderSort(ULONG* startLineNum, ULONG* endLineNum)
{
	std::reverse(playerFootprint.points.begin(), playerFootprint.points.end());
	std::swap(*startLineNum, *endLineNum);
}

bool Player::isInFootPrint(POINT &p)
{
	if (playerFootprint.points.size() == 0)
		return false;
	return this->playerFootprint.isInLine(p, this->playerFootprint.points.size() - 1) != -1;
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
