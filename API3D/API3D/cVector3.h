#pragma once
#include <string>

class cMatrix;

class cVector3
{
private:
	float x, y, z;
public:
	float get_x() const
	{
		return x;
	}

	void set_x(float x)
	{
		this->x = x;
	}

	__declspec(property(get = get_x, put = set_x)) float X;

	float get_y() const
	{
		return y;
	}

	void set_y(float y)
	{
		this->y = y;
	}

	__declspec(property(get = get_y, put = set_y)) float Y;

	float get_z() const
	{
		return z;
	}

	void set_z(float z)
	{
		this->z = z;
	}

	__declspec(property(get = get_z, put = set_z)) float Z;
	cVector3();
	cVector3(float x_, float y_, float z_);
	~cVector3();

	bool operator==(const cVector3& vec);
	bool operator!=(const cVector3& vec);
	cVector3 operator+(const cVector3& vec);
	cVector3 operator-(const cVector3& vec);
	cVector3 operator*(float f);
	cVector3 operator/(float f);

	static float Dot(cVector3& v1, cVector3& v2);
	static cVector3 Cross(cVector3& v1, cVector3& v2);
	float getAngle(cVector3& vec);
	float getLength();
	cVector3 Normalize();
	static cVector3 TransformCoord(cVector3& v, cMatrix& mat);
	static cVector3 TrnasformNormal(cVector3& v, cMatrix& mat);

	
	std::string	to_string();
};
