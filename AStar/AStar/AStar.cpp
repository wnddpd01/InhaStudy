// AStar.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "AStar.h"

#include <functional>

#define MAX_LOADSTRING 100
#define BOARD_WHITE_SPACE 39
#define BOARD_ONE_SPACE_LENGTH 38
#define BOARD_WIDTH  768
#define BOARD_HEIGHT 768
#define CELL_NUM 18
enum cell_state : char
{
	cell_open = 0, cell_calculated ,cell_close, cell_block, cell_start, cell_end
};

class cell final
{
	POINT pos_;
	cell_state m_cell_state_;
	size_t f_val_;
	size_t h_val_;
	size_t g_val_;
public:
#pragma region cellSetterAndGetter
	POINT pos() const
	{
		return pos_;
	}

	void set_pos(POINT pos)
	{
		this->pos_ = pos;
	}
	
	cell_state m_cell_state() const
	{
		return m_cell_state_;
	}

	void set_m_cell_state(cell_state m_cell_state)
	{
		m_cell_state_ = m_cell_state;
	}

	size_t f_val() const
	{
		return f_val_;
	}

	void set_f_val(size_t f_val)
	{
		f_val_ = f_val;
	}

	size_t h_val() const
	{
		return h_val_;
	}

	void set_h_val(size_t h_val)
	{
		h_val_ = h_val;
	}

	size_t g_val() const
	{
		return g_val_;
	}

	void set_g_val(size_t g_val)
	{
		g_val_ = g_val;
	}

#pragma endregion

#pragma region cellOperatorOverride
	friend bool operator< (const cell& cell1, const cell &cell2)
	{
		int a = 0;
		return cell1.f_val() < cell2.f_val();
	}
	friend bool operator> (const cell& cell1, const cell &cell2)
	{
		return cell1.f_val() > cell2.f_val();
	}
#pragma endregion

	cell()
	{
		f_val_ = h_val_ = g_val_ = INF;
		//pos_ = { INF, INF };
		m_cell_state_ = cell_open;
	}
	~cell() = default;

};
using namespace  std;
//struct cmp {
//	bool operator()(cell * t, cell * u) {
//		return t->f_val() > u->f_val();
//	}
//};
cell map[CELL_NUM][CELL_NUM];
// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name
HWND hwnd;

priority_queue<cell * , vector<cell *>, greater<vector<cell *>::value_type>> pq;
//priority_queue<cell *> pq;
//priority_queue<cell *, vector<cell *>, cmp> pq;
// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);

bool PtInBoard(POINT & p);
void MapInit();
void DrawBoard(HDC hdc);
void DrawCell(HDC hdc);
void TextOutCell(HDC hdc, size_t left, size_t top, cell & cell);
bool AStarPathFinding(POINT &start_point, POINT& end_point);
void AStartCalculateNode(POINT &start_point, POINT& end_point, POINT& cur_point);
int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: Place code here.

    // Initialize global strings
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_ASTAR, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_ASTAR));

    MSG msg;

    // Main message loop:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
}



//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ASTAR));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = NULL;
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // Store instance handle in our global variable

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, BOARD_WIDTH + 16, BOARD_HEIGHT + 39, nullptr, nullptr, hInstance, nullptr);
   hwnd = hWnd;
   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_COMMAND  - process the application menu
//  WM_PAINT    - Paint the main window
//  WM_DESTROY  - post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
	case WM_CREATE :
		{
			MapInit();
			SetTimer(hWnd, 1, 1000 / 10, NULL);
			SetBkMode(GetDC(hWnd), TRANSPARENT);
		}
		break;
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);

			static HDC backHdc = NULL;
    		if(backHdc == NULL)
    		{
				backHdc = CreateCompatibleDC(hdc);
				static HBITMAP backHBit = CreateCompatibleBitmap(hdc, BOARD_WIDTH, BOARD_HEIGHT);
				SelectObject(backHdc, backHBit);
    		}
            // TODO: Add any drawing code that uses hdc here...
			DrawBoard(backHdc);
			DrawCell(backHdc);

			BitBlt(hdc, 0, 0, BOARD_WIDTH, BOARD_HEIGHT, backHdc, 0, 0, SRCCOPY);
            EndPaint(hWnd, &ps);
        }
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
	case WM_LBUTTONDOWN:
		{
			static bool pointStart = true;
			static POINT startPoint = {-1,-1};	
			POINT xy = { LOWORD(lParam), HIWORD(lParam) }; 
			if (PtInBoard(xy) == true)
			{
				xy.x = (xy.x - BOARD_WHITE_SPACE) / BOARD_ONE_SPACE_LENGTH;
				xy.y = (xy.y - BOARD_WHITE_SPACE) / BOARD_ONE_SPACE_LENGTH;
				if (pointStart == true)
				{
					MapInit();
					map[xy.y][xy.x].set_m_cell_state(cell_start);
					startPoint = xy;
				}
				else
				{
					map[xy.y][xy.x].set_m_cell_state(cell_end);
					AStarPathFinding(startPoint, xy);
					return 0;
				}
				pointStart = !pointStart;
			}
		}
		break;
	case WM_RBUTTONDOWN :
		{
			POINT xy = { LOWORD(lParam), HIWORD(lParam) };
			if (PtInBoard(xy) == true)
			{
				size_t i = (xy.y - BOARD_WHITE_SPACE) / BOARD_ONE_SPACE_LENGTH;	
				size_t j = (xy.x - BOARD_WHITE_SPACE) / BOARD_ONE_SPACE_LENGTH;
				map[i][j].set_m_cell_state(cell_block);
			}
		}
		break;
	case WM_TIMER :
		{
			InvalidateRect(hWnd, NULL, FALSE);
		}
		break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

