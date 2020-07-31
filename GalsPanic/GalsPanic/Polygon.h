#pragma once
#include <vector>

struct CPolygon
{
	std::vector<POINT> points;

public:
	CPolygon() {}
	CPolygon(POINT points[], size_t vertexCount)
	{
		for (size_t i = 0; i < vertexCount; i++)
		{
			this->points.push_back(points[i]);
		}
	}
	int isInLine(POINT & p);
	bool isInPoly(POINT & p);
	double getArea()
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
	~CPolygon()
	{
		points.clear();
	}
};
