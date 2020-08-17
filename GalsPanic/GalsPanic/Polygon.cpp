#include "stdafx.h"
#include "Polygon.h"

int CPolygon::isInLine(POINT& p, size_t vertexCount)
{
	if (vertexCount == 0)
		vertexCount = this->points.size();
	for (int i = 0; i < vertexCount; ++i) {
		int j = (i + 1) % this->points.size();
		if (isInBetweenTwoPoint(p, points[i], points[j]) == true)
		{
			return i;
		}
	}
	return -1;
}

bool CPolygon::isInBetweenTwoPoint(const POINT& p, const POINT& startP, const POINT& endP)
{
	/*LONG left, right;
	LONG up, down; 

	if (startP.x < endP.x)
	{
		left = startP.x;
		right = endP.x;
	}
	else
	{
		left = endP.x;
		right = startP.x;
	}

	if (startP.y < endP.y)
	{
		up = startP.y;
		down = endP.y;
	}
	else
	{
		up = endP.y;
		down = startP.y;
	}

	if (p.x >= left && p.x <= right && p.y >= up && p.y <= down)
		return true;*/
	if (startP.y == endP.y)
	{
		if (startP.y != p.y)
			return false;
		if (startP.x == p.x)
			return true;
		if ((startP.x > p.x) != (endP.x > p.x))
			return true;
	}
	else if(startP.x == endP.x)
	{
		if (startP.x != p.x)
			return false;
		if (startP.y == p.y)
			return true;
		if ((startP.y > p.y) != (endP.y > p.y))
			return true;
	}
	return false;
}

bool CPolygon::isInPoly(POINT& p)
{
	/*HRGN hrgn = CreatePolygonRgn(&(this->points[0]), points.size(), WINDING);
	return PtInRegion(hrgn, p.x, p.y);*/
	int crosses = 0;
	size_t vertexCount = this->points.size();
	for (int i = 0; i < vertexCount; i++) {
		int j = (i + 1) % vertexCount;
		if (isInBetweenTwoPoint(p, points[i], points[j]) == true)
			return false;
		if ((points[i].y > p.y) != (points[j].y > p.y)) 
		{
			double atX = (points[j].x - points[i].x)*(p.y - points[i].y) / (points[j].y - points[i].y) + points[i].x;
			if (p.x < atX)
				crosses++;
		}
	}
	return crosses % 2 > 0;
}

void CPolygon::MergePolygon(std::vector<POINT> & newPolygon)
{
	ULONG startLineNum = isInLine(newPolygon.front());
	ULONG endLineNum = isInLine(newPolygon.back());

	if (startLineNum == endLineNum && CheckPolygonMergedCorrectly(newPolygon))
	{
		MergePolygonEnd(newPolygon);
		return;
	}


	std::vector<POINT> mergedPolygon(newPolygon);

	MakeMergedPolygon(mergedPolygon, startLineNum, endLineNum);

	if (CheckPolygonMergedCorrectly(mergedPolygon) == false)
	{
		mergedPolygon.clear();
		std::reverse(newPolygon.begin(), newPolygon.end());
		std::swap(startLineNum, endLineNum);
		mergedPolygon = newPolygon;
		MakeMergedPolygon(mergedPolygon, startLineNum, endLineNum);
	}

	MergePolygonEnd(mergedPolygon);
	return;
}

bool CPolygon::CheckPolygonMergedCorrectly(std::vector<POINT>& newPolygon)
{
	CPolygon temp;
	temp.points = newPolygon;
	size_t vertexCount = points.size();
	for (size_t i = 0; i < vertexCount; i++)
	{
		if (temp.isInLine(points[i]) == -1 && temp.isInPoly(points[i]) == false )
			return false;
	}
	vertexCount = newPolygon.size();
	for (size_t i = 0; i < vertexCount; i++)
	{
		if (getLineSlope(temp.points[i], temp.points[(i + 1) % vertexCount]) != 0)
			return false;
	}
	return true;
}

void CPolygon::MakeMergedPolygon(std::vector<POINT>& mergedPolygon, const ULONG & startLineNum, const ULONG & endLineNum)
{
	int searchPointNum = (endLineNum + 1) % this->points.size();
	while (searchPointNum != startLineNum)
	{
		mergedPolygon.push_back(this->points[searchPointNum]);
		searchPointNum++;
		if (searchPointNum == this->points.size())
			searchPointNum = 0;
	}

	if (mergedPolygon.front() != this->points[startLineNum])
	{
		mergedPolygon.push_back(this->points[startLineNum]);
	}
}

void CPolygon::MergePolygonEnd(std::vector<POINT>& mergedPolygon)
{
	this->points.clear();
	this->points.insert(this->points.begin(), mergedPolygon.begin(), mergedPolygon.end());
}

double CPolygon::getDistanceWithPolygon(POINT & p)
{
	double dis = 111111;
	size_t vertexCount = points.size();

	for (int i = 0; i < vertexCount; ++i)
	{
		int j = (i + 1) % vertexCount;
		double tempDis = getDistancePointAndLine(p, this->points[i], this->points[j]);
		if (tempDis < dis)
		{
			if (this->points[i].y - this->points[j].y == 0 && this->points[i].x > p.x == this->points[j].x > p.x)
			{
				continue;
			}
			if (this->points[i].x - this->points[j].x == 0 && this->points[i].y > p.y == this->points[j].y > p.y)
			{
				continue;
			}
			dis = tempDis;
		}
	}
	return dis;
}

double CPolygon::getArea()
{
	double area = 0.0;
	size_t vertexCount = points.size();

	for (int i = 0; i < vertexCount; ++i)
	{
		int j = (i + 1) % vertexCount;
		area += 0.5 * (points[i].x * points[j].y - points[j].x * points[i].y);
	}

	return area;
}