bool PtInBoard(POINT & p)
{
	if (p.x > BOARD_WHITE_SPACE && p.x < BOARD_WIDTH - BOARD_WHITE_SPACE &&
		p.y > BOARD_WHITE_SPACE && p.y < BOARD_HEIGHT - BOARD_ONE_SPACE_LENGTH)
		return true;
	return false;
}

void MapInit()
{
	for (size_t i = 0; i < CELL_NUM; i++)
	{
		for (size_t j = 0; j < CELL_NUM; j++)
		{
			map[i][j].set_m_cell_state(cell_open);
			POINT ij = { j, i };
			map[i][j].set_pos(ij);
		}
	}
}

void DrawBoard(HDC hdc)
{
	static HBRUSH boardBrush = CreateSolidBrush(RGB(125, 125, 255));
	//static HBRUSH flowerPtBrush = CreateSolidBrush(RGB(0, 0, 0));
	HBRUSH oldBrush = (HBRUSH)SelectObject(hdc, boardBrush);
	Rectangle(hdc, 0, 0, BOARD_WIDTH, BOARD_HEIGHT);
	////SelectObject(hdc, flowerPtBrush);
	
	/*for (size_t i = 0; i < 3; i++)
	{
		Circle(hdc, BOARD_WHITE_SPACE + (BOARD_ONE_SPACE_LENGTH * 3), BOARD_WHITE_SPACE + (BOARD_ONE_SPACE_LENGTH * 3) + (BOARD_ONE_SPACE_LENGTH * 6) * i, 5);
		Circle(hdc, BOARD_WHITE_SPACE + (BOARD_ONE_SPACE_LENGTH * 9), BOARD_WHITE_SPACE + (BOARD_ONE_SPACE_LENGTH * 3) + (BOARD_ONE_SPACE_LENGTH * 6) * i, 5);
		Circle(hdc, BOARD_WHITE_SPACE + (BOARD_ONE_SPACE_LENGTH * 15), BOARD_WHITE_SPACE + (BOARD_ONE_SPACE_LENGTH * 3) + (BOARD_ONE_SPACE_LENGTH * 6) * i, 5);
	}*/	
	/*for (size_t i = 0; i < 19; i++)
	{
		MoveToEx(hdc, BOARD_WHITE_SPACE, BOARD_WHITE_SPACE + BOARD_ONE_SPACE_LENGTH * i, NULL);
		LineTo(hdc, 684 + BOARD_WHITE_SPACE, BOARD_WHITE_SPACE + BOARD_ONE_SPACE_LENGTH * i);
		MoveToEx(hdc, BOARD_WHITE_SPACE + BOARD_ONE_SPACE_LENGTH * i, BOARD_WHITE_SPACE, NULL);
		LineTo(hdc, BOARD_WHITE_SPACE + BOARD_ONE_SPACE_LENGTH * i, 684 + BOARD_WHITE_SPACE);
	}*/
	SelectObject(hdc, boardBrush);
}

