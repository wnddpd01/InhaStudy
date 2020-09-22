#pragma once

class cMatrix;

class cVector3
{
public:
	cVector3();
	cVector3(float _x, float _y, float _z, float w);
	~cVector3();

public:
	float x, y, z, w;
public:
	bool operator == (cVector3 &vec);
	bool operator != (cVector3 &vec);
	cVector3 operator + (cVector3 &vec);
	cVector3 operator - (cVector3 &vec);
	cVector3 operator * (float f);
	cVector3 operator / (float f);

	static float Dot(cVector3 &v1, cVector3 &v2);		//내적
	static cVector3 Cross(cVector3 &v1, cVector3 &v2);	//외적
	float Length();
	cVector3 Normalize();
	static float Angle(cVector3 &v1, cVector3 &v2);

	string to_String();

	//
	static cVector3 TransformCoord(cVector3 &v, cMatrix &mat);		//점에 대한 이동
	static cVector3 TransformNormal(cVector3 &v, cMatrix &mat);
};

