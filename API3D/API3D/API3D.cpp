// API3D.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "API3D.h"

#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name

// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

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
    LoadStringW(hInstance, IDC_API3D, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_API3D));

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
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_API3D));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_API3D);
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
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

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
	std::vector<cVector3> rectangle;
	std::vector<cVector3> polygon;

    switch (message)
    {
	case WM_CREATE :
	{

		break;
	}
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // Parse the menu selections:
            switch (wmId)
            {
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
    case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hWnd, &ps);

		static cVector3 eye(2, 5, -5);
		static cVector3 lookAt(0, 0, 0);
		cVector3 up(0, 1, 0);
		RECT clientRect = {0,0,0,0};
		GetClientRect(hWnd, &clientRect);
  
		static cMatrix world = cMatrix::Translation(0,0,0) * 
			cMatrix::View(eye, lookAt, up) * 
			cMatrix::Projection(45, clientRect.right/ clientRect.bottom) * 
			cMatrix::Viewport(0, 0, clientRect.right, clientRect.bottom);
		std::vector<cVector3> coord;
    	std::vector<int> dir = { -1, 1};
		for (size_t i = 0; i < 2; i++)
		{
			for (size_t j = 0; j < 2; j++)
			{
				for (size_t k = 0; k < 2; k++)
				{
					rectangle.emplace_back(dir[i], dir[j], dir[k]);
				}
			}
		}
		polygon.emplace_back(1, 5, 3);
		polygon.emplace_back(5, 7, 3);
		polygon.emplace_back(1, 0, 2);
		polygon.emplace_back(1, 3, 2);
		polygon.emplace_back(0, 4, 2);
		polygon.emplace_back(4, 6, 2);
		polygon.emplace_back(4, 5, 7);
		polygon.emplace_back(4, 6, 7);
		polygon.emplace_back(1, 4, 5);
		polygon.emplace_back(1, 4, 0);
		polygon.emplace_back(3, 7, 6);
		polygon.emplace_back(3, 2, 6);
    	for (cVector3 c_vector3 : rectangle)
    	{
			coord.push_back(cVector3::TransformCoord(c_vector3, world));

			Rectangle(hdc, coord.back().get_x() -2, coord.back().get_y() -2, coord.back().get_x() +2, coord.back().get_y() + 2);
    	}
    	for(cVector3 poly : polygon)
    	{
			MoveToEx(hdc, coord[poly.get_x()].get_x(), coord[poly.get_x()].get_y(), NULL);
			LineTo(hdc, coord[poly.get_y()].get_x(), coord[poly.get_y()].get_y());
			LineTo(hdc, coord[poly.get_z()].get_x(), coord[poly.get_z()].get_y());
			LineTo(hdc, coord[poly.get_x()].get_x(), coord[poly.get_x()].get_y());
    	}
		EndPaint(hWnd, &ps);
	}
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// Message handler for about box.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}
