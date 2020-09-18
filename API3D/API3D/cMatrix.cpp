#include "stdafx.h"
#include "cMatrix.h"

#include <ctime>
#include <iostream>


cMatrix::cMatrix() : cMatrix(4, 0)
{
}


cMatrix::~cMatrix()
{
}

cMatrix::cMatrix(int nDimension) : cMatrix(nDimension, 0)
{
	
}

cMatrix::cMatrix(int nDimension, int data)
{
	dimension_ = nDimension;
	for (size_t i = 0; i < nDimension; i++)
	{
		matrix_.emplace_back(dimension_);
		for (size_t j = 0; j < nDimension; j++)
		{
			matrix_[i][j] = 0;
		}
	}
}

void cMatrix::Print()
{
	std::cout.precision(2);
	std::cout << std::fixed;
	for (size_t i = 0; i < dimension_; i++)
	{
		for (size_t j = 0; j < dimension_; j++)
		{
			std::cout.width(6);
			std::cout << matrix_[i][j] << '\t';
		}
		std::cout << std::endl;
	}
}

int cMatrix::Dimension()
{
	return dimension_;
}

cMatrix cMatrix::Identity(int nDimension)
{
	cMatrix I(nDimension, 0);
	for (size_t i = 0; i < nDimension; i++)
	{
		I[i][i] = 1;
	}

	return I;
}

bool cMatrix::operator==(cMatrix& mat)
{
	if (this->dimension_ != mat.Dimension())
		return false;
	for (size_t i = 0; i < dimension_; i++)
	{
		for (size_t j = 0; j < dimension_; j++)
		{
			if (matrix_[i][j] != mat[i][j])
				return false;
		}
	}
	return true;
}

bool cMatrix::operator!=(cMatrix& mat)
{
	return !(*this == mat);
}

cMatrix cMatrix::operator+(cMatrix& mat)
{
	cMatrix ret(dimension_);

	for (size_t i = 0; i < dimension_; i++)
	{
		for (size_t j = 0; j < dimension_; j++)
		{
			ret[i][j] = matrix_[i][j] + mat[i][j];
		}
	}
	return ret;
}

cMatrix cMatrix::operator-(cMatrix& mat)
{
	cMatrix ret(dimension_);

	for (size_t i = 0; i < dimension_; i++)
	{
		for (size_t j = 0; j < dimension_; j++)
		{
			ret[i][j] = matrix_[i][j] - mat[i][j];
		}
	}
	return ret;
}

cMatrix cMatrix::operator*(cMatrix& mat)
{
	cMatrix ret(dimension_, 0);
	for (size_t i = 0; i < dimension_; i++)
	{
		for (size_t j = 0; j < dimension_; j++)
		{
			float element = 0;
			for (size_t k = 0; k < dimension_; k++)
			{
				element += matrix_[i][k] * mat[k][j];
			}
			ret[i][j] = element;
		}
	}
	return ret;
}

cMatrix cMatrix::operator*(float f)
{
	cMatrix ret(dimension_, 0);
	for (size_t i = 0; i < dimension_; i++)
	{
		for (size_t j = 0; j < dimension_; j++)
		{
			ret[i][j] = matrix_[i][j] * f;
		}
	}
	return ret;
}

cMatrix cMatrix::operator/(float f)
{
	cMatrix ret(dimension_, 0);
	for (size_t i = 0; i < dimension_; i++)
	{
		for (size_t j = 0; j < dimension_; j++)
		{
			ret[i][j] = matrix_[i][j] / f;
		}
	}
	return ret;
}

cMatrix cMatrix::Transpose()
{
	cMatrix ret(dimension_, 0);
	for (size_t i = 0; i < dimension_; i++)
	{
		for (size_t j = 0; j < dimension_; j++)
		{
			ret[j][i] = matrix_[i][j];
		}
	}
	return ret;
}

float cMatrix::Determinant()
{
	if(dimension_ == 2)
	{
		return matrix_[0][0] * matrix_[1][1] - matrix_[0][1] * matrix_[1][0];
	}
	float ret = 0;
	for (size_t i = 0; i < dimension_; i++)
	{
		ret += matrix_[i][0] * Cofactor(i, 0);
	}
	return ret;
}

cMatrix cMatrix::Inverse()
{
	return Adjoint() / Determinant();
}

cMatrix cMatrix::Adjoint()
{
	cMatrix ret(dimension_, 0);

	for (size_t i = 0; i < dimension_; i++)
	{
		for (size_t j = 0; j < dimension_; j++)
		{
			ret[j][i] = Cofactor(i, j);
		}
	}
	return ret;
}

float cMatrix::Cofactor(int nRow, int nCol)
{
	return ((nRow + nCol) % 2 == 0 ? 1 : -1 ) * Minor(nRow, nCol);
}

