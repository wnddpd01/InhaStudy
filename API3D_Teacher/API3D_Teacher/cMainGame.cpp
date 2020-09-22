#include "stdafx.h"
#include "cMainGame.h"

#define GRID_ROW 31
#define GRID_COL 31

cMainGame::cMainGame() : m_hBitmap(NULL), m_vEye(0, 0, -5, 1), m_vLookat(0, 0, 0, 1), m_vUp(0, 1, 0, 1)
{
}

cMainGame::~cMainGame()
{
	SelectObject(m_MemDc, m_hOldBitmap);
	DeleteObject(m_hBitmap);
	DeleteDC(m_MemDc);
}

void cMainGame::SetUp()
{
	HDC hdc = GetDC(g_hWnd);
	m_MemDc = CreateCompatibleDC(hdc);

	RECT rc;
	GetClientRect(g_hWnd, &rc);
	m_hBitmap = CreateCompatibleBitmap(hdc, rc.right, rc.bottom);
	m_hOldBitmap = (HBITMAP)SelectObject(m_MemDc, m_hBitmap);
	ReleaseDC(g_hWnd, hdc);

	m_vecVertex.push_back(cVector3(-1.0f, -1.0f, -1.0f, 1));
	m_vecVertex.push_back(cVector3(-1.0f, 1.0f, -1.0f, 1));
	m_vecVertex.push_back(cVector3(1.0f, 1.0f, -1.0f, 1));
	m_vecVertex.push_back(cVector3(1.0f, -1.0f, -1.0f, 1));
	
	m_vecVertex.push_back(cVector3(-1.0f, -1.0f, 1.0f, 1));
	m_vecVertex.push_back(cVector3(-1.0f, 1.0f, 1.0f, 1));
	m_vecVertex.push_back(cVector3(1.0f, 1.0f, 1.0f, 1));
	m_vecVertex.push_back(cVector3(1.0f, -1.0f, 1.0f, 1));

	//front
	m_vecIndex.push_back(0);
	m_vecIndex.push_back(1);
	m_vecIndex.push_back(2);

	m_vecIndex.push_back(0);
	m_vecIndex.push_back(2);
	m_vecIndex.push_back(3);

	//back
	m_vecIndex.push_back(4);
	m_vecIndex.push_back(6);
	m_vecIndex.push_back(5);

	m_vecIndex.push_back(4);
	m_vecIndex.push_back(7);
	m_vecIndex.push_back(6);

	//left
	m_vecIndex.push_back(4);
	m_vecIndex.push_back(5);
	m_vecIndex.push_back(1);

	m_vecIndex.push_back(4);
	m_vecIndex.push_back(1);
	m_vecIndex.push_back(0);

	//right
	m_vecIndex.push_back(3);
	m_vecIndex.push_back(2);
	m_vecIndex.push_back(6);

	m_vecIndex.push_back(3);
	m_vecIndex.push_back(6);
	m_vecIndex.push_back(7);

	//top
	m_vecIndex.push_back(1);
	m_vecIndex.push_back(5);
	m_vecIndex.push_back(6);

	m_vecIndex.push_back(1);
	m_vecIndex.push_back(6);
	m_vecIndex.push_back(2);

	//buttom
	m_vecIndex.push_back(4);
	m_vecIndex.push_back(0);
	m_vecIndex.push_back(3);

	m_vecIndex.push_back(4);
	m_vecIndex.push_back(3);
	m_vecIndex.push_back(7);

	m_matWorld = cMatrix::Identity((4));
	m_matView = cMatrix::Identity((4));
	m_matProj = cMatrix::Identity((4));
	m_matViewport = cMatrix::Identity((4));

	for (int i = -(GRID_COL / 2); i <= GRID_COL / 2; i++)
	{
		m_gridVertex.emplace_back(i, 0, -(GRID_COL / 2), 1);
		m_gridVertex.emplace_back(i, 0, GRID_COL / 2, 1);
	}

	for (int i = -(GRID_ROW / 2); i <= GRID_ROW / 2; i++)
	{
		m_gridVertex.emplace_back(-(GRID_ROW / 2), 0, i, 1);
		m_gridVertex.emplace_back(GRID_ROW / 2, 0, i, 1);
	}
	m_vLookat = cVector3(0, 0, 0, 1);
	m_vEye = cVector3(0, 5.0f, -15.0f, 1);
	m_vFront = cVector3(0, 0, 1, 1);
}

