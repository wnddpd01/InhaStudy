#include "stdafx.h"
#include "cVector3.h"

#include <cmath>
#include <string>


cVector3::cVector3() : cVector3(0, 0, 0)
{
}

cVector3::cVector3(float x_, float y_, float z_)
{
	this->x = x_;
	this->y = y_;
	this->z = z_;
}


cVector3::~cVector3()
{
}

bool cVector3::operator==(const cVector3& vec)
{
	return this->x == vec.x && this->y == vec.y && this->z == vec.z;
}

bool cVector3::operator!=(const cVector3& vec)
{
	return this->x != vec.x || this->y != vec.y || this->z != vec.z;
}

cVector3 cVector3::operator+(const cVector3& vec)
{
	cVector3 ret;
	ret.x = this->x + vec.x;
	ret.y = this->y + vec.y;
	ret.z = this->z + vec.z;
 	return ret;
}

cVector3 cVector3::operator-(const cVector3& vec)
{
	cVector3 ret;
	ret.x = this->x - vec.x;
	ret.y = this->y - vec.y;
	ret.z = this->z - vec.z;
	return ret;
}

cVector3 cVector3::operator*(float f)
{
	cVector3 ret;
	ret.x = this->x * f;
	ret.y = this->y * f;
	ret.z = this->z * f;
	return ret;
}

cVector3 cVector3::operator/(float f)
{
	cVector3 ret;
	ret.x = this->x / f;
	ret.y = this->y / f;
	ret.z = this->z / f;
	return ret;
}

float cVector3::Dot(cVector3& v1, cVector3& v2)
{
	return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}

cVector3 cVector3::Cross(cVector3& v1, cVector3& v2)
{
	cVector3 ret(v1.y * v2.z - v2.y * v1.z, v1.z * v2.x - v2.z * v1.x, v1.x * v2.y - v2.x * v1.y);
	return ret;
}

float cVector3::getAngle(cVector3& vec)
{
	float cosRadian = cVector3::Dot(*this, vec) / (this->getLength() * vec.getLength());
	return acos(cosRadian) * 180 / 3.14159;	
}

float cVector3::getLength()
{
	return sqrt(x * x + y * y + z * z);
}

cVector3 cVector3::Normalize()
{
	float length = getLength();
	cVector3 ret(this->x / length, this->y / length, this->z / length);
	return ret;
}

cVector3 cVector3::TransformCoord(cVector3& v, cMatrix& mat)
{
	cVector3 ret;
	float w = 1;

	ret.x = v.x * mat[0][0] + v.y * mat[1][0] + v.z * mat[2][0] + 1.0 * mat[3][0];
	ret.y = v.x * mat[0][1] + v.y * mat[1][1] + v.z * mat[2][1] + 1.0 * mat[3][1];
	ret.z = v.x * mat[0][2] + v.y * mat[1][2] + v.z * mat[2][2] + 1.0 * mat[3][2];
	w = v.x * mat[0][3] + v.y * mat[1][3] + v.z * mat[2][3] + 1.0 * mat[3][3];
	//ret.x = v.x * mat[0][0] + v.y * mat[0][1] + v.z * mat[0][2] + w * mat[0][3];
	//ret.y = v.x * mat[1][0] + v.y * mat[1][1] + v.z * mat[1][2] + w * mat[1][3];
	//ret.z = v.x * mat[2][0] + v.y * mat[2][1] + v.z * mat[2][2] + w * mat[2][3];
	//w = v.x * mat[3][0] + v.y * mat[3][1] + v.z * mat[3][2] + 1.0 * mat[3][3];
	ret.x /= w;
	ret.y /= w;
	ret.z /= w;
	return ret;
}

std::string cVector3::to_string()
{
	std::string ret;
	ret = "< x : " + std::to_string(x) + ", y : " + std::to_string(y) + ", z : " + std::to_string(z) + " >";
	return ret;
}
