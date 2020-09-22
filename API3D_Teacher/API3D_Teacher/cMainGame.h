#pragma once
class cVector3;
class cMatrix;

class cMainGame
{
public:
	cMainGame();
	~cMainGame();

private:
	HDC m_MemDc;
	HBITMAP m_hOldBitmap, m_hBitmap;

	vector<cVector3> m_vecVertex;
	vector<cVector3> m_gridVertex;
	
	vector<DWORD>	m_vecIndex;
	vector<cVector3> m_gridIndex;

	cMatrix			m_matWorld;
	cMatrix			m_matView;
	cMatrix			m_matProj;
	cMatrix			m_matViewport;
	cMatrix			m_matRotationY;

	cVector3		m_vEye;
	cVector3		m_vLookat;
	cVector3		m_vUp;
	cVector3		m_vFront;

	cVector3		m_translate;
	cVector3		m_rotate;

public:
	void SetUp();
	void Update();
	void Render(HDC hdc);
	void WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
};

