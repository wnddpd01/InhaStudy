#include "stdafx.h"
#include "cMatrix.h"

#define EPSILON 0.0001f

cMatrix::cMatrix()
{
}

cMatrix::~cMatrix()
{
}

cMatrix::cMatrix(int nDimension)
{
	srand(time(NULL));
	dimension = nDimension;

	for (int i = 0; i < nDimension; i++)
		matrix.emplace_back(nDimension);	//인자 넘겨주면서 생성
}

void cMatrix::Print()
{
	/*for (int i = 0; i < this->dimension; i++)
	{
		for (int j = 0; j < this->dimension; j++)
		{
			cout << matrix[i][j] << " ";
		}
		cout << endl;
	}
	cout << endl;*/

	for (int i = 0; i<dimension; i++)
	{
		for (int j = 0; j<dimension; j++)
		{
			printf("%8.2f", (*this)[i][j]);
		}
		printf("\n");
	}
	printf("\n");
}

void cMatrix::Resize(int nDimension)
{
}

cMatrix cMatrix::Identity(int nDimension)
{
	cMatrix cMat(nDimension);

	for(int i = 0; i < nDimension; i++)
	{
		for(int j = 0; j < nDimension; j++)
		{
			cMat.matrix[i][j] = (i == j) ? 1 : 0;
		}
	}
	return cMat;
}

cRow & cMatrix::operator[](int nIndex)
{
	return matrix[nIndex];
}

bool cMatrix::operator==(cMatrix & mat)
{
	for (int i = 0; i < this->dimension; i++)
	{
		for (int j = 0; j < this->dimension; j++)
		{
			if(this->matrix[i][j] != mat[i][j])
				return false;
		}
	}
	return true;
}

bool cMatrix::operator!=(cMatrix & mat)
{
	for (int i = 0; i < this->dimension; i++)
	{
		for (int j = 0; j < this->dimension; j++)
		{
			if (this->matrix[i][j] != mat[i][j])
				return true;
		}
	}
	return false;
}

cMatrix cMatrix::operator+(cMatrix & mat)
{
	cMatrix cMat(dimension);
	for (int i = 0; i < this->dimension; i++)
		for (int j = 0; j < this->dimension; j++)
			cMat[i][j] = this->matrix[i][j] + mat[i][j];
	return cMat;
}

cMatrix cMatrix::operator-(cMatrix & mat)
{
	cMatrix cMat(dimension);
	for (int i = 0; i < this->dimension; i++)
		for (int j = 0; j < this->dimension; j++)
			cMat[i][j] = this->matrix[i][j] - mat[i][j];
	return cMat;
}

cMatrix cMatrix::operator*(cMatrix & mat)
{
	cMatrix cMat(dimension);

	if (this->dimension == mat.dimension)
	{
		for (int i = 0; i < this->dimension; i++)
			for (int j = 0; j < this->dimension; j++)
			{
				cMat[i][j] = 0;
				for (int k = 0; k < this->dimension; k++)
					cMat[i][j] += this->matrix[i][k] * mat.matrix[k][j];
			}
		return cMat;
	}
	else
	{
		cout << L"곱할 수 없습니다." << endl;
		return 0;
	}
}

cMatrix cMatrix::operator*(float f)
{
	cMatrix cMat(dimension);
	for (int i = 0; i < this->dimension; i++)
		for (int j = 0; j < this->dimension; j++)
			cMat[i][j] = this->matrix[i][j] * f;
	return cMat;
}

cMatrix cMatrix::Transpose()
{
	cMatrix cMat(this->dimension);

	for (int i = 0; i < this->dimension; i++)
	{
		for (int j = 0; j < this->dimension; j++)
		{
				cMat[i][j] = matrix[j][i];
		}
	}
	return cMat;
}

cMatrix cMatrix::Inverse(OUT float & fDeterminant)
{
	cMatrix cMat = cMatrix::Identity(dimension);
	fDeterminant = Determinant();

	if(abs(fDeterminant) <= EPSILON)
	{
		cout << "역행렬이 없다." << endl;
		return cMat;
	}
	cMatrix matAdj = Adjoint();
	
	cMat = matAdj * (1.0f / fDeterminant);
	
	return cMat;
}

float cMatrix::Determinant()
{
	if (dimension == 1)
		return (*this)[0][0];
	else if (dimension == 2)
		return (*this)[0][0] * (*this)[1][1] - (*this)[1][0] * (*this)[0][1];	//ad - cb

	float fDeterminant = 0;
	for (int i = 0; i < dimension; i++)
		fDeterminant += ((*this)[i][0] * Cofactor(i, 0));

	return fDeterminant;
}

cMatrix cMatrix::Adjoint()
{
	cMatrix cMat(dimension);
	for (int i = 0; i < dimension; i++)
	{
		for(int j = 0; j < dimension; j++)
		{
			cMat[i][j] = Cofactor(j, i);
		}
	}
	return cMat;
}

float cMatrix::Cofactor(int nRow, int nCol)
{
	int nConst = 1;
	if ((nRow + nCol) % 2 != 0)
		nConst = -1;
	
	return nConst * Minor(nRow, nCol);
}

float cMatrix::Minor(int nRow, int nCol)
{
	cMatrix cMat(dimension - 1);
	int nMinorRow = 0;
	int nMinorCol = 0;
	
	for (int i = 0; i < dimension; i++)
	{
		if (nRow == i)
			continue;
		nMinorCol = 0;

		for (int j = 0; j < dimension; j++)
		{
			if(nCol == j)
				continue;
			cMat[nMinorRow][nMinorCol] = (*this)[i][j];
			++nMinorCol;
		}
		++nMinorRow;
	}
	return cMat.Determinant();
}

