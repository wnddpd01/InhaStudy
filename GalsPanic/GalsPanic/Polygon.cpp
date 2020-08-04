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
			if (i == 0 && isEqualPoint(points[i], p))
				return vertexCount - 1;
			return i;
		}
	}
	return -1;
}

bool CPolygon::isInBetweenTwoPoint(const POINT& p, const POINT& startP, const POINT& endP)
{
	LONG left, right;
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
		return true;
	return false;
}

bool CPolygon::isInPoly(POINT& p)
{
	HRGN hrgn = CreatePolygonRgn(&(this->points[0]), points.size(), WINDING);
	return PtInRegion(hrgn, p.x, p.y);
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