void DrawCell(HDC hdc)
{
	static HBRUSH OpenBrush  = CreateSolidBrush(RGB(255, 255, 255));
	static HBRUSH CalBrush   = CreateSolidBrush(RGB(31, 211, 85));
	static HBRUSH CloseBrush = CreateSolidBrush(RGB(255, 127, 127));
	static HBRUSH BlockBrush = CreateSolidBrush(RGB(160, 160, 160));
	static HBRUSH StartBrush = CreateSolidBrush(RGB(100, 175, 220));
	static HBRUSH EndBrush   = CreateSolidBrush(RGB(0, 128, 192));
	static HPEN   CellPen = CreatePen(PS_SOLID, 2, RGB(0, 0, 0));
	HBRUSH oldBrush = (HBRUSH)SelectObject(hdc, OpenBrush);
	HPEN oldPen = (HPEN)SelectObject(hdc, CellPen);
	for (size_t i = 0; i < CELL_NUM; i++)
	{
		for (size_t j = 0; j < CELL_NUM; j++)
		{
			switch (map[i][j].m_cell_state())
			{
			case cell_open :
				SelectObject(hdc, OpenBrush);
				break;
			case cell_calculated :
				SelectObject(hdc, CalBrush);
				break;
			case cell_close :
				SelectObject(hdc, CloseBrush);
				break;
			case cell_block :
				SelectObject(hdc, BlockBrush);
				break;
			case cell_start :
				SelectObject(hdc, StartBrush);
				break;
			case cell_end :
				SelectObject(hdc, EndBrush);
				break;
			}
			const size_t left = BOARD_WHITE_SPACE + j * BOARD_ONE_SPACE_LENGTH;
			const size_t top  = BOARD_WHITE_SPACE + i * BOARD_ONE_SPACE_LENGTH;
			Rectangle(hdc, left, top, left + BOARD_ONE_SPACE_LENGTH, top + BOARD_ONE_SPACE_LENGTH);
			if (map[i][j].m_cell_state() == cell_calculated || map[i][j].m_cell_state() == cell_close)
				TextOutCell(hdc, left, top, map[i][j]);
		}
	}
	SelectObject(hdc, oldPen);
	SelectObject(hdc, oldBrush);
}

void TextOutCell(HDC hdc, size_t left, size_t top, cell & cell)
{
	//static HFONT cell_font;
	RECT gh_val_rect = { left + 1 , top + 1, left + 37, top + 10 };
	RECT f_val_rect = { gh_val_rect.left, gh_val_rect.bottom + 15, gh_val_rect.right, gh_val_rect.bottom + 29 };
	char buffer[8];
	if (cell.g_val() == INF)
	{
		sprintf(buffer, "INF");
	}
	else
	{
		sprintf(buffer, "%-3d", cell.g_val());
	}
	if (cell.h_val() == INF)
	{
		sprintf(&buffer[3], "INF");
	}
	else
	{
		sprintf(&buffer[3], "%3d", cell.h_val());
	}
	DrawTextA(hdc, buffer, strlen(buffer), &gh_val_rect, DT_SINGLELINE | DT_VCENTER);

	if (cell.f_val() == INF)
	{
		sprintf(buffer, "INF");
	}
	else
	{
		sprintf(buffer, "%d", cell.f_val());
	}
	DrawTextA(hdc, buffer, strlen(buffer), &f_val_rect, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
}

bool AStarPathFinding(POINT& start_point, POINT& end_point)
{
	static POINT cur_point;
	pq.push(&(map[start_point.y][start_point.x]));
	static int count = 0;
	while (count < 30)
	{
		cur_point = (*pq.top()).pos();
		pq.pop();
		AStartCalculateNode(start_point, end_point, cur_point);
		map[cur_point.y][cur_point.x].set_m_cell_state(cell_close);
		count++;
	}
	count = 0;
	return true;
}

void AStartCalculateNode(POINT& start_point, POINT& end_point, POINT& cur_point)
{
	POINT temp_point;

	for (size_t i = 0; i < 4; i++)
	{
		switch (i)
		{
		case 0 :
			temp_point = { cur_point.x - 1, cur_point.y };
			break;
		case 1 :
			temp_point = { cur_point.x, cur_point.y - 1 };
			break;
		case 2 :
			temp_point = { cur_point.x + 1, cur_point.y };
			break;
		case 3 :
			temp_point = { cur_point.x, cur_point.y + 1 };
			break;
		}
		if (temp_point.x < 0 || temp_point.x == CELL_NUM || temp_point.y < 0 || temp_point.y == CELL_NUM || map[temp_point.y][temp_point.x].m_cell_state() != cell_open)
			continue;
		size_t g_val = getDistance(start_point, temp_point);
		size_t h_val = getDistance(temp_point, end_point);
		map[temp_point.y][temp_point.x].set_g_val(g_val);
		map[temp_point.y][temp_point.x].set_h_val(h_val);
		map[temp_point.y][temp_point.x].set_f_val(g_val + h_val);
		map[temp_point.y][temp_point.x].set_m_cell_state(cell_calculated);
		pq.push(&(map[temp_point.y][temp_point.x]));
	}
}