void cMainGame::Update()
{
	RECT rc;
	GetClientRect(g_hWnd, &rc);


}

void cMainGame::Render(HDC hdc)
{
	RECT rc;
	GetClientRect(g_hWnd, &rc);

	m_matWorld = cMatrix::Translation(0, 0, 0);
	m_matView = cMatrix::View(m_vEye, m_vLookat, m_vUp);
	m_matProj = cMatrix::Projection(45, rc.right / (float)rc.bottom, 1.0f, 1000.0f);
	m_matViewport = cMatrix::Viewport(0, 0, rc.right, rc.bottom, 0, 1);
	PatBlt(m_MemDc, rc.left, rc.top, rc.right, rc.bottom, WHITENESS);
	cMatrix matWVP = m_matWorld * m_matView * m_matProj;
	for(int i = 0; i < m_vecIndex.size(); i += 3)
	{
		cVector3 v1 = m_vecVertex[m_vecIndex[i + 0]];
		cVector3 v2 = m_vecVertex[m_vecIndex[i + 1]];
		cVector3 v3 = m_vecVertex[m_vecIndex[i + 2]];

		v1 = cVector3::TransformCoord(v1, matWVP);
		v2 = cVector3::TransformCoord(v2, matWVP);
		v3 = cVector3::TransformCoord(v3, matWVP);

		v1 = cVector3::TransformCoord(v1, m_matViewport);
		v2 = cVector3::TransformCoord(v2, m_matViewport);
		v3 = cVector3::TransformCoord(v3, m_matViewport);

		MoveToEx(m_MemDc, v1.x, v1.y, NULL);
		LineTo(m_MemDc, v2.x, v2.y);
		LineTo(m_MemDc, v3.x, v3.y);
		LineTo(m_MemDc, v1.x, v1.y);
	}
	m_gridIndex.clear();
	for (cVector3 c_vector3 : m_gridVertex)
	{
		m_gridIndex.push_back(cVector3::TransformCoord(c_vector3, matWVP * m_matViewport));
	}
	for (int i = 0; i < (GRID_COL + GRID_ROW) * 2 - 1; i += 2)
	{
		MoveToEx(m_MemDc, m_gridIndex[i].x, m_gridIndex[i].y, NULL);
		LineTo(m_MemDc, m_gridIndex[i + 1].x, m_gridIndex[i + 1].y);
	}
	BitBlt(hdc, 0, 0, rc.right, rc.bottom, m_MemDc, 0, 0, SRCCOPY);
}

