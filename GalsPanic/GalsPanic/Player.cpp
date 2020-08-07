#include "stdafx.h"
#include "Player.h"
#include "InGameScene.h"

Player::Player()
{
	drawMode = false;
	playerPos = { 0, 0 };
	ULONG startLineNum = 0;
	ULONG endLineNum = 0;
}


Player::~Player()
{
}


WPARAM Player::PlayerMove(WPARAM moveDir, CPolygon ** transparentPoly)
{
	if (moveDir == NULL)
		return EVENT_NO_EVENT;
	static UCHAR speed = 2;

	POINT tempPoint = MakeNextPointWithDir(playerPos, moveDir, speed);
	if (BoundaryCheck(tempPoint) == false)
		return EVENT_PLAYER_CANT_MOVE;

	POINT onePixel = MakeNextPointWithDir(playerPos, moveDir, 1);
	if((*transparentPoly)->isInPoly(onePixel) == true)
		return EVENT_PLAYER_CANT_MOVE;


	if (playerFootprint.points.size() > 0)
	{
		if (abs(int(moveDir) - int(prevMoveDir)) == 2)
		{
			if (playerFootprint.points.back() == tempPoint)
			{
				playerFootprint.points.pop_back();
				if (playerFootprint.points.size() == 0)
					drawMode = false;
				else
					prevMoveDir = getPointDirection(playerFootprint.points.back(), tempPoint);
			}
			setPlayerPos(tempPoint);
			return EVENT_NO_EVENT;
		}
		else if (playerFootprint.points.front() != tempPoint && isInFootPrint(tempPoint))
		{
			return EVENT_PLAYER_CANT_MOVE;
		}
	}

	lastMoveDir = moveDir;

	if ((*transparentPoly)->isInLine(tempPoint) != -1)
	{
		if ((*transparentPoly)->isInLine(onePixel) == -1)
		{
			MoveOutOfPoly(tempPoint);
		}
		MoveToLine(tempPoint);
		if (NeedDrawNewPoly())
		{
			if (playerFootprint.getArea() < 0)
				footprintOrderSort();
			return EVENT_DRAW_NEW_TR_POLYGON;
		}
	}
	else if ((*transparentPoly)->isInPoly(tempPoint) == false)
	{
		MoveOutOfPoly(tempPoint);
	}

	return EVENT_NO_EVENT;
}

void Player::MoveToLine(POINT & tempPoint)
{
	if (drawMode == false)
	{
	}
	else
	{
		if (lastMoveDir != prevMoveDir)
		{
			playerFootprint.points.push_back(playerPos);
		}
		playerFootprint.points.push_back(tempPoint);
		drawMode = false;
	}
	prevMoveDir = lastMoveDir;
	setPlayerPos(tempPoint);
}

void Player::MoveOutOfPoly(POINT & tempPoint)
{
	if (drawMode == false)
	{
		playerFootprint.points.push_back(playerPos);
		drawMode = true;
	}
	else if (lastMoveDir != prevMoveDir)
	{
		playerFootprint.points.push_back(playerPos);
	}
	prevMoveDir = lastMoveDir;
	setPlayerPos(tempPoint);
}

bool Player::NeedDrawNewPoly()
{
	if (playerFootprint.points.size() > 0 && drawMode == false)
		return true;
	return false;
}

void Player::footprintOrderSort()
{
	std::reverse(playerFootprint.points.begin(), playerFootprint.points.end());
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

void Player::setPlayerDestPos(WPARAM dir)
{
	if (prevMoveDir != dir)
	{
		playerDestPos = MakeNextPointWithDir(playerPos, dir, 8);
	}
	else
	{
		playerDestPos = MakeNextPointWithDir(playerDestPos, dir, 8);
	}
}
