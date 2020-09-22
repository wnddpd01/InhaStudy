#pragma once
class cRow
{
	vector<float> m_vecData;
public:
	cRow();
	cRow(int nDimension);
	~cRow();
	void Resize(int nDimension);
	float &operator[](int nIndex);
};

class cMatrix
{
public:
	cMatrix();
	~cMatrix();
	vector<cRow> matrix;
	int dimension;

	cMatrix(int nDimension);	//정방행렬

	void Print();
	void Resize(int nDimension);
	
	static cMatrix Identity(int nDimension);	//항등행렬
	cRow &operator[](int nIndex);

	// =  = = = = = ===================================
	bool operator == (cMatrix& mat);
	bool operator != (cMatrix& mat);
	cMatrix operator + (cMatrix &mat);
	cMatrix operator - (cMatrix &mat);
	cMatrix operator * (cMatrix &mat);
	cMatrix operator * (float f);
	cMatrix Transpose();
	cMatrix Inverse(OUT float& fDeterminant);
	float Determinant();
	cMatrix Adjoint();
	float Cofactor(int nRow, int nCol);
	float Minor(int nRow, int nCol);	//minor 행렬의 determinant 구하기

	static cMatrix Translation(float x, float y, float z);
	static cMatrix Translation(cVector3 &v);
	static cMatrix RotationX(float angle);
	static cMatrix RotationY(float angle);
	static cMatrix RotationZ(float angle);
	static cMatrix View(cVector3 &vEye, cVector3 &vLookAt, cVector3 &vUp);
	static cMatrix Projection(float fFovY, float fAspect, float fNearZ, float fFarZ);
	static cMatrix Viewport(float x, float y, float w, float h, float minz, float maxz);
};