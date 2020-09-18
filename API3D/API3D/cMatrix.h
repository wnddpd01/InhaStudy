#pragma once
#include <vector>

class cRow
{
public:
	cRow() = default;
	cRow(int nDimension)
	{
		data_.resize(nDimension);
	}
	~cRow()
	{
		
	}
private :
	std::vector<float> data_;
public :
	float& operator[](int nIndex)
	{
		return data_[nIndex];
	}
};


class cMatrix
{
	int dimension_;
	std::vector<cRow> matrix_;
public:
	cMatrix();
	~cMatrix();
	cMatrix(int nDimension);
	cMatrix(int nDimension, int data);
	void Print();
	int Dimension();

	static cMatrix Identity(int nDimension = 4);

	bool operator==(cMatrix & mat);
	bool operator!=(cMatrix & mat);
	cMatrix operator+(cMatrix & mat);
	cMatrix operator-(cMatrix & mat);
	cMatrix operator*(cMatrix & mat);
	cMatrix operator*(float f);
	cMatrix operator/(float f);
	cRow& operator[](int nIndex);
	cMatrix Transpose();
	float Determinant();
	
	cMatrix Inverse();
	cMatrix Adjoint();
	float Cofactor(int nRow, int nCol);
	float Minor(int nRow, int nCol);

	static cMatrix Translation(float x, float y, float z);
	static cMatrix Translation(cVector3& v);
	static cMatrix RotationX(float angle);
	static cMatrix RotationY(float angle);
	static cMatrix RotationZ(float angle);
	static cMatrix View(cVector3& vEye, cVector3& vLookAt, cVector3& vUp);
	static cMatrix Projection(float fFovY = 45, float fAspect = 1, float fNearZ = 1, float fFarZ= 1000);
	static cMatrix Viewport(float x, float y, float w, float h, float minz = 0, float maxz = 1);

	static cVector3 TransformCoord(cVector3& v, cMatrix& mat);
	static cVector3 TransformNormal(cVector3& v, cMatrix& mat);
};

inline cRow& cMatrix::operator[](int nIndex)
{
	return (matrix_[nIndex]);
}

