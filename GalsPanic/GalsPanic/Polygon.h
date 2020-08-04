#pragma once
#include <vector>

class CPolygon
{

public:
	std::vector<POINT> points;
	CPolygon() {}
	CPolygon(POINT points[], size_t vertexCount)
	{
		for (size_t i = 0; i < vertexCount; i++)
		{
			this->points.push_back(points[i]);
		}
	}
	int isInLine(POINT& p, size_t vertexCount = 0);
	bool isInBetweenTwoPoint(const POINT& p, const POINT& startP, const POINT& endP);
	bool isInPoly(POINT & p);
	double getArea();
	~CPolygon()
	{
		points.clear();
	}
};