float cMatrix::Minor(int nRow, int nCol)
{
	cMatrix minorMatrix(dimension_ - 1, 0);

	size_t minor_i_count = 0;
	for (size_t i = 0; i < dimension_; i++)
	{
		if(nRow == i)
			continue;
		size_t minor_j_count = 0;
		for (size_t j = 0; j < dimension_; j++)
		{
			if(j == nCol)
				continue;
			minorMatrix[minor_i_count][minor_j_count] = matrix_[i][j];
			minor_j_count++;
		}
		minor_i_count++;
	}
	return minorMatrix.Determinant();
}

cMatrix cMatrix::Translation(float x, float y, float z)
{
	cMatrix ret = cMatrix::Identity(4);
	ret[3][0] = x;
	ret[3][1] = y;
	ret[3][2] = z;
	return ret;
}

cMatrix cMatrix::Translation(cVector3& v)
{
	return Translation(v.get_x(), v.get_y(), v.get_z());
}

cMatrix cMatrix::RotationX(float angle)
{
	cMatrix ret = cMatrix::Identity();
	float radian = deg_to_rad(angle);
	ret[1][1] = cos(radian);
	ret[1][2] = sin(radian);
	ret[2][1] = -sin(radian);
	ret[2][2] = cos(radian);
	return ret;
}

cMatrix cMatrix::RotationY(float angle)
{
	cMatrix ret = cMatrix::Identity();
	float radian = deg_to_rad(angle);
	ret[0][0] = cos(radian);
	ret[0][2] = -sin(radian);
	ret[2][0] = sin(radian);
	ret[2][2] = cos(radian);
	return ret;
}

cMatrix cMatrix::RotationZ(float angle)
{
	cMatrix ret = cMatrix::Identity();
	float radian = deg_to_rad(angle);
	ret[0][0] = cos(radian);
	ret[0][1] = sin(radian);
	ret[1][0] = -sin(radian);
	ret[1][1] = cos(radian);
	return ret;
}

cMatrix cMatrix::View(cVector3& vEye, cVector3& vLookAt, cVector3& vUp)
{/*
*view
*	eye , look at , up
*	l = look vector
*	r = right vector
*	u = up vector
*
*		r.x			u.x			l.x		0
*		r.y			u.y			l.y		0
*		r.z			u.z			l.z		0
*		-r dot eye -u dot eye	-l dot eye 0
*
* projection
*	sy = cot(fovY / 2) => 1/0f / tanF(fovY / 2.0f)
*	sx = sy / aspect
*		sx		0		0		0
*		0		sy		0		0
*		0		0	fz/(fz-nz)	1
*		0		0	-fz*nz/(fz-nz)	0
*	viewport
*		maxz = 1, minz = 0
*		w/2		0		0		0
*		0		-h/2	0		0
*		0		0	maxz-minz	0
*		x+w/2  y+h/2	minz	1
*/

	static cVector3 target(0,0,0);
	vLookAt = target - vEye;
	cVector3 rightVector = cVector3::Cross(vUp, vLookAt);
	vUp = cVector3::Cross(vLookAt, rightVector).Normalize();
	cMatrix ret;
	ret[0][0] = rightVector.get_x(); ret[1][0] = rightVector.get_y(); ret[2][0] = rightVector.get_z();
	ret[0][1] = vUp.get_x(); ret[1][1] = vUp.get_y(); ret[2][1] = vUp.get_z();
	ret[0][2] = vLookAt.get_x(); ret[1][2] = vLookAt.get_y(); ret[2][2] = vLookAt.get_z();
	ret[3][0] = cVector3::Dot(rightVector * -1, vEye); ret[3][1] = cVector3::Dot(vUp * -1, vEye); ret[3][2] = cVector3::Dot(vLookAt * -1, vEye);
	return ret;
}

cMatrix cMatrix::Projection(float fFovY, float fAspect, float fNearZ, float fFarZ)
{
	cMatrix ret;
	float radian = deg_to_rad(fFovY);
	float sy = (float)1 / tan(radian*0.5);
	float sx = sy / fAspect;
	float q = fFarZ / (fFarZ - fNearZ);
	ret[0][0] = sx;
	ret[1][1] = sy;
	ret[2][2] = q;
	ret[3][2] = -q * fNearZ;
	ret[2][3] = 1;
	return ret;
}

cMatrix cMatrix::Viewport(float x, float y, float w, float h, float minz, float maxz)
{
	cMatrix ret;
	ret[0][0] = w * 0.5;
	ret[1][1] = -h * 0.5;
	ret[2][2] = maxz - minz;
	ret[3][0] = x + ret[0][0];
	ret[3][1] = y + -ret[1][1];
	ret[3][2] = minz;
	ret[3][3] = 1;
	return ret;
}
