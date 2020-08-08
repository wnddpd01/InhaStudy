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
	void MergePolygon(std::vector<POINT> & newPolygon);
	bool CheckPolygonMergedCorrectly(std::vector<POINT> & newPolygon);
	void MakeMergedPolygon(std::vector<POINT> & mergedPolygon, const ULONG& startLineNum, const ULONG& endLineNum);
	void MergePolygonEnd(std::vector<POINT>& mergedPolygon);
	double getArea();
	virtual ~CPolygon()
	{
		points.clear();
	}
};