void cMainGame::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	if (GetKeyState(VK_LEFT) & 0x8000)
	{
		cVector3 right_v_vector3 = cVector3::Cross(cVector3(0, 1, 0, 1), m_vFront).Normalize() * 0.2f;
		for (int i = 0; i < m_vecVertex.size(); ++i)
		{
			m_vecVertex[i] = m_vecVertex[i] + right_v_vector3;
		}
		m_vEye = m_vEye + right_v_vector3;
		m_vLookat = m_vLookat + right_v_vector3;
		
	}
	if (GetKeyState(VK_RIGHT) & 0x8000)
	{
		cVector3 right_v_vector3 = cVector3::Cross(cVector3(0, 1, 0, 1), m_vFront).Normalize() * -0.2f;
		for (int i = 0; i < m_vecVertex.size(); ++i)
		{
			m_vecVertex[i] = m_vecVertex[i] + right_v_vector3;
		}
		m_vEye = m_vEye + right_v_vector3;
		m_vLookat = m_vLookat + right_v_vector3;
	}
	if (GetKeyState(VK_UP) & 0x8000)
	{
		for (int i = 0; i < m_vecVertex.size(); ++i)
		{
			m_vecVertex[i] = m_vecVertex[i] + m_vFront * 0.2f;
		}
		m_vEye = m_vEye +m_vFront * 0.2f;
		m_vLookat = m_vLookat + m_vFront * 0.2f;
	}
	if (GetKeyState(VK_DOWN) & 0x8000)
	{
		for (int i = 0; i < m_vecVertex.size(); ++i)
		{
			m_vecVertex[i] = m_vecVertex[i] - m_vFront * 0.2f;
		}
		m_vEye = m_vEye - m_vFront * 0.2f;
		m_vLookat = m_vLookat - m_vFront * 0.2f;
	}
	if (GetKeyState('A') & 0x8000)
	{
		for (size_t i = 0; i < m_vecVertex.size(); i++)
		{
			m_vecVertex[i] = cVector3::TransformCoord(m_vecVertex[i] - m_vLookat, cMatrix::RotationY(-1)) + m_vLookat;
		}
		m_vFront = (cVector3::TransformCoord(m_vFront, cMatrix::RotationY(-1))).Normalize();
		m_vEye = cVector3::TransformCoord(m_vEye - m_vLookat, cMatrix::RotationY(-1)) + m_vLookat;
	}
	if (GetKeyState('D') & 0x8000)
	{
		for (size_t i = 0; i < m_vecVertex.size(); i++)
		{
			m_vecVertex[i] = cVector3::TransformCoord(m_vecVertex[i] - m_vLookat, cMatrix::RotationY(+1)) + m_vLookat;
		}
		m_vFront = (cVector3::TransformCoord(m_vFront, cMatrix::RotationY(+1))).Normalize();
		m_vEye = cVector3::TransformCoord(m_vEye - m_vLookat, cMatrix::RotationY(+1)) + m_vLookat;
	}

	switch (message)
	{
	case WM_LBUTTONDOWN:
		break;
	case WM_LBUTTONUP:
		break;
	case WM_MOUSEMOVE:
		switch (wParam)
		{
		case MK_LBUTTON:
		{
			static int oldY = HIWORD(lParam);
			static int oldX = LOWORD(lParam);
			int newX = LOWORD(lParam);
			int newY = HIWORD(lParam);
			cVector3 vRight = cVector3::Cross(m_vUp, m_vLookat - m_vEye).Normalize();
			cVector3 vUp = cVector3::Cross(m_vLookat - m_vEye, vRight).Normalize();
			int diffX = oldX - newX;
			int diffY = oldY - newY;
			if(abs(diffX) > 5)
			{
				if(diffX < 0)
				{
					m_vEye = m_vEye + vRight;
				}
				else
				{
					m_vEye = m_vEye - vRight;
				}
				oldX = newX;
			}
	
			if (abs(diffY) > 5)
			{
				if (diffY < 0)
				{
					m_vEye = m_vEye - vUp;
				}
				else
				{
					m_vEye = m_vEye + vUp;
				}
				oldY = newY;
			}
		}
		break;
		}
		break;
	case WM_MOUSEWHEEL:
	{
		int zDelta = GET_WHEEL_DELTA_WPARAM(wParam);
		static int count = 0;
		switch (zDelta)
		{
		case -120:
			if (count > -8)
			{
				count--;
				m_vEye = m_vEye + (m_vEye - m_vLookat).Normalize();
			}
			break;
		case 120:
			if (count < 8)
			{
				count++;
				m_vEye = m_vEye + (m_vLookat - m_vEye).Normalize();
			}
			break;
		}
	}
	break;
	default:
		break;
	}
}
