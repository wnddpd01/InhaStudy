#include "stdafx.h"
#include "cVector3.h"

#define EPSILON 0.0001f

cVector3::cVector3()
{
	this->x = 0;
	this->y = 0;
	this->z = 0;
	this->w = 0;
}

cVector3::cVector3(float _x, float _y, float _z, float _w)
{
	this->x = _x;
	this->y = _y;
	this->z = _z;
	this->w = _w;
}

cVector3::~cVector3()
{
}

bool cVector3::operator==(cVector3 & vec)
{
	if (abs(this->x - vec.x) <= EPSILON &&
		abs(this->y - vec.y) <= EPSILON &&
		abs(this->z - vec.z) <= EPSILON)
		return true;
	else
		return false;
}

bool cVector3::operator!=(cVector3 & vec)
{
	if (abs(this->x - vec.x) > EPSILON &&
		abs(this->y - vec.y) > EPSILON &&
		abs(this->z - vec.z) > EPSILON)
		return true;
	else
		return false;
}

cVector3 cVector3::operator+(cVector3 & vec)
{
	cVector3 cVec;
	cVec.x = this->x + vec.x;
	cVec.y = this->y + vec.y;
	cVec.z = this->z + vec.z;
	return cVec;
}

cVector3 cVector3::operator-(cVector3 & vec)
{
	cVector3 cVec;
	cVec.x = this->x - vec.x;
	cVec.y = this->y - vec.y;
	cVec.z = this->z - vec.z;
	return cVec;
}

cVector3 cVector3::operator*(float f)
{
	cVector3 cVec;
	cVec.x = this->x * f;
	cVec.y = this->y * f;
	cVec.z = this->z * f;
	return cVec;
}


cVector3 cVector3::operator/(float f)
{
	cVector3 cVec;
	cVec.x = this->x / f;
	cVec.y = this->y / f;
	cVec.z = this->z / f;
	return cVec;
}

float cVector3::Dot(cVector3 & v1, cVector3 & v2)
{
	return (v1.x * v2.x) + (v1.y * v2.y) + (v1.z * v2.z);
}

cVector3 cVector3::Cross(cVector3 & v1, cVector3 & v2)
{
	cVector3 cVec;
	cVec.x = (v1.y * v2.z) - (v1.z * v2.y);
	cVec.y = (v1.z * v2.x) - (v1.x * v2.z);
	cVec.z = (v1.x * v2.y) - (v1.y * v2.x);
	return cVec;
}

float cVector3::Length()
{
	return pow(pow(this->x, 2) + pow(this->y, 2) + pow(this->z, 2), 0.5f);
}

cVector3 cVector3::Normalize()
{
	cVector3 cVec;
	cVec.x = this->x / Length();
	cVec.y = this->y / Length();
	cVec.z = this->z / Length();
	return cVec;
}

float cVector3::Angle(cVector3 & v1, cVector3 & v2)
{
	return acos(cVector3::Dot(v1, v2) / (v1.Length() * v2.Length()));
}

string cVector3::to_String()
{
	string str;
	str = "x = " + to_string(this->x) + " y = " + to_string(this->y) + " z = " + to_string(this->z);
	return str;
}


cVector3 cVector3::TransformCoord(cVector3& v, cMatrix& mat)
{
	cVector3 newVec(0, 0, 0, 0);
	newVec.x = mat[0][0] * v.x + mat[1][0] * v.y + mat[2][0] * v.z + mat[3][0] * 1.0;
	newVec.y = mat[0][1] * v.x + mat[1][1] * v.y + mat[2][1] * v.z + mat[3][1] * 1.0;
	newVec.z = mat[0][2] * v.x + mat[1][2] * v.y + mat[2][2] * v.z + mat[3][2] * 1.0;
	newVec.w = mat[0][3] * v.x + mat[1][3] * v.y + mat[2][3] * v.z + mat[3][3] * 1.0;

	newVec.x /= newVec.w;
	newVec.y /= newVec.w;
	newVec.z /= newVec.w;
	newVec.w /= newVec.w;
	return newVec;
}

cVector3 cVector3::TransformNormal(cVector3& v, cMatrix& mat)
{
	cVector3 newVec(0, 0, 0, 0);
	newVec.x = mat[0][0] * v.x + mat[1][0] * v.y + mat[2][0] * v.z + mat[3][0];
	newVec.y = mat[0][1] * v.x + mat[1][1] * v.y + mat[2][1] * v.z + mat[3][1];
	newVec.z = mat[0][2] * v.x + mat[1][2] * v.y + mat[2][2] * v.z + mat[3][2];

	return newVec;
}