cMatrix cMatrix::Translation(float x, float y, float z)
{
	cMatrix cMat(4);
	cMat = cMat.Identity(cMat.dimension);
	cMat[3][0] = x;
	cMat[3][1] = y;
	cMat[3][2] = z;
	return cMat;
}

cMatrix cMatrix::Translation(cVector3 &v)
{
	cMatrix cMat(4);
	cMat = cMat.Identity(cMat.dimension);
	cMat[3][0] = v.x;
	cMat[3][1] = v.y;
	cMat[3][2] = v.z;
	return cMat;
}

cMatrix cMatrix::RotationX(float angle)
{
	cMatrix cMat(4);
	cMat = cMat.Identity(cMat.dimension);

	cMat[1][1] = cosf(degToRad(angle));
	cMat[1][2] = sinf(degToRad(angle));
	cMat[2][1] = -sinf(degToRad(angle));
	cMat[2][2] = cosf(degToRad(angle));

	return cMat;
}

cMatrix cMatrix::RotationY(float angle)
{
	cMatrix cMat(4);
	cMat = cMat.Identity(cMat.dimension);

	cMat[0][0] = cosf(degToRad(angle));
	cMat[0][2] = -sinf(degToRad(angle));
	cMat[2][0] = sinf(degToRad(angle));
	cMat[2][2] = cosf(degToRad(angle));

	return cMat;
}

cMatrix cMatrix::RotationZ(float angle)
{
	cMatrix cMat(4);
	cMat = cMat.Identity(cMat.dimension);

	cMat[0][0] = cosf(degToRad(angle));
	cMat[0][1] = sinf(degToRad(angle));
	cMat[1][0] = -sinf(degToRad(angle));
	cMat[1][1] = cosf(degToRad(angle));

	return cMat;
}

cMatrix cMatrix::View(cVector3 &vEye, cVector3 &vLookAt, cVector3 &vUp)
{
	/*
	viewMatrix
	eye, look at, up
	l = look vector
	r = right vector
	u = up vector
		r.x			u.x			l.x			0
		r.y			u.y			l.y			0
		r.z			u.z			l.z			0
	-r dot eye	-u dot eye	-l dot eye		1
	*/

	cMatrix cMat = cMatrix::Identity((4));
	cVector3 l(vLookAt.x, vLookAt.y, vLookAt.z, 1);
	cVector3 r;
	cVector3 u(vUp.x, vUp.y, vUp.z, 1);
	
	l = (vLookAt - vEye).Normalize();
	r = cVector3::Cross(vUp, l).Normalize();
	u = cVector3::Cross(l, r).Normalize();
	
	cMat[0][0] = r.x;	cMat[0][1] = u.x;	cMat[0][2] = l.x;
	cMat[1][0] = r.y;	cMat[1][1] = u.y;	cMat[1][2] = l.y;
	cMat[2][0] = r.z;	cMat[2][1] = u.z;	cMat[2][2] = l.z;
	cMat[3][0] = cVector3::Dot(r*-1, vEye);
	cMat[3][1] = cVector3::Dot(u*-1, vEye);
	cMat[3][2] = cVector3::Dot(l*-1, vEye);

	return cMat;
}

cMatrix cMatrix::Projection(float fFovY, float fAspect, float fNearZ, float fFarZ)
{
	/*
	 sy = cot(fovY /2)	=> 1.0f / tanf(fovY / 2.0f)
	 sx = sy / aspect
			sx		0		0				0
			0		sy		0				0
			0		0	fz / (fz-nz)		1
			0		0	-fz*nz / (fz-nz)	0
	*/
	
	cMatrix cMat = cMatrix::Identity((4));
	float sy = 1.0f / tanf(degToRad(fFovY) / 2.0f);
	float sx = sy / fAspect;

	cMat[0][0] = sx;
	cMat[1][1] = sy;
	cMat[2][2] = fFarZ / (fFarZ - fNearZ);
	cMat[3][2] = -fFarZ * fNearZ / (fFarZ - fNearZ);
	cMat[2][3] = 1;
	cMat[3][3] = 0;

	return cMat;
}

cMatrix cMatrix::Viewport(float x, float y, float w, float h, float minz, float maxz)
{
	/*
			w / 2.0f		0		0			0
			0			-h / 2.0f	0			0
			0				0		maxz-minz	0
			x+(w/2.0f)	y+(h/2.0f)	minz		1

			default => maxz = 1, minz = 0
	 */
	cMatrix cMat(4);
	cMat = cMat.Identity(cMat.dimension);

	cMat[0][0] = w / 2.0f;
	cMat[1][1] = -h / 2.0f;
	cMat[2][2] = maxz - minz;
	cMat[3][0] = x + (w / 2.0f);
	cMat[3][1] = y + (h / 2.0f);
	cMat[3][2] = minz;

	return cMat;
}

cRow::cRow()
{
}

cRow::cRow(int nDimension)
{
	for (int i = 0; i < nDimension; i++)
		m_vecData.push_back(rand() % 10);
}

cRow::~cRow()
{
}

void cRow::Resize(int nDimension)
{
}

float &cRow::operator[](int nIndex)
{
	return m_vecData[nIndex];
}
